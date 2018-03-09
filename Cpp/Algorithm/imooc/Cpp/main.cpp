#include <iostream>
using namespace std;
void change();

int global = 0;

int main() {

    cout << global << endl;
    
    change();

    cout << global << endl;

}
