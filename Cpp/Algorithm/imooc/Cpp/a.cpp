#include <iostream>

using namespace std;


void change() {
    extern int global;
    global = 100;
    cout << "in change() " << ":" << " " << global << endl;
}
