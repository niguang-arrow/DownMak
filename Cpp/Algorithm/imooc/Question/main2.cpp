// 左成云: 5. 用一个栈实现另一个栈的排序
#include <iostream>
#include <stack>

using namespace std;


void makeStackSort(stack<int> &sta) {
    stack<int> help;

    while (!sta.empty()) {
        int cur = sta.top();
        sta.pop();

        while (!help.empty() && help.top() < cur) {
            sta.push(help.top());
            help.pop();
        }
        help.push(cur);
    }

    while (!help.empty()) {
        sta.push(help.top());
        help.pop();
    }
}


int main() {
    stack<int> sta({3, 1, 6, 4});
    makeStackSort(sta);
    while (!sta.empty()) {
        cout << sta.top() << " ";
        sta.pop();
    }
    cout << endl;
}
