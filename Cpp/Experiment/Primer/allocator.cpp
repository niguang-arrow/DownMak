#include <iostream>
#include <memory>
#include <initializer_list>
#include <utility> // pair 定义在该头文件中, move 也定义在该文件中.


using namespace std;

/*
 * vector 类将其元素定义在连续的内存当中, 为了获得可接受的性能, vector 预先分配
 * 足够的内存来保存可能需要的更多元素. vector 的每个添加元素的成员函数会检查是否
 * 有空间容纳更多的元素. 如果有, 成员函数会在下一个可用位置构造一个对象; 如果没有
 * 可用空间, vector 就会重新分配空间: 它获得新的空间, 将已有元素移动到新空间中,
 * 释放旧空间, 并添加新元素.
 * 
 * 下面定义 StrVec, 使用 allocator来获得原始内存, 由于其分配的内存是未构造的, 我们
 * 需要使用 construct 成员在原始内存中创建对象. 当我们删除一个元素时, 我们将使用
 * destroy 成员来销毁元素.
 * 
 * StrVec 有三个指针成员指向其元素所使用的内存:
 * elements, 指向分配的内存中的首元素
 * first_free, 指向最后一个实际元素之后的位置
 * cap, 指向分配的内存末尾之后的位置
 * 还有一个 alloc 的静态成员, 用于分配内存.
 */
class StrVec {
public:
    StrVec() :
        elements(nullptr), first_free(nullptr), cap(nullptr) {}
    StrVec(const StrVec&);
    StrVec(const initializer_list<string>);
    StrVec& operator=(const StrVec&);
    ~StrVec() { free(); }

    string& operator[](size_t index) {
        if (index >= size())
            throw out_of_range("index is out of range");
        return *(elements + index);
    }
    void push_back(const string&);
    size_t size() const { return first_free - elements; }
    size_t capacity() const { return cap - elements; }
    string* begin() const { return elements; }
    string* end() const { return first_free;  }

private:
    static allocator<string> alloc; // 分配元素
    // 被添加元素的函数所使用
    void chk_n_alloc() {
        if (size() == capacity())
            reallocate();
    }
    // 辅助函数, 被拷贝构造函数, 赋值运算符和析构函数所使用
    pair<string*, string*> alloc_n_copy(const string*, const string*);
    void free(); // 销毁元素并释放内存
    void reallocate(); // 获得更多内存并拷贝已有元素
    string *elements; // 指向数组的首元素
    string *first_free; // 指向数组第一个空闲元素的指针
    string *cap; // 指向数组尾后位置的指针
};

allocator<string> StrVec::alloc;

void StrVec::push_back(const string &s) {
    chk_n_alloc(); // 确保有空间容纳新元素
    // 在 first_free 指向的元素中构造 s 的副本
    alloc.construct(first_free++, s);
}


/*
 * 在写 reallocate 函数之前, 想一下它的作用:
 * 1. 为一个新的更大的 string 数组分配内存
 * 2. 在内存空间中的前一部分构造对象, 保存现有元素
 * 3. 销毁原内存空间中的元素, 并释放这块内存
 * 
 */
void StrVec::reallocate() {
    // 我们将分配当前大小两倍的内存空间
    auto newcapacity = size() ? 2 * size() : 1;
    // 分配新内存
    auto newdata = alloc.allocate(newcapacity);
    // 将数据从旧内存移动到新内存
    auto dest = newdata; // 指向新数组中下一个空闲位置
    auto elem = elements; // 指向旧数组中下一个元素
    for (size_t i = 0; i != size(); ++i)
        alloc.construct(dest++, std::move(*elem++));
    free(); // 一旦移动完元素就释放内存
    // 更新我们的数据结构
    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity;
}

void StrVec::free() {
    // 不能传递给 deallocate 一个空指针, 如果 elements 为 0, 函数什么也不做
    if (elements) {
        // 逆序销毁旧元素
        for (auto p = first_free; p != elements; /*空*/)
            alloc.destroy(--p);
        alloc.deallocate(elements, cap - elements);
    }
}

pair<string*, string*>
StrVec::alloc_n_copy(const string *b, const string *e) {
    // 分配空间保存给定范围中的元素
    auto data = alloc.allocate(e - b);
    // 初始化并返回一个 pair, 该 pair 由 data 和 uninitialized_copy 的返回值构成
    return {data, uninitialized_copy(b, e, data)};
}

StrVec::StrVec(const StrVec &s) {
    // 调用 alloc_n_copy 分配空间以容纳和 s 中一样多的元素
    auto newdata = alloc_n_copy(s.begin(), s.end());
    elements = newdata.first;
    first_free = cap = newdata.second;
}

StrVec& StrVec::operator=(const StrVec &s) {
    auto newdata = alloc_n_copy(s.begin(), s.end());
    free();
    elements = newdata.first;
    first_free = cap = newdata.second;
    return *this;
}

StrVec::StrVec(const initializer_list<string> il) {
    auto data = alloc.allocate(il.size());
    elements = data;
    first_free = cap = uninitialized_copy(il.begin(), il.end(), elements);
}

int main() {
    
    StrVec vec({"a", "b", "c", "d"});

    for (size_t i = 0; i != vec.size(); ++i)
        cout << vec[i] << " ";
    cout << endl;
    

    return 0;
}
