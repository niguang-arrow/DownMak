#include <iostream>
#include <list>
#include <algorithm>


using namespace std;


int main() {

    list<int> ls;
    for (int i = 0; i < 5; ++i)
        ls.push_back(i);

    ls.push_back(0);
    auto a = find(ls.begin(), ls.end(), 0);
    cout << *a << endl;
    cout << &(ls.front()) << endl;
    cout << &(ls.back()) << endl;
    cout << sizeof(int) << endl;
}
