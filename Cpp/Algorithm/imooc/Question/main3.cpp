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


using namespace std;

//class Solution {
//private:
    //bool contain(BinaryTreeNode<int> *root, int target) {
        //if (!root)
            //return false;

        //if (root->val == target)
            //return true;
        //else if (root->val < target)
            //return contain(root->right, target);
        //else
            //return contain(root->left, target);
    //}
//public:
    //bool findTarget(BinaryTreeNode<int> *root, int k) {
        //if (!root)
            //return false;


    //}
//};
//

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        if (nums.empty())
            return false;

        // 使用滑动窗口和查找表
        // nums[l....r) 为滑动窗口
        int l = 0, r = 1;
        unordered_set<int> records;
        records.insert(nums[0]);
        while (r < nums.size()) {
            if (records.find(nums[r]) != records.end()) {
                if (r - l <= k)
                    return true;
                else {
                    records.
                }
            }
        }
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


    //string st1 = "anagram", st2 = "nagaram";
    //int digit = 19;

    string str = "abab", p = "ab";
    auto res = Solution().findAnagrams(str, p);
    //cout << res << endl;
    for (auto &d : res)
        cout << d << " ";
    cout << endl;
    //cout << std::boolalpha << res << endl;
}
