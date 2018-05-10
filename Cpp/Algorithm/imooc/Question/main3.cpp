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
    vector<int> nextGreaterElement(vector<int>& findNums, vector<int>& nums) {
        vector<int> res(findNums.size(), -1);
        stack<int> s;
        unordered_map<int, int> record; // nums[i] -> index of nums[nextGreaterElement]
        for (int i = 0; i < nums.size(); ++i) {
            while (!s.empty() && nums[i] > nums[s.top()]) {
                record[nums[s.top()]] = i;
                s.pop();
            }
            s.push(i);
        }
        for (int i = 0; i < findNums.size(); ++i)
            if (record.count(findNums[i]))
                res[i] = record[findNums[i]];

        return res;
    }
};

//class Solution {
//public:
    //int kthSmallest(vector<vector<int>>& matrix, int k) {
    //}
//};

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
    //vector<vector<int>> nums = {{0, 0, 0}, {0, 1, 0}, {1, 1, 1}};
    vector<int> nums1 = {2, 4};
    vector<int> nums = {1, 2, 3, 4};
    //string input = "owoztneoer";
    auto res = Solution().nextGreaterElement(nums1, nums);
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

    for (auto &d : res)
        cout << d << " ";
    cout << endl;
}
