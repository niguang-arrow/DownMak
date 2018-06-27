/*
 * 关于继承方面的内容, C++ Primer 第 15 章的内容复习
 * 
 */
#include <iostream>
#include <string>

using namespace std;


template <typename T>
class Base {
private:
    T n;
public:
    Base(int m = 0) : n(m) {}
    virtual ~Base() {}
    virtual int count() const { return n + 10; }
};


// 虚函数, 覆盖override
template <typename T>
class Derived : public Base<T> {
public:
    Derived(int m = 0) : Base<T>(m) {}
    int count() const override { return 100; }
};

// 运行时绑定, 可以是指针或者引用
template <typename T>
void print_info(ostream &os, const Base<T> &ins) {
    os << ins.count() << endl;
}

class Quote {
public:
    Quote() = default;
    Quote(const string)
};


int main() {
    Derived<int> a = Derived<int>(10);
    cout << a.count() << endl;
    print_info(cout, a);
}
