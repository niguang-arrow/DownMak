#include <iostream>

using namespace std;


class Base {
public:
    Base() : pub(10), num(0) {}
    void bfunc() { cout << "Base: " << num << endl; }
    int pub;
private:
    int num;
};

class Derived : public Base {
public:
    Derived() : Base(), num(10) {}
    void dfunc() { cout << "Derived: " << num << endl; }
private:
    int num;
};

class Derived_prot : protected Base {
public:
    Derived_prot() : Base(), num(100) {}
    void dfunc() { cout << "Derived_prot: " << num << endl; }
private:
    int num;
};

int main() {

    Derived derived;
    cout << derived.pub << endl;
    Base b;
    cout << b.pub << endl;

}
