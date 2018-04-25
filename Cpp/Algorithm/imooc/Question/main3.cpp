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


//class Solution {
//public:
    //void rotate(vector<int>& nums, int k) {
        //if (nums.empty())
            //return;

        //int len = nums.size();
        //vector<int> res(len);
        //for (int i = 0; i < len; ++i) {
            //int idx = (len - k % len + i) % len;
            //res[i] = nums[idx];
        //}
        //nums = res;
        //return;
    //}
//};
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        if (nums.empty() || (k % nums.size() == 0))
            return;

        rotate(nums, k - 1);
        int tmp = nums.back();
        for (int i = nums.size() - 2; i >= 0; --i)
            nums[i + 1] = nums[i];
        nums[0] = tmp;
        return;
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


    //int arr[] = {1, 2, 3};
    //auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
 
    //vector<vector<char>> nums =  {{'A','B','C','E'},{'S','F','E','S'},{'A','D','E','E'}};
    //string word = "ABCESEEEFS";
   //vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    //vector<vector<int>> nums = {{1, 2, 3}, {4, 5, 6}};
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7};
    //string input = "owoztneoer";
    Solution().rotate(nums, 3);
    //auto res = Solution().rotate(nums, 3);
    //cout << res << endl;
    //cout << std::boolalpha << res << endl;
    //preOrder(res);
    //cout << endl;
    //cout << res << endl;

    //for (auto &d : res) {
        //for (auto & data : d)
            //cout << data << " ";
        //cout << endl;
    //}
    //printLinkedList(res);
    //cout << res << endl;

    for (auto &d : nums)
        cout << d << " ";
    cout << endl;
}
