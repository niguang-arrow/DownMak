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
    int longestConsecutive(vector<int>& nums) {
        if (nums.empty()) return 0;
        int res = 0, pre = 0, next = 0;
        unordered_set<int> record(nums.begin(), nums.end());
        for (auto &a : nums) {
            if (record.count(a)) {
                record.erase(a);
                pre = a - 1, next = a + 1;
                while (record.count(pre)) record.erase(pre--);
                while (record.count(next)) record.erase(next++);
                res = max(res, next - pre - 1);
            }
        }
        return res;
    }
};

int main() {
    TreeNode *root = new TreeNode(1);
    //root->left = new TreeNode(1);
    root->right = new TreeNode(2);
    //root->left->left = new TreeNode(1);
    root->right->left = new TreeNode(2);
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
    vector<int> nums1 = {100, 1, 2, 3, 4, 100};
    //vector<int> nums2 = {4, 3, 5, 1, 2};
    vector<int> preorder = {9, 3, 15, 20, 7};
    vector<int> inorder = {9, 15, 7, 20, 3};
    vector<vector<int>> nums = {{1,2,3,4}, {5,6,7,8}, {9,10,11,12}};
    string input = "thecattlewasrattledbythebattery";
    vector<string> words = {"a", "banana", "app", "appl", "ap", "apply", "apple"};
    auto res = Solution().longestConsecutive(nums1);
    cout << res << endl;
    //cout << std::boolalpha << res << endl;
    //preOrder(res);
    //cout << endl;

    //printVector(res);
    //printMatrix(res);
    //printLinkedList(res);
}
