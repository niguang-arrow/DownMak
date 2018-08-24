#include <iostream>

using namespace std;

class Base {
public:
    Base() {}
    virtual void foo() { cout << "Base::foo()" << endl; }
    virtual ~Base() { cout << "Base::~Base()" << endl; }
};

int main() {
    Base b;

    cout << "obj address: " << &b << endl;
    auto p = (int*)(&b);
    cout << "vptr address: " << p << endl;

    auto t = (int*)*p;
    cout << "vtbl address: " << t << endl;

    typedef void(*Func)();
    Func f = (Func)*t;
    f();
}
