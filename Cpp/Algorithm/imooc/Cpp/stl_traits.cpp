/* 3.3 迭代器相应的型别 (associated types)
 * 要通过迭代器知道其所指对象的相应的型别,  
 * 
 */ 


#include <iostream>
#include <list>
#include <initializer_list>

using namespace std;

template <typename T>
struct ListNode {
    T data;
    ListNode* next; 
    ListNode() : data(T()), next(nullptr) {}
    ListNode(const T &elem) : data(elem), next(nullptr) {}
    ListNode(const ListNode &ln) : data(ln.data), next(ln.next) {}

    friend ostream& operator<<(ostream &os, const ListNode &node) {
        return os << node.data;
    }
};

template <typename T>
class ListIter {
    typedef ListIter<T> iterator;
    typedef ListNode<T> list_node;
    typedef T value_type;
    typedef T& reference_type;
    typedef T* pointer;

public:
    ListIter(list_node *head) : node(head) {}
    ListIter(const ListIter &iter) : node(iter.node) {}

    list_node *node;
    reference_type operator*() const { return (*node).data; }
    pointer operator->() const { return &(operator*()); } 
    bool operator==(const ListIter &x) const { return node == x.node; }
    bool operator!=(const ListIter &x) const { return node != x.node; }
    iterator& operator++() {
        //node = (list_node*)((*node).next);
        node = (list_node*) node->next;
        return *this;
    }
    iterator operator++(int) {
        iterator tmp = *this;
        ++(*this);
        return tmp;
    }
};

template <typename T>
class List {
    typedef ListNode<T> list_node;
public:
    typedef ListIter<T> iterator;

    friend ostream& operator<<(ostream &os, const List &L) {
        list_node *ptr = L.head->next;
        while (ptr != L.cur->next) {
            os << *ptr << " -> ";
            ptr = ptr->next;
        }
        os << "NULL";
        return os;
    }
public:
    List() : head(new list_node()), cur(head) {}
    List(const T &data) : head(new list_node()), cur(head) {
        insert(data);
    }
    List(const initializer_list<T> &ls) : head(new list_node()), cur(head) {
        for (auto &data : ls)
            insert(data);
    }

    void insert(const T &data) {
        cur->next = new list_node(data);
        cur = cur->next;
    }

    iterator begin() const { return (list_node*)head->next; }
    iterator end() const { return (list_node*)cur->next; }
private:
    list_node *head;
    list_node *cur;
};

class Data {
private:
    int data;
public:
    friend ostream& operator<<(ostream &os, const Data &rhs) {
        return os << "Data: " << rhs.data;
    }
    Data(int i = 0) : data(i) {}
};

int main() {
    List<Data> mylist{1, 2, 3, 4};
    cout << mylist << endl;
    List<Data>::iterator iter = mylist.begin();
    while (iter != mylist.end()) {
        cout << *iter++ << endl;
    }
    //for (int i = 1; i < 60; i *= 2)
        //ls.push_back(Data(i));
    //cout << ls.back() << endl;
    //list<Data>::iterator iter = ls.begin();
}
