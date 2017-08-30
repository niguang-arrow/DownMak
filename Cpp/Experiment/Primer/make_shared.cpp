#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <initializer_list>

using namespace std;

class strBlobPtr;

class strBlob {
    friend class strBlobPtr;
public:
    typedef vector<string>::size_type size_type;
    strBlob() :
        data(make_shared<vector<string>>()) {}
    strBlob(initializer_list<string> il) :
        data(make_shared<vector<string>>(il)) {}
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    void push_back(const string &s) { data->push_back(s); }
    void pop_back() {
        check(0, "pop_back on empty StrBlob");
        data->pop_back();
    }

    string& operator[](size_type index) {
        return (*data)[index];
    }

    const string& operator[](size_type index) const {
        return (*data)[index];
    }

    string& front() {
         //如果 vector 为空, 抛出一个异常
        check(0, "front on empty StrBlob");
        return data->front();
    }
    string& back() {
        check(0, "back on empty StrBlob");
        return data->back();
    }

    strBlobPtr begin();
    strBlobPtr end();

private:
    shared_ptr<vector<string>> data;
     //如果 data[i] 不合法, 抛出一个异常
    void check(size_type i, const string &msg) const {
        if (i >= data->size())
            throw out_of_range(msg);
    }

};

class strBlobPtr {
    friend bool operator==(const strBlobPtr &lhs, const strBlobPtr &rhs) {
        auto l = lhs.wptr.lock(), r = rhs.wptr.lock();
        if (l == r)
            // if they are both null or point to the same data;
            return (!r || lhs.curr == rhs.curr);
        else
            return false;
    }
    friend bool operator!=(const strBlobPtr &lhs, const strBlobPtr &rhs) {
        return !(lhs == rhs);
    }
public:
    strBlobPtr() :
        curr(0) {}
    strBlobPtr(strBlob &a, size_t sz = 0) :
        wptr(a.data), curr(sz) {}
    string& operator*() const {
        auto p = check(curr, "dereference past end");
        return (*p)[curr];
    }
    strBlobPtr& operator++() {
        check(curr, "increment past end");
        ++curr;
        return *this;
    }
    strBlobPtr operator++(int) {
        check(curr, "increment past end");
        auto ret = *this;
        ++(*this);
        return ret;
    }

private:
    weak_ptr<vector<string>> wptr; 
    size_t curr; 
    shared_ptr<vector<string>> check(size_t t, const string &msg) const {
        auto ret = wptr.lock();
        if (!ret)
            throw runtime_error("unbound StrBlobPtr");
        if (t >= ret->size())
            throw out_of_range(msg);
        return ret;
    }
};


inline strBlobPtr strBlob::begin() { return strBlobPtr(*this); }
inline strBlobPtr strBlob::end() { return strBlobPtr(*this, this->size()); }

int main() {

    initializer_list<string> il = {"a", "b", "c"};
    shared_ptr<vector<string>> str = make_shared<vector<string>>(il);
    auto a = *str;
    cout << (*str)[1] << endl;

    strBlob p1 = {"a", "b", "c", "d"};
    // 使用下标运算符
    auto p2 = p1;
    for (size_t i = 0; i < p2.size(); ++i)
        cout << p2[i] << " ";
    cout << endl;
    cout << p2.size() << endl;

    // 使用 begin()
    cout << *(p1.begin()) << endl;

    // 使用伴随类
    strBlobPtr strp(p1);
    cout << *strp << endl;

    // 使用范围 for
    for (const auto &p : p1)
        cout << p << " ";
    cout << endl;
    return 0;
}
