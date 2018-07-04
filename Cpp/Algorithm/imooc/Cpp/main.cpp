#include <iostream>
using namespace std;


int main() {
    double x = 0;
    double *p  = &x;
    cout << sizeof(p) << endl;
    cout << sizeof(x) << endl;
}
