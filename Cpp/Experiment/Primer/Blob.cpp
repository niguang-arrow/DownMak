#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <initializer_list>
#include <type_traits> // 包含 remove_reference 等类型转换模板

using namespace std;

template <typename T>
class Blob {
public:
    typedef T value_type;
    typedef typename vector<T>::size_type size_type;
    Blob() : data(make_shared<vector<T>>()) {}
    Blob(initializer_list<T> il) : data(make_shared<vector<T>>(il)) {}
    Blob(const Blob &b) : data(shared_ptr<vector<T>>(b.data)) {}
    // 自己定义了移动构造函数
    Blob(Blob &&b) : data(shared_ptr<vector<T>>(new vector<T>(std::move(*(b.data))))) {}
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }

    // 添加或删除元素
    void push_back(const T &t) { data->push_back(t); }
    void push_back(T &&t) { data->push_back(std::move(t)); }
    void pop_back();
    // 元素访问
    T& back();
    T& operator[](size_type i);
private:
    shared_ptr<vector<T>> data;
    void check(size_type i, const string &msg) const;
};

template <typename T>
void Blob<T>::check(size_type i, const string &msg) const {
    if (i >= data->size())
        throw out_of_range(msg);
}

template <typename T>
T& Blob<T>::back() {
    check(0, "back on empty Blob");
    return data->back();
}

template <typename T>
T& Blob<T>::operator[](size_type i) {
    check(i, "subscript out of range");
    return (*data)[i];
}

template <typename T>
void Blob<T>::pop_back() { 
    check(0, "pop back on empty Blob");
    data->pop_back();
}

template <typename T>
int compare(const T &lhs, const T &rhs) {
    if (lhs < rhs) return 1;
    else if (rhs < lhs) return -1;
    else return 0;
}

// 我希望最后返回的是类型为 T 的值
// 当然这个例子有点强行..
template <typename T>
auto retVal(const T *pt) ->  // 使用尾置返回类型
        typename remove_pointer<decltype(pt)>::type // 推断 pt 的类型为指针
{
    return *pt;
}

// 翻转函数, flip 接受一个函数指针, 以及两个类型为 T1&& 和 T2&& 的额外参数
// 使用右值引用的目的是: 对于指向模板类型参数的右值引用, 它对应的实参的左值/右值
// 属性以及 const 属性能够得到保持.
// 为了理解为什么要使用 std::forward<Type> 函数, 首先需要观察 func 和 gunc 两个函数,
// 如果我使用 flip(f, j, 42), (f可以为 func 或者 gunc), j 是一个左值, 42 是右值
// 那么对于 f 为 func 时, 如果没有std::forward 也没有问题, 因为此时 T1 被推断为 int&,
// 而 T2 被推断为 int, 刚好符合 func 参数要求的类型.
// 但是当 f 为 gunc 时, 如果没有 std::forward, 这时候就有问题了, 虽然
// T1 仍是推断为 int&, T2 推断为 int, 但是将 t2 参数传入到 gunc 中的 int &&x 参数中就会
// 产生问题, 因此 x 是右值引用(和模板参数类型的右值引用不同), 无法绑定到 t2 这个左值
// 上! 所以为了保持 t2 的右值属性(因为传递给 t2 的实参是 42), 我们需要使用 std::forward
// 它能保持实参类型的所有细节(当它用于一个指向模板参数类型的右值引用时), 此时 
// std::forward<T2>(t2) 的类型为 int&&, 而 std::forward<T1>(t1) 为 int&.
template <typename F, typename T1, typename T2>
void flip(F f, T1 &&t1, T2 &&t2) {
    f(std::forward<T2>(t2), std::forward<T1>(t1));
}

void func(int x, int &y) { // 注意该函数第二个参数为左值引用
    cout << "func: " << x << " " << y << endl;
}

void gunc(int &&x, int &y) { // 注意该函数的第一个参数为左值引用
    cout << "gunc: " << x << " " << y << endl;
}

// 可变参数模板
template <typename ... Args>
void foo(const Args& ... rest) {
    cout << sizeof ... (Args) << endl;
    cout << sizeof ... (rest) << endl;
}


int main(int argc, const char *argv[]) {


    Blob<string> b1({"a", "b", "c"}), b2({"hello"}), b3 = {"World"};
    cout << b1.size() << " " << b2.size() << " " << b3.size() << endl;
    Blob<string> b4(b1);
    Blob<string> b5 = std::move(b2);
    cout << b4.size() << " " << b5.size() << endl;
    
    long num = 1000;
    // 显式模板实参, 实例化为 compare(const long&, const long&)
    // 同时 1024 还会转换为 long
    auto res = compare<long>(num, 1024);
    cout << res << endl;

    // 测试 type_traits 头文件中的 remove_reference
    string str = "abc";
    string *pstr = &str;
    cout << retVal(pstr) << endl;

    // 翻转函数
    int aa = 100;
    flip(func, aa, 42);
    flip(gunc, aa, 42);

    // char* 字符数组
    char p[] = "abcde";
    cout << p << endl; // 注意打印的不是地址值, 而是字符数组的内容

    // 可变参数模板
    foo(42, 42, 42);
    foo(42, 32);
    return 0;
}
