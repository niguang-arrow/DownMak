#include <iostream>

using namespace std;


class Base {
public:
    Base() : num(0) {}
    void bfunc() { cout << "Base: " << num << endl; }
private:
    int num;
};

class Derived : protected Base {
public:
    Derived() : Base(), num(10) {}
    void dfunc() { cout << "Derived: " << num << endl; }
private:
    int num;
};

int main() {

    Derived derived;
    derived.dfunc();
    Base *p = &derived;

}
