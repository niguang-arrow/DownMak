// 左成云: 2. 由两个栈组成的队列
#include <iostream>
#include <stack>
#include <initializer_list>

using namespace std;

template <typename T>
class Queue {
private:
    stack<T> stack1;
    stack<T> stack2;
public:
    Queue() {}
    ~Queue() {}
    Queue(initializer_list<T> ls) {
        for (const auto &d : ls)
            push(d);
    }

    void push(T data) {
        stack1.push(data);
    }

    void pop() {
        // 这里先判断 stack2 是否为空, 如果为空, 那么只需要将 stack1 中的
        // 元素入栈 stack2. 但也有可以 stack1 也是为空的, 此时直接执行
        // stack2.pop(), 异常检测由标准库去做.
        if (stack2.empty()) {
            while (!stack1.empty()) {
                stack2.push(stack1.top());
                stack1.pop();
            }
        }
        stack2.pop();
    }

    // 这里 top 不要实现为 const 的, 要修改 stack1 和 stack2
    T top() {
        if (stack2.empty()) {
            while (!stack1.empty()) {
                stack2.push(stack1.top());
                stack1.pop();
            }
        }

        return stack2.top();
    }

    bool empty() const {
        return (stack1.empty() && stack2.empty());
    }

    int size() const {
        return (stack1.size() + stack2.size());
    }
};


int main() {
    Queue<int> myque = {1, 2, 3, 4, 5};

    while (!myque.empty()) { 
        cout << myque.top() << " ";
        myque.pop();
    }
    cout << endl;
}
