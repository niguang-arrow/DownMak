/*
 *Refer:  http://www.techiedelight.com/stack-implementation-in-cpp/
 */

#include <iostream>
#include <cassert>

using namespace std;


template <typename Item>
class Stack {
private:
    Item *data;
    int count;
    int capacity;
public:
    // 注意, 为了让 pop 等操作中直接使用 count-- 等简洁的写法, 这里设置 count 的初值
    // 为 -1, 所以 stack 的实际大小应该是 count + 1; count 真实的意义应该是元素的索引,
    Stack(int capa) : data(new Item[capa]), count(-1), capacity(capa) {}
    ~Stack() { delete[] data; }

    int size() const { return count + 1; }
    bool isEmpty() const { return count == -1; }

    void push(Item item) {
        assert( capacity >= count + 2 );
        data[++count] = item;
    }

    void pop() {
        assert(count >= 0);
        count--;
    }

    Item top() const {
        assert(count >= 0);
        return data[count];
    }

    void print() const {
        for (int i = 0; i < size(); ++i)
            cout << data[i] << " ";
        cout << endl;
    }

};


int main() {
    Stack<int> sta = Stack<int>(100);
    for (int i = 0; i < 15; ++i)
        sta.push(rand() % 100);
    sta.print();

    while (!sta.isEmpty()) {
        cout << sta.top() << " ";
        sta.pop();
    }
    cout << endl;
}
