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
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        if (nums.size() < 4)
            return {};

        std::sort(nums.begin(), nums.end());
        vector<vector<int>> res;
        vector<int> path;
        for (int i = 0; i < nums.size() - 3; ++i) {
            if (i == 0 || (i > 0 && nums[i] != nums[i - 1])) {
                for (int j = i + 1; j < nums.size() - 2; ++j) {
                    if (j == i + 1 || (j > i + 1 && nums[j] != nums[j - 1])) {
                        int lo = j + 1, hi = nums.size() - 1;
                        while (lo < hi) {
                            int sum = nums[i] + nums[j] + nums[lo] + nums[hi];
                            cout << sum << endl;
                            if (sum == target) {
                                path = {nums[i], nums[j], nums[lo], nums[hi]};
                                res.push_back(path);
                                while (lo < hi && nums[lo] == nums[lo + 1]) lo ++;
                                while (lo < hi && nums[hi] == nums[hi - 1]) hi --;
                                lo ++;
                                hi --;
                            }
                            else if (sum < target)
                                lo ++;
                            else
                                hi --;
                        }
                    }
                }
            }
        }
        return res;
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


    //int arr[] = {1, 2, 2, 2, 2, 3, 3, 4, 5};
    int arr[] = {1, 0, -1, 0, 2};
    //auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
    vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    auto res = Solution().fourSum(nums, 0);
    //cout << res << endl;
    for (auto &d : res) {
        for (auto & data : d)
            cout << data << " ";
        cout << endl;
    }
    //printLinkedList(res);
    //cout << res << endl;

    //for (auto &d : res)
        //cout << d << " ";
    //cout << endl;
    //cout << std::boolalpha << res << endl;

}
