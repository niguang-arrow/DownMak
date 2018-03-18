// BOBO : leetcode 
#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include "function.h"
#include <queue>
#include <string>
#include <map>


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
private:
    int Area(int i, int j, int ai, int aj) {
        return (j - i) * min(ai, aj);
    }
public:
    int maxArea(vector<int>& height) {
        if (height.empty())
            return 0;

        int i = 0, j = height.size() - 1;
        int res = Area(i, j, height[i], height[j]);
        while (i < j) {
            if (height[i] < height[j]) {
                ++i;
                res = max(res, Area(i, j, height[i], height[j]));
            }
            else {
                --j;
                res = max(res, Area(i, j, height[i], height[j]));
            }
        }
        return res;

    }
};

int main() {
    TreeNode *root = new TreeNode(10);
    root->left = new TreeNode(5);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(8);
    root->left->right->left = new TreeNode(7);
    root->left->right->right = new TreeNode(9);
    root->right = new TreeNode(15);
    root->right->right = new TreeNode(17);

    //TreeNode *root = new TreeNode(0);
    //root->left = new TreeNode(1);
    //root->right = new TreeNode(1);


    vector<int> height = {2,3,10,5,7,8,9};

    //cout << std::boolalpha << Solution().isPalindrome(s) << endl;
    cout << Solution().maxArea(height) << endl;
}
