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
public:
    int numComponents(ListNode* head, vector<int>& G) {
        if (!head)
            return 0;
        if ((!head->next && head->val == G.back()))
            return 1;
        unordered_map<int, int> record;
        auto ptr = head;
        while (ptr && ptr->next) {
            record[ptr->val] = ptr->next->val;
            ptr = ptr->next;
        }

        ptr = head;
        while (ptr) {
            if (record.find(ptr->val) != record.end()) {

            }
        }

        //for (auto &iter : record) {
            //cout << iter.first << " -> " << iter.second << endl;
        //}

        //unordered_set<int> iset(G.begin(), G.end());
        //int res = 0;
        //for (auto val : iset) {
            //cout << "val: " << val << endl;
            //while (record.find(val) != record.end()) {
                //iset.erase(iset.find(val));
                //val = record[val];
            //}
            //res ++;
        //}
        //return res;
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

    //TreeNode *root2 = new TreeNode(0);
    //root2->left = new TreeNode(1);
    //root2->right = new TreeNode(1);


    int arr[] = {1, 2, 3, 4, 5};
    auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
 
    //vector<vector<char>> nums =  {{'A','B','C','E'},{'S','F','E','S'},{'A','D','E','E'}};
    //string word = "ABCESEEEFS";
   //vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    //vector<vector<int>> nums = {{1, 2, 3}, {4, 5, 6}};
    vector<int> nums = {4, 5};
    //string input = "owoztneoer";
    auto res = Solution().numComponents(ls, nums);
    cout << res << endl;
    //cout << std::boolalpha << res << endl;
    //preOrder(root);
    //cout << endl;
    //cout << res << endl;

    //for (auto &d : res) {
        //for (auto & data : d)
            //cout << data << " ";
        //cout << endl;
    //}
    //printLinkedList(res);
    //cout << res << endl;

    //for (auto &d : res)
        //cout << d << " ";
    //cout << endl;
}
