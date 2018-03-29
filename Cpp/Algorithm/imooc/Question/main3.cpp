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

class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        if (cost.empty())
            return 0;

        int n = cost.size();
        vector<int> memo(n, INT32_MAX);
        memo[0] = cost[0];

        for (int i = 1; i < n; ++i)
            memo[i] = cost[i] + min(memo[i - 1], i - 2 >= 0 ? memo[i - 2] : 0);

        return min(memo[n - 1], memo[n - 2]);
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


    int arr[] = { 1, 100, 1, 1, 1, 100, 1, 1, 100, 1};
    //int arr[] = {1};
    //auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
    vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    //auto res = Solution().quickSort(nums);
    auto res = Solution().minCostClimbingStairs(nums);
    cout << res << endl;
    //for (auto &d : res) {
        //for (auto & data : d)
            //cout << data << " ";
        //cout << endl;
    //}
    //printLinkedList(res);
    //cout << res << endl;

    //for (auto &d : nums)
        //cout << d << " ";
    //cout << endl;
    //cout << std::boolalpha << res << endl;

}
