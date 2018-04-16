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
    bool isSubsequence(string s, string t) {
        if (s.empty())
            return true;
        int k = 0;
        for (int i = 0; i < t.size(); ++i)
            if (t[i] == s[k])
                k ++;
        if (k >= s.size())
            return true;
        return false;
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


    //int arr[] = {-1, -1, -1, -1, -1, 0};
    //auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
 
    //vector<vector<char>> nums =  {{'A','B','C','E'},{'S','F','E','S'},{'A','D','E','E'}};
    //string word = "ABCESEEEFS";
   //vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    //vector<vector<int>> nums = {{1, 2, 3}, {4, 5, 6}};
    vector<int> nums = {1, 2, 3, 4};
    string s = "aba";
    auto res = Solution().numberOfArithmeticSlices(nums);
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
    //printLinkedList(root);
    //cout << res << endl;

    //for (auto &d : res)
        //cout << d << " ";
    //cout << endl;
}
