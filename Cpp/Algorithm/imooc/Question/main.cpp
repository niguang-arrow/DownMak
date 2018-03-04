#include <iostream>
using namespace std;


long fib(int n) {

    if (n <= 1)
        return 1;

    long a = 1, b = 1;
    for (int i = 0; i != n; ++i) {
        long c = a + b;
        a = b;
        b = c;
    }
    return a;
}

long fib2(int n) {
    if (n <= 1)
        return 1;

    long f = 0, g = 1;
    // 通过 n 次加法和减法求出数列
    while (n--) {
        g = f + g;
        f = g - f;
    }
    return g;
}

int main() {

    for (int i = 0; i < 20; ++i)
        cout << fib2(i) << " ";
    cout << endl;

}
