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
    vector<int> constructArray(int n, int k) {
        vector<int> res(n, 0);
        if (k == 1) {
            for (int i = 0; i < n; ++i)
                res[i] = i + 1;
            return res;
        }
        if (k >= 2) {
            int count = 0, num = n;
            for (int i = 0; i < k; i += 2)
                res[i] = num --;
            for (int i = 1; i < k; i += 2)
                res[i] = ++count;
            for (int i = k; i < n; i ++)
                res[i] = ++count;
        }
        return res;
    }
};

//class Solution {
//private:
    //vector<int> memo;
    //int combinations(vector<int> &nums, int target) {
        //if (memo[target] != -1)
            //return memo[target];

        //int res = 0;
         ////这行注释的代码是错误的, 当不满足 target >= nums[i] 时, 会直接跳出
         ////循环, 这显然是不对的, 因为还要考虑 i 后面的情况
        //for (int i = 0; i < nums.size() && target >= nums[i]; ++i) {
        //for (int i = 0; i < nums.size(); ++i) {
            //if (target >= nums[i])
                //res += combinations(nums, target - nums[i]);
        //}
        //memo[target] = res;
        //return res;
    //}
//public:
    //int combinationSum4(vector<int>& nums, int target) {
        //memo = vector<int>(target + 1, -1);
        //memo[0] = 1;
        //return combinations(nums, target);
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

    //TreeNode *root = new TreeNode(0);
    //root->left = new TreeNode(1);
    //root->right = new TreeNode(1);


    int arr[] = {-1, -1, -1, -1, -1, 0};
    //int arr[] = {1};
    //auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
    //vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    //auto res = Solution().quickSort(nums);
    //vector<vector<int>> grid = {{1, 1, 1}, {0, 1, 0},
                                //{1, 1, 1}};
    //vector<vector<int>> grid = {{0}, {1}};
    //vector<vector<char>> grid = {{'X','X','X','X'},
                                 //{'X','O','O','X'},
                                 //{'X','O','O','X'},
                                 //{'X','O','X','X'}};
    //vector<vector<char>> grid = {{'O','O','O','O',
                                 //'O','O','O','O'}};
    //Solution().solve(grid);
    //vector<string> str = {"ab", "ac", "a", "c", "b"};
    //string str = "Flag";
    int N = 4;
    //vector<vector<int>> nums = {{83}, {64}, {2}};
    //vector<int> nums = {1, 3, 5, 4, 7};
    vector<int> nums = {1,3,6,7,9,4,10,5,6};
    string s = "babad";
    auto res = Solution().constructArray(3, 2);
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
    //printLinkedList(root);
    //cout << res << endl;

    for (auto &d : res)
        cout << d << " ";
    cout << endl;
}
