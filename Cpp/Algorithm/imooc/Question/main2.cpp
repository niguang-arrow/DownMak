// 左成云: 5. 用一个栈实现另一个栈的排序
#include <iostream>
#include <vector>
#include <stack>
#include "function.h"
#include <unordered_map>
#include <queue>
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

//BinaryTreeNode<int>* convert(BSTree<int> &tree) {
    //BinaryTreeNode<int> *root = tree.root;
//}

//int ArrayPartition(vector<int> &array) {
    //if (array.empty())
        //return -1;
    //int sz = array.size();
    //int 
//}


int SubsequenceMaxSum(vector<int> &array) {
    if (array.empty())
        return -1;

    int res = array[0];
    int sum = res;
    // 使用 res 保存当访问到 i 时得到的最大和, 用 sum 保存
    // 以往的连续子数组的最大值, 最后返回 sum 即可.
    for (int i = 1; i < array.size(); ++i) {
        res = max(res + array[i], array[i]);
        sum = max(sum, res);
    }
    return sum;
}


int main() {
    vector<int> array = {1, -2, 3, 10, -4, 7, 2, -5};
    cout << SubsequenceMaxSum(array) << endl;
    return 0;
}
