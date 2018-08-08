#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <map>
#include <unordered_map>

using namespace std;


int add(int a, int b) {
    return a + b;
}

static int (*pa) (int, int) = add;

class Add {
public:
    int operator()(int a, int b) {
        return a + b;
    }
};

using T = int(int, int);

static function<int(int, int)> add_a = [] (int a, int b) { return a + b; };



int main() {
    vector<function<T>> record{add, Add(), add_a, pa};

    for (auto &func : record)
        cout << func(1, 10) << endl;
}
