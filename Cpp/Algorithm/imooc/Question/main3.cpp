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
private:
    int sum = 0;
public:
    // 使用中序遍历, 但是先访问右子树, 再访问根节点, 最后访问
    // 左子树, 这样的话, 元素是从大到小排列, 使用 sum 记录访问
    // 到当前元素时, 前面已访问过的元素的和是多少, 访问完当前
    // 根节点的元素后, 不要忘记更新 sum 的值.
    TreeNode* convertBST(TreeNode* root) {
        if (!root)
            return nullptr;
        convertBST(root->right);
        root->val += sum;
        sum = root->val;
        convertBST(root->left);
        return root;
    }
};
//
//class Solution {
//private:
    //int addTilt(TreeNode *root, vector<int> &tilts) {
        //if (!root)
            //return 0;
        //if (!root->left && !root->right)
            //return root->val;
        //int leftTilt = addTilt(root->left, tilts);
        //int rightTilt = addTilt(root->right, tilts);

        //tilts.push_back(abs(rightTilt - leftTilt));
        //return abs(rightTilt - leftTilt);
    //}
//public:
    //int findTilt(TreeNode* root) {
        //if (!root)
            //return 0;
        //vector<int> tilts;
        //addTilt(root, tilts);

        //return std::accumulate(tilts.begin(), tilts.end(), 0);
    //}
//};

//class Solution {
//public:
    //int kthSmallest(vector<vector<int>>& matrix, int k) {
    //}
//};

int main() {
    TreeNode *root = new TreeNode(3);
    root->left = new TreeNode(2);
    root->right = new TreeNode(5);
    root->right->right = new TreeNode(6);
    root->right->left = new TreeNode(4);
    root->left->left = new TreeNode(1);
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
    string input = "thecattlewasrattledbythebattery";
    vector<string> words = {"cat","bat","rat"};
    auto res = Solution().numMatchingSubseq(input, words);
    cout << res << endl;
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

    //for (auto &d : res)
        //cout << d << " ";
    //cout << endl;
}
