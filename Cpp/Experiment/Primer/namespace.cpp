#include <iostream>
#include <vector>
#include <typeinfo> // 定义 dynamic_cast 和 typeid

namespace myspace {

    class Student {
        friend std::ostream& operator<<(std::ostream& os, const Student &s) {
            return os << s.name;
        }
        friend void print(std::ostream& os, const Student &s) {
            os << s.name << std::endl;
        }
    public:
        void print() { std::cout << name << std::endl; }
    private:
        std::string name = "Student";
    };

    void print(const Student &s) {
        std::cout << "myspace Student" << std::endl;
    }
}

class Base1 {

public:
    Base1() = default;
    Base1(const int &s) { count = s; }
    static int count;
};

int Base1::count = 100;

class Base2 {

public:
    Base2() = default;
    Base2(const int &s) { count = s; }
    static int count;
};

int Base2::count = 200;

class Derived : public Base1, public Base2 {
public:
    using Base1::Base1;
    using Base2::Base2;
    Derived(const int &s) { count = s; }
    Derived() = default;
    static int count;

};
int Derived::count = 10;

int main() {

    myspace::Student stu;
    stu.print();
    std::cout << stu << std::endl;
    print(std::cout, stu);

    print(stu);

    Derived b;
    Derived a(1000);
    std::cout << a.Base1::count << std::endl;
    std::cout << a.Base2::count << std::endl;
    std::cout << a.count << std::endl;

    // RTTI 测试
    const int w = 10;
    if (typeid(w) == typeid(int))
        std::cout << "int is int" << std::endl;

    Derived p, *dp = &p;
    Base1 *bp = dp; // 指向派生类对象
    if (typeid(*bp) == typeid(Derived))
        std::cout << "Derived" << std::endl;
    else if (typeid(*bp) == typeid(Base1))
        std::cout << "Base1" << std::endl;
    std::cout << typeid(*bp).name() << std::endl;

    return 0;
}

