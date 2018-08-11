#include <iostream>
#include <cassert>

using namespace std;

template <typename Item>
class MaxHeap {
private:
    Item *data;
    int count;
    int capacity; // 堆的容量
    
    // shiftUP(k), 将索引为k的位置的元素向上移动, 以满足堆的定义
    // 如果大于父节点, 那么就需要和父节点进行交换
    void shiftUp(int k) {
        // k 处的元素的父节点为 k/2 处的节点, 但 k 要满足k > 1
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
