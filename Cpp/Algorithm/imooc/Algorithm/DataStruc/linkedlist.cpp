#include <iostream>
#include <initializer_list>

using namespace std;


template <typename T>
class List {
public:
    List() : head(new Node(0)), count(0) {}
    List(const initializer_list<T> &ls) : head(new Node(0)), count(0) {
        for (const auto &d : ls) {
            this->insert(d);
        }
    }
    
    ~List() { clear(); }
    void insert(const T &d); // 从头部插入一个新的节点
    void insert(const T &pos, const T &d); // 向指定位置插入节点
    void erase(const T &d);   // 删除指定数据的节点
    void update(const T &pos, const T &d); // 更新指定的数据
    void reverse();  // 反转链表
    void print(); // 打印链表
    int size() const {
        return count;
    }
    bool isempty() const {
        return count == 0;
    }

private:
    struct Node {
        T data;
        Node *next;

        Node(const T &d) : data(d), next(nullptr) {}
    };

    Node* head;
    int count;

    // 用于删除链表
    void clear() {
        Node *p = head;
        while (p) {
            Node *q = p->next;
            delete p;
            p = q;
        }
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
    while (p->next) {
        cout << p->next->data << " ";
        p = p->next;
    }
    cout << endl;
}

template <typename T>
void List<T>::insert(const T &d) {
    Node *p = new Node(d);
    p->next = head->next;
    head->next = p;
    count++;
}

// 在 pos 之前的位置插入 d
template <typename T>
void List<T>::insert(const T &pos, const T &d) {
    Node *pre = find(pos);
    Node *p = new Node(d);
    p->next = pre->next;
    pre->next = p;
}


template <typename T>
void List<T>::erase(const T &d) {
    Node *pre = find(d);
    Node *q = pre->next;
    pre->next = pre->next->next;
    delete q;
}

template <typename T>
void List<T>::update(const T &pos, const T &d) {
    Node *pre = find(pos);
    pre->next->data = d;
}


template <typename T>
void List<T>::reverse() {
    if (!head->next || !head->next->next)
        return;
    Node *p = head->next;
    Node *q = head->next->next;
    Node *m = head->next->next->next;
    p->next = nullptr;
    while (m) {
        q->next = p;
        p = q;
        q = m;
        m = m->next;
    }
    q->next = p;
    head->next = q;
}


int main() {
    List<int> ls({ 2});
    cout << ls.size() << endl;
    ls.print();
    ls.reverse();
    ls.print();

    return 0;
}
