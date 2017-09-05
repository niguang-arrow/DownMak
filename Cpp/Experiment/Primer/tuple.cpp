#include <iostream>
#include <tuple>
#include <random>
#include <cmath>

/*
 * 操纵符可以修改流的状态, 一个操纵符是一个函数或是一个对象, 会影响流的状态, 并能
 * 用作输入或输出运算符的运算对象. 操纵符也返回它所处理的流对象.
 * 
 */

using namespace std;

static default_random_engine e;
static uniform_real_distribution<double> u(0, 1); // 生成随机实数
static normal_distribution<> n(4, 1.5); //使用默认模板参数


int main() {

    // 关于 tuple
    tuple<int, string, string> a(42, "Hello", "world");
    cout << get<2>(a) << endl;
    cout << tuple_size<decltype(a)>::value << endl;
    tuple_element<1, decltype(a)>::type b("helloooo");
    cout << b << endl;

    // 生成随机实数
    for (size_t i = 0; i != 5; ++i)
        cout << u(e) << " ";
    cout << endl;
    cout << u.max() << " " << u.min() << endl;

    // 生成非均匀分布的随机数
    vector<unsigned> vals(9);
    for (size_t i = 0; i != 200; ++i) {
        unsigned v = lround(n(e));
        if (v < vals.size())
            ++vals[v];
    }
    for (size_t j = 0; j != vals.size(); ++j)
        cout << j << ": " << string(vals[j], '*') << endl;

    // 使用操纵符, 控制 bool 值输出的格式状态
    cout << "default bool values: " << true << " " << false
        << "\nalpha bool values: " << boolalpha
        << true << " " << false << endl;
    // 控制整型值的进制
    cout << showbase; // 打印整型数时显示进制
    cout << "default: " << 20 << endl;
    cout << "octal: " << oct << 20 << endl;
    cout << "hex: " << hex << 20 << endl;
    cout << "decimal: " << dec << 20 << endl;
    cout << noshowbase; //  恢复正常状态

    return 0;
}
