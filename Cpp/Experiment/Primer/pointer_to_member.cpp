#include <iostream>


/*
 * 在* 前添加 classname:: 以表示当前定义的指针可以指向 classname 的成员 
 */

using namespace std;

class Screen {
    friend void show(const Screen&);
public:
    typedef string::size_type pos;
    Screen() = default;
    Screen(const string &s) : contents(s) {}
    char get_cursor() const { return contents[cursor]; }
    // 定义静态成员函数 pm, 返回成员指针. 返回类型
    // 为 const string Screen::*
    static const string Screen::*pm() {
        return &Screen::contents;
    }

private:
    string contents;
    pos cursor;
};

void show(const Screen &s) {
    // 使用类数据成员指针, 类型也可以使用 auto 等进行推断
    // 注意在我们初始化一个成员指针或为成员指针赋值时, 该指针并没有
    // 指向任何数据.
    const string Screen::*pdata = &Screen::contents; // contents 为 private, 要用友元
    cout << s.*pdata << endl;
}

int main() {
    
    Screen data("myScreen");
    show(data);
    // 通过这种方式, 竟然可以在类外访问一个 private 的成员.
    const string Screen::*pdata = Screen::pm();
    cout << data.*pdata << endl;

    return 0;
}
