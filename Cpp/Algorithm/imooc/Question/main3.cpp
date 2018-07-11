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
#include <cstring>
#include <climits>
#include <cmath>


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
    TreeNode* KthNode(TreeNode* root, int k) {
        int idx = 0;
        TreeNode *res = nullptr;
        KthNode(root, res, k, idx);
        return res;
    }
private:
    void KthNode(TreeNode *root, TreeNode* &res, int k, int &idx) {
        if (!root || k <= 0) return;
        KthNode(root->left, res, k, idx);
        idx ++;
        cout << "1idx: " << idx << endl;
        if (idx == k) {
            cout << "idx: " << idx << endl;
            res = root;
            cout << "res: " << res << endl;
            return;
        }
        KthNode(root->right, res, k, idx);
    }
};

class Codec {
public:
    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        if (!root) return "";
        ostringstream os;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode *t = q.front(); q.pop();
            if (t) {
            os << t->val << " ";
            q.push(t->left);
            q.push(t->right);
            } else {
                os << "# ";
            }
            cout << os.str() << endl;
        }
        return os.str();
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        istringstream in(data);
        return deserialize(in);
    }
    
    // 前序遍历
    void serialize(TreeNode *root, ostringstream &out) {
        if (!root) {
            out << "# ";
            return;
        }
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            auto r = q.front();
            q.pop();
            if (!r) {
                out << "# ";
                continue;
            }
            out << r->val << " ";
            q.push(r->left);
            q.push(r->right);
            cout << out.str() << endl;
        }
    }

    TreeNode* deserialize(istringstream &in) {
        string val;
        in >> val;
        if (val == "#") return nullptr;
        TreeNode *root = new TreeNode(stoi(val));
        auto ptr = root;
        queue<TreeNode*> q;
        q.push(ptr);
        while (!q.empty()) {
            auto r = q.front();
            q.pop();
            if (!(in >> val)) break;
            if (val != "#") {
                ptr = new TreeNode(stoi(val));
                q.push(ptr);
                r->left = ptr;
            }
            if (!(in >> val)) break;
            if (val != "#") {
                ptr = new TreeNode(stoi(val));
                q.push(ptr);
                r->right = ptr;
            }
        }
        return root;
    } 
};

int main() {
    TreeNode *root = new TreeNode(2);
    root->left = new TreeNode(1);
    root->right = new TreeNode(4);
    //root->left->left = new TreeNode(1);
    root->right->left = new TreeNode(3);
    root->right->right = new TreeNode(6);
    //root->left->right->left = new TreeNode(7);
    //root->left->right->right = new TreeNode(9);
    //root->right->right = new TreeNode(17);


    //int arr[] = {1, 2,3,  4, 5};
    //vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    //auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
 
    vector<vector<char>> board =  {{'A','B','C','E'},
                                   {'S','F','E','S'},
                                   {'A','D','E','E'}};
    string word = "ABCESEEEFS";
    //vector<vector<int>> nums = {{0, 0, 0}, {0, 1, 0}, {1, 1, 1}};
    //vector<int> nums1 = {1, 2, 2, 1};
    vector<int> nums1 = {5, 7, 6, 9, 11, 10,};
    //vector<int> nums1 = {3, 2};
    vector<int> preorder = {9, 3, 15, 20, 7};
    vector<int> inorder = {9, 15, 7, 20, 3};
    vector<vector<int>> nums = {{1,2,3,4}, {5,6,7,8}, {9,10,11,12}};
    string input = "thecattlewasrattledbythebattery";
    vector<string> words = {"a", "banana", "app", "appl", "ap", "apply", "apple"};
    //auto res = Solution().permuteUnique(nums1);
    //auto res = Solution().KthNode(root, 1);
    //cout << res << endl;
    Codec a;
    auto str = a.serialize(root);
    cout << str << endl;
    ostringstream os;
    a.serialize(root, os);
    cout << os.str() << endl;
    //cout << std::boolalpha << res << endl;
    //preOrder(res);
    //cout << endl;

    //printVector(res);
    //printMatrix(res);
    //printLinkedList(res);
}
