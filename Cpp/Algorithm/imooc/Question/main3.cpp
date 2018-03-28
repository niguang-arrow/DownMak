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

class Solution {
public:
    int maxProduct(vector<int>& nums) {
        // 设 dp[n] 表示以 n 结尾的序列中子序列最大乘积的值.
        if (nums.empty())
            return 0;

        vector<int> dp(nums.size(), nums[0]);
        for (int i = 1; i < nums.size(); ++i) {
            int imin = min(nums[i - 1], dp[i - 1]);
            int imax = max(nums[i - 1], dp[i - 1]);

            dp[i] = max(max(imin * nums[i], imax * nums[i]), nums[i]);
        }
        return dp[nums.size() - 1];
    }
};

int main() {
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    //root->left->right->left = new TreeNode(7);
    //root->left->right->right = new TreeNode(9);
    //root->right->right = new TreeNode(17);

    //TreeNode *root = new TreeNode(0);
    //root->left = new TreeNode(1);
    //root->right = new TreeNode(1);


    int arr[] = {2, 3, -2, 4};
    //int arr[] = {1, 0, -1, 0, 2, -2};
    //int arr[] = {1};
    //auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
    vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
   auto res = Solution().maxProduct(nums);
    cout << res << endl;
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
    //cout << std::boolalpha << res << endl;

}
