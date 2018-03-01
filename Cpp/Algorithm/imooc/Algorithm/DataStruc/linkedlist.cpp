#include <iostream>

using namespace std;


template <typename T>
class List {
public:
    List() : head(new Node(0)) {}
    ~List() { clear(); }
    void insert(const T &d); // 从头部插入一个新的节点
    void insert(const int &pos, const T &d); // 向指定位置插入节点
    void erase(const T &d);   // 删除指定数据的节点
    void update(const int &pos, const T &d); // 更新指定的数据
    void reverse();  // 反转链表
    void print(); // 打印链表

private:
    struct Node {
        T data;
        Node *next;

        Node(const T &d) : data(d), next(nullptr) {}
    };

    Node* head;

    // 用于删除链表
    void clear() {

    }

    // 寻找数据为 d 的节点的上一个节点的位置
    // 要这样想, 第一个数据节点的上一个节点是 head, 所以初始化 p 为 head
    // 最后就能返回 p 了.
    Node* find(const T &d) {
        Node *p = head;
        while (p) {
            if (p->next->data == d)
                break;
            p = p->next;
        }
        return p;
    }
};

template <typename T>
void List<T>::print() {
    Node *p = head;
    while (p) {
        cout << p->next->data << " ";
        p = p->next;
    }
}

template <typename T>
void List<T>::insert(const T &d) {
    Node *p = new Node(d);
    p->next = head->next;
    head->next = p;
}

template <typename T>
void List<T>::insert(const int &pos, const T &d) {
    Node *p = new Node(d);
}
