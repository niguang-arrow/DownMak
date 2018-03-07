# Data Structure

2018 年 3 月 7 日

[最大堆 优先队列](#maxheap)

[栈](#栈)



## maxheap

最大堆可以用来实现优先队列这种数据结构.

最大堆的特点是: 它首先是一棵完全二叉树, 这意味着它可以使用数组来实现; 它的每个节点的值总是不大于其父节点的值.

对于堆来说, 有一种经典的实现是从索引 1 开始标记根节点.

这样, 有如下关系:

```bash
parent(i) = i / 2
left child(i) = 2 * i
right child(i) = 2 * i + 1
```

往堆中插入元素: shiftUp, 在堆的末尾插入节点之后, 要对该节点进行shiftUp 操作.

从堆中取出元素: shift Down, 首先将根节点和堆中最后一个节点交换, 然后再将根节点进行 shiftDown.

实现代码:

```cpp
#include <iostream>
#include <cassert>

using namespace std;

template <typename Item>
class MaxHeap {
private:
    Item *data;
    int count; // 堆中当前的节点个数
    int capacity; // 堆的容量
    
    // shiftUP(k), 将索引为k的位置的元素向上移动, 以满足堆的定义
    // 如果大于父节点, 那么就需要和父节点进行交换
    void shiftUp(int k) {
        // k 处元素的父节点为 k/2 处的节点, 但 k 要满足 k > 1
        while (k >= 2 && data[k] > data[k/2]) {
            swap(data[k], data[k/2]);
            k /= 2;
        }
    }

    // 将 k 处的节点shiftDown, 首先要保证它有孩子, 在堆中不可能只有右孩子
    // 而没有左孩子, 因此, 只要有 2*k 这个左孩子的话, 循环继续
    void shiftDown(int k) {
        while (2 * k <= count) {
            int j = 2 * k; // 在此轮循环中, data[k] 和 data[j] 交换位置
            if ((j + 1) <= count && data[j + 1] > data[j])
                j += 1; // 如果有右孩子并且右孩子还比较大, 那么就要和右孩子交换

            if (data[k] >= data[j]) // 如果比孩子还大, 就不用 shiftDown
                break;

            swap(data[k], data[j]);
            k = j;
        }
    }

public:
    MaxHeap(int capacity) {
        // 因为堆从索引1开始记录根节点, 所以data 的总大小要加1.
        data = new Item[capacity + 1];
        count = 0; // 堆中当前元素的个数
        this->capacity = capacity;
    }
    ~MaxHeap() { delete[] data; }

    int size() const {
        return count;
    }
    
    bool isEmpty() const {
        return count == 0;
    }

    void insert(Item item) {
        assert(capacity >= count + 1);// 要保证堆的容量足够, 至少还能插入一个新元素 
        data[count + 1] = item; //根从索引1开始, 所以要加1
        count++;
        shiftUp(count);
    }

    void print() const {
        for (int i = 1; i <= count; ++i)
            cout << data[i] << " ";
        cout << endl;
    }

    // 从堆中取出元素, 首先要保证堆中确实有元素, 然后将其和最后一个节点交换,
    // 并减小 count 的值(这就相当于删除了该节点), 然后再将根节点 shiftDown
    Item extractMax() {
        assert(count > 0);

        Item ret = data[1];
        swap(data[1], data[count]);
        count--;
        shiftDown(1);
        return ret;
    }
};


int main() {
    MaxHeap<int> maxheap = MaxHeap<int>(100);
    for (int i = 0; i < 15; ++i)
        maxheap.insert(rand() % 100);
    maxheap.print();

    // 将堆中的数据从大到小输出, 注意, 由于使用 extractMax() 会使堆的大小发生变化,
    // 所以应该使用 while 将堆输出而不能使用 i < maxheap.size() 的 for 循环
    while (!maxheap.isEmpty())
        cout << maxheap.extractMax() << " ";
    cout << endl;
}
```



## 栈

栈的实现非常简单, 它是一种先入后出的结构. 下面的实现中, 只需要实现 push, pop, top 等方法即可.

```cpp
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
```

