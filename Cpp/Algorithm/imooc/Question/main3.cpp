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
    vector<int> singleNumber(vector<int>& nums) {
        int num = 0;
        // num 中保存两个 single number 的异或值, 并且 num 肯定不为 0,
        // 那么找到num中位为 1 的位, 假设为 i, 那么根据第 i 位是否为 1,
        // 可以将 nums 中的数分为两类.
        for (auto &d : nums)
            num ^= d;
        unsigned i = 0;
        while (((num >> i) & 1) != 1) i ++;
        int one = 0, two = 0;
        for (auto &d : nums) {
            if (isBit_1(d, i)) one ^= d;
            else two ^= d;
        }
        return {one, two};
    }
private:
    bool isBit_1(int num, unsigned i) {
        while (i) num >>= i--;
        return (num & 1);
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
    vector<int> nums1 = {1,1,2,2,3,5};
    //vector<int> nums2 = {4, 3, 5, 1, 2};
    vector<int> preorder = {9, 3, 15, 20, 7};
    vector<int> inorder = {9, 15, 7, 20, 3};
    vector<vector<int>> nums = {{1, 2}, {3}, {3}, {}};
    string input = "bbbab";
    vector<string> words = {"a", "banana", "app", "appl", "ap", "apply", "apple"};
    auto res = Solution().singleNumber(nums1);
    //cout << res << endl;
    //cout << std::boolalpha << res << endl;
    //preOrder(res);
    //cout << endl;

    printVector(res);
    //printMatrix(res);
    //printLinkedList(res);
}
