#include <iostream>

using namespace std;

class Uncopyable {
public:
    Uncopyable() {} // 允许 derived 对象构造和析构
    ~Uncopyable() {}
private:
    Uncopyable(const Uncopyable&); // 但是阻止 copying
    Uncopyable& operator=(const Uncopyable&);
};

class Base {
public:
    void pubf(int x = 1) {
        func(x);
    }
    virtual ~Base() {}
private:
    virtual void func(int x) { cout << "Base::" << x << endl; }
};

class Derived : public Base {
public:
    void func(int x) { cout << "Derived::" << x << endl; }
};


class Empty {};
class A {
public:
    int x;
    Empty a;
};

class B : public Empty {
public:
    int x;
};

int main(){
    cout << std::boolalpha << (sizeof(A) == sizeof(int)) << endl;
    cout << std::boolalpha << (sizeof(B) == sizeof(int)) << endl;
}

