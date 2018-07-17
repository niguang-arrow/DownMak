#include <iostream>

using namespace std;

class A{
public:
    A() { cout << "A()" << endl; }
    virtual ~A() { cout << "~A()" << endl; }
};

class B: public A{
public:
    int *pb;
    B() : pb(new int(100)) { cout << "B()" << endl; }
    virtual ~B() { delete pb; cout << "~B()" << endl; }
};

int main() {
    B b;
    A *pb = &b;
    return 0;
}
