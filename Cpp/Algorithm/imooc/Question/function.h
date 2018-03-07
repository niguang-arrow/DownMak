#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <stack>
#include <random>
#include <ctime>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
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
#endif
