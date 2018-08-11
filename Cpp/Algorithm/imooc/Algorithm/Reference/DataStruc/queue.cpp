#include <iostream>
#include <cassert>

using namespace std;

// 使用数组实现循环队列
// 最需要注意的地方:
// 1. 是如何判断当前队列是否为空,已满以及队列的大小
// 2. 而是下面队列的容量虽然设置为 N, 但实际上最多存储 N - 1 个数.
// 因为 isFull 的代码是 (rear + 1) % N == head; 那么当 head = 0,
// rear = N - 1 时, 此时就满足了 isFull 的条件, 但是 rear 这个位置
// 是没有插入元素的... 这说明要空出一个位置了.
template <typename T>
class Queue {
private:
    T *data;
    int capacity;
    int head;
    int rear;

public:
    Queue(int cap = 10) : data(new T[cap]), capacity(cap), head(0), rear(0) {}
    ~Queue() { if (data) delete[] data; }
    bool isEmpty() const { return head == rear; }
    bool isFull() const { return (((rear + 1) % capacity) == head); }
    int size() const { return ((rear - head + capacity) % capacity); }

    void push(T item) {
        assert(!isFull());
        data[rear] = item;
        rear = (rear + 1) % capacity;
    }

    T pop() {
        assert(!isEmpty());
        T d = data[head];
        head = (head + 1) % capacity;
        return d;
    }

    T front() const {
        assert(!isEmpty());
        return data[head];
    }
};


int main() {
    Queue<int> queue = Queue<int>(10);
    for (int i = 0; i < 10; ++i) {
        queue.push(i);
        if (queue.isFull()) {
            cout << queue.size() << endl;
            break;
        }
    }

    while (!queue.isEmpty()) {
        cout << queue.front() << " ";
        queue.pop();
    }
    cout << endl;

}
