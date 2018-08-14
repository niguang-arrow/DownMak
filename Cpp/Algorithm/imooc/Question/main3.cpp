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
#include <iterator>
#include <list>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <utility>
#include <cmath>


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
    // 解答来自leetcode
    string longestPalindrome(string s) {
        int n = s.size();
        string res;
        for (int i = 0; i < n; ++i) {
            auto s1 = extend(s, i, i);
            auto s2 = extend(s, i, i + 1);
            cout << i << " " << s1 << " " << s2 << endl;
            res = s1.size() > res.size() ? s1 : res;
            res = s2.size() > res.size() ? s2 : res;
        }
        return res;
    }
private:
    string extend(const string &s, int l, int r) {
        cout << "before : l: " << l << " r: " << r << endl;
        while (l >= 0 && r < s.size() && s[l--] == s[r++]);
        cout << "after : l: " << l << " r: " << r << endl;
        return s.substr(l+2, (r - 2) - (l + 2));
    }
};



int main() {
    TreeNode *root = new TreeNode(5);
    root->left = new TreeNode(2);
    root->right = new TreeNode(-5);
    //root->left->left = new TreeNode(1);
    //root->right->left = new TreeNode(2);
    //root->right->right = new TreeNode(6);
    //root->left->right->left = new TreeNode(7);
    //root->left->right->right = new TreeNode(9);
    //root->right->right = new TreeNode(17);

    //int arr[] = {1, 2,3,  4, 5};
    //vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    //auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
 
    vector<vector<char>> board =  {{'A','B','C','E'},
                                   {'S','F','E','S'},
                                   {'A','D','E','E'}};
    string word = "ABCESEEEFS";
    //vector<vector<int>> nums = {{0, 0, 0}, {0, 1, 0}, {1, 1, 1}};
    //vector<int> nums1 = {1, 2, 2, 1};
    vector<int> nums1 = {1,1,2,2,3,5};
    //vector<int> nums2 = {4, 3, 5, 1, 2};
    vector<int> preorder = {9, 3, 15, 20, 7};
    vector<int> inorder = {9, 15, 7, 20, 3};
    vector<vector<int>> nums = {{1, 2}, {3}, {3}, {}};
    string input = "bbbab";
    vector<string> words = {"a", "banana", "app", "appl", "ap", "apply", "apple"};
    auto res = Solution().longestPalindrome("accb");
    cout << res << endl;
    //cout << std::boolalpha << res << endl;
    //preOrder(res);
    //cout << endl;

    //printVector(res);
    //printMatrix(res);
    //printLinkedList(res);
}
