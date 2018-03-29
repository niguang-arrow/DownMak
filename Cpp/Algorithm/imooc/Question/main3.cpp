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
    void flatten(TreeNode *root) {
        if (!root || (!root->left && !root->right))
            return;

        flatten(root->left);
        flatten(root->right);

        if (!root->left) return;

        auto p = root->left;
        while (p->right) p = p->right;
        p->right = root->right;
        root->right = root->left;
        root->left = nullptr;

    }
};


bool exists(vector<int> &nums) {
    if (nums.size() < 3)
        return false;

    priority_queue<pair<int, int>> Queue;
    for (int i = 0; i < nums.size(); ++i)
        Queue.push(make_pair(nums[i], i));

    while (!Queue.empty()) {
        auto ele = Queue.top();
        Queue.pop();

        if (ele.second < Queue.size())
            return true;
    }

    return false;
}


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


    int arr[] = { 1, 1, 2, 2, 1, 2, 2, 2, 2 };
    //int arr[] = {1};
    //auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
    vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    cout << std::boolalpha << exists(nums) << endl;
    //auto res = Solution().quickSort(nums);
    Solution().flatten(root);
    preOrder(root);
    cout << endl;
    //cout << res << endl;
    //for (auto &d : res) {
        //for (auto & data : d)
            //cout << data << " ";
        //cout << endl;
    //}
    //printLinkedList(root);
    //cout << res << endl;

    //for (auto &d : nums)
        //cout << d << " ";
    //cout << endl;
    //cout << std::boolalpha << res << endl;

}
