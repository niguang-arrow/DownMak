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

int main(){
    A a1;
    A a2(a1);
}
