// 左成云: 5. 用一个栈实现另一个栈的排序
#include <iostream>
#include <vector>
#include <stack>
#include "function.h"
using namespace std;

bool isConnected(vector<int> &in, vector<int> &out) {
    if (!in.size() || !out.size() || in.size() != out.size())
        throw "invalid input";

    stack<int> stk;
    int j = 0;
    stk.push(in[0]);
    for (int i = 1; i < in.size(); ) {
        if (stk.top() != out[j]) {
            stk.push(in[i]);
            ++i;
        } else {
            stk.pop();
            j++;
        }
    }

    while (!stk.empty()) {
        if (stk.top() == out[j]) {
            stk.pop();
            ++j;
        } else {
            return false;
        }
    }
    return true;
}

bool isBSTree(vector<int> &post, int start, int end) {
    if (start > end)
        return true;
    int root = post[end];
    int i = end - 1;
    while (i >= start) {
        if (post[i] < root)
            break;
        i--;
    }

    for (int k = end - 1; k > i; k--) {
        if (post[k] < root)
            return false;
    }

    for (int k = i; k >= start; k--)
        if (post[k] > root)
            return false;

    return isBSTree(post, start, i) && isBSTree(post, i + 1, end - 1);
}

void findPath(BinaryTreeNode<int> *node, vector<int> &path, int &sum, int target) {
    if (node) {
        sum += node->val;
        if (sum == target) {
            path.push_back(node->val);
        } else {
            findPath(node->left, path, sum, target);
            findPath(node->right, path, sum, target);
        }
    } else {
        sum -= path.back();
        path.pop_back();
    }
}

void findPathToTarget(BSTree<int> &tree, int target) {
    if (tree.empty())
        return;

    vector<int> path;
    BinaryTreeNode<int> *node = tree.root;
    int sum = 0;
    findPath(node, path, sum, target);

    cout << sum << endl;
    cout << path[0] << endl;
    //for (const auto &d : path)
        //cout << d << " ";
    //cout << endl;
} 


struct ComplexListNode {
    int val;
    ComplexListNode *next;
    ComplexListNode *pSibling;
};

int main() {

    //vector<int> in = {1, 2, 3, 4, 5};
    //vector<int> out1 = {4, 5, 3, 2, 1};
    //vector<int> out2 = {4, 3, 5, 1, 2};
    //vector<int> out3 = {5, 4, 3, 2, 1};
    //cout << isConnected(in, out1) << endl;
    //cout << isConnected(in, out2) << endl;
    //cout << isConnected(in, out3) << endl;
    //
    BSTree<int> tree = {4, 5, 2, 3, 1, 0, 6};
    tree.postOrder();
    
    vector<int> post = {0, 1, 3, 2, 6, 5, 4};
    cout << isBSTree(post, 0, post.size() - 1) << endl;

    findPathToTarget(tree, 7);
    return 0;
} 
