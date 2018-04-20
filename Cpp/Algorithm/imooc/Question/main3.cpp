// BOBO : leetcode 
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include "function.h"
#include <queue>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <climits>


using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

 struct Point {
     int x;
     int y;
     Point() : x(0), y(0) {}
     Point(int a, int b) : x(a), y(b) {}
 };

void preOrder(TreeNode *root) {
    if (!root)
        return;

    cout << root->val << " ";
    preOrder(root->left);
    preOrder(root->right);
}

class Solution {
private:
    ListNode* merge(ListNode *head1, ListNode *head2) {
        ListNode *dummy = new ListNode(0);
        auto ptr = dummy;
        auto p1 = head1, p2 = head2;
        while (p1 && p2) {
            if (p1->val < p2->val) {
                ptr->next = p1;
                p1 = p1->next;
            }
            else {
                ptr->next = p2;
                p2 = p2->next;
            }
            ptr = ptr->next;
        }
        ptr->next = p1 ? p1 : p2;
        return dummy->next;
    }

public:
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next)
            return head;

        ListNode *dummy = new ListNode(0);
        dummy->next = head;
        auto slow = dummy, fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        auto rightHead = slow->next;
        slow->next = nullptr;
        auto leftHead = dummy->next;
        //printLinkedList(rightHead);
        //printLinkedList(leftHead);
        //return nullptr;
        leftHead = sortList(leftHead);
        rightHead = sortList(rightHead);
        auto newHead = merge(leftHead, rightHead);
        return newHead;
    }
};

int main() {
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(5);
    root->right->right = new TreeNode(6);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(4);
    //root->left->right->left = new TreeNode(7);
    //root->left->right->right = new TreeNode(9);
    //root->right->right = new TreeNode(17);

    //TreeNode *root = new TreeNode(0);
    //root->left = new TreeNode(1);
    //root->right = new TreeNode(1);


    int arr[] = {1, 2};
    auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
 
    //vector<vector<char>> nums =  {{'A','B','C','E'},{'S','F','E','S'},{'A','D','E','E'}};
    //string word = "ABCESEEEFS";
   //vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    //vector<vector<int>> nums = {{1, 2, 3}, {4, 5, 6}};
    //vector<int> nums = {4, 3, 2, 6};
    //string input = "owoztneoer";
    auto res = Solution().sortList(ls);
    //cout << res << endl;
    //cout << std::boolalpha << res << endl;
    //preOrder(root);
    //cout << endl;
    //cout << res << endl;

    //for (auto &d : res) {
        //for (auto & data : d)
            //cout << data << " ";
        //cout << endl;
    //}
    printLinkedList(res);
    //cout << res << endl;

    //for (auto &d : res)
        //cout << d << " ";
    //cout << endl;
}
