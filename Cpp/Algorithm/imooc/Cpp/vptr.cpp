#include <iostream>

using namespace std;

class Base {
public:
    Base(int x = 10) : ib(x) {}
    virtual void foo() { cout << "Base::foo()" << endl; }
    virtual ~Base() { cout << "Base::~Base()" << endl; }
private:
    int ib;
};

class Derived : public Base {
public:
    Derived(int x = 10) : Base(x), id(x) {}
    virtual void foo() { id = 10000; cout << "Derived::foo()" << endl; }
    virtual ~Derived() { cout << "Derived::~Derived()" << endl; }
public:
    int id;
};

int main() {
    
    Derived d(100);
    cout << "Before: d.id = " << d.id << endl;
    Base *pd = &d;
    pd->foo();
    cout << "After d.id = " << d.id << endl;
    Derived *pdd = (Derived*)pd;
    cout << "pdd->id: " << pdd->id << endl;

    //Base b;

    //cout << "obj address: " << &b << endl;
    //auto p = (int*)(&b);
    //cout << "vptr address: " << p << endl;

    //auto t = (int*)*p;
    //cout << "vtbl address: " << t << endl;

    //typedef void(*Func)();
    //Func f = (Func)*t;
    //f();
}
