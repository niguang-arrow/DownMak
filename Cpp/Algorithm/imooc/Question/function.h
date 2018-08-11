#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <stack>
#include <random>
#include <ctime>
#include <vector>
#include <queue>
#include <stack>
#include <initializer_list>
using namespace std;

// 二分搜索树
template <typename T>
struct BinaryTreeNode {
    T val;
    BinaryTreeNode *left;
    BinaryTreeNode *right;
    BinaryTreeNode(T v) : val(v), left(nullptr), right(nullptr) {}
};

template <typename T>
class BSTree {
private:
    BinaryTreeNode<T>* insert(BinaryTreeNode<T> *node, T val) {
        if (node == nullptr) {
            count++;
            return new BinaryTreeNode<T>(val);
        }

        if (val == node->val)
            node->val = val;
        else if (val < node->val)
            node->left = insert(node->left, val);
        else
            node->right = insert(node->right, val);

        return node;
    }

    void preOrder(BinaryTreeNode<T> *node) const {
        if (node) {
            cout << node->val << " ";
            preOrder(node->left);
            preOrder(node->right);
        }
    }

    void inOrder(BinaryTreeNode<T> *node ) const {
        if (node) {
            inOrder(node->left);
            cout << node->val << " ";
            inOrder(node->right);
        }
    }

    void postOrder(BinaryTreeNode<T> *node ) const {
        if (node) {
            postOrder(node->left);
            postOrder(node->right);
            cout << node->val << " ";
        }
    }

    void clear(BinaryTreeNode<T> *node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
            count--;
        }
    }

    void reverse(BinaryTreeNode<T> *node) {
        if (!node || (!node->left && !node->right))
            return;
        BinaryTreeNode<T> *temp = node->left;
        node->left = node->right;
        node->right = temp;
        reverse(node->left);
        reverse(node->right);
    }

public:
    // 将根节点放在 public, 为了以后写代码方便
    BinaryTreeNode<T> *root;
    int count;
    BSTree() : root(nullptr), count(0) {}
    BSTree(initializer_list<T> ls) : root(nullptr), count(0) {
        for (const auto &d : ls)
            insert(d);
    }
    ~BSTree() {
        clear(root);
    }

    int size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }
    void insert(T val) {
        root = insert(root, val);
    }

    void preOrder() const {
        preOrder(root);
        cout << endl;
    }
     
    // 广度优先遍历
    void BreadthFirst() {
        queue<BinaryTreeNode<T>*> que;
        if (root) {
            que.push(root);
            while (!que.empty()) {
                BinaryTreeNode<T> *node = que.front();
                que.pop();
                if (node) {
                    cout << node->val << " ";
                    que.push(node->left);
                    que.push(node->right);
                }
            }
            cout << endl;
        }
    }


    void inOrder() const {
        inOrder(root);
        cout << endl;
    }

    void postOrder() const {
        postOrder(root);
        cout << endl;
    }

    void reverse() {
        reverse(root);
    }
};


// 链表相关
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x = 0) : val(x), next(nullptr) {}
};

// 用于链表的构建, 输出与删除
ListNode* createLinkedList(int arr[], int n) {
    if (n <= 0)
        return nullptr;

    ListNode *head = new ListNode(arr[0]);

    ListNode *curNode = head;

    for (int i = 1; i < n; ++i) {
        curNode->next = new ListNode(arr[i]);
        curNode = curNode->next;
    }

    return head;
}


void deleteLinkedList(ListNode *head) {
    ListNode *curNode = head;
    while (curNode) {
        ListNode *delNode = curNode;
        curNode = curNode->next;
        delete delNode;
    }
}

ListNode* addToLinkedListTail(ListNode *head, int d) {
    ListNode *dummy = new ListNode(0);
    dummy->next = head;

    ListNode *cur = dummy;
    while (cur->next != nullptr) {
        cur = cur->next;
    }
    ListNode *pnew = new ListNode(d);
    cur->next = pnew;

    head = dummy->next;
    delete dummy;
    return head;
}

void printLinkedList(ListNode *head) {
    ListNode *curNode = head;
    while (curNode) {
        cout << curNode->val << " -> ";
        curNode = curNode->next;
    }
    cout << "NULL " << endl;
    return;
}

void reversePrint(ListNode *head) {

    ListNode *cur = head;
    stack<int> sta;
    while (cur) {
       sta.push(cur->val) ;
       cur = cur->next;
    }

    while (!sta.empty()) {
        cout << sta.top() << " -> ";
        sta.pop();
    }
    cout << "NULL" << endl;
}

void recursivePrint(ListNode *head) {
    if (head == nullptr)
        return;
    recursivePrint(head->next);
    cout << head->val << " -> ";
}

ListNode* removeNodeFromLinkedList(ListNode *head, int value) {
    ListNode *dummy = new ListNode(0);
    dummy->next = head;

    ListNode *cur = dummy;
    while (cur->next) {
        if (cur->next->val == value) {
            ListNode *delNode = cur->next;
            cur->next = delNode->next;
            delete delNode;
            break;
        }
        else {
            cur = cur->next;
        }
    }
    head = dummy->next;
    delete dummy;
    return head;
}


int* generateArray(int n) {
    static default_random_engine e(1234);
    static uniform_int_distribution<int> uni(1, 10);

    int *arr = new int[n];
    for (int i = 0; i < n; ++i)
        *(arr + i) = uni(e);
    return arr;
}

template <typename T>
void printMatrix(const vector<vector<T>> &matrix) {
    for (const auto &d : matrix) {
        for (const auto &data : d)
            cout << data << " ";
        cout << endl;
    }
}

template <typename T>
void printVector(const vector<T> &vec) {
    for (const auto &d : vec)
        cout << d << " ";
    cout << endl;
}

#endif
