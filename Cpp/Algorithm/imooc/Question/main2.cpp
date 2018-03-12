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


void kMinNumber(vector<int> &array, int k) {
    if (array.empty() || array.size() < k || k <= 0)
        return;
    priority_queue<int> que;

    for (int i = 0; i < array.size(); ++i) {
        if (que.size() < k)
            que.push(array[i]);
        else {
            if (array[i] < que.top()) {
                que.pop();
                que.push(array[i]);
            }
        }
    }

    int arr[k];
    for (int i = k - 1; i >= 0; --i) {
        arr[i] = que.top();
        que.pop();
    }
    for (int i = 0; i < k; ++i)
        cout << arr[i] << " ";
    cout << endl;
}

int Partition(vector<int> &numbers, int start, int end) {
    if (numbers.empty() || start >= end || end >= numbers.size())
        return -1;

    int v = numbers[start];
    // 其中 arr[start+1...lt] 保存小于 v 的元素
    // arr[lt+1, gt) 保存等于 v 的元素
    // arr[gt, end] 保存大于 v 的元素
    // i 指向当前访问的元素
    int lt = start, gt = end + 1, i = start + 1;
    while (i < gt) {
        if (numbers[i] == v) { // 当当前访问的元素等于基准, 那么就直接访问下一个元素
            ++i;
        } // 如果小于基准, 那么就将其插入到 arr[start+1, lt] 的后面, 因此 lt 要加1
        else if (numbers[i] < v) {
            swap(numbers[i], numbers[++lt]);
            ++i;
        }
        else { // 如果大于基准, 便插入到 arr[gt, end] 的前面, 但是此时可以不用增加i,
          		// 因为交换后的位于 i 处的位置的元素还没有被判断.
            swap(numbers[i], numbers[--gt]);
        }
    }
  	// 最后只要将基准和 lt处的元素交换.
    swap(numbers[start], numbers[lt]);
    return lt;
}

void kMinNumberWithPartition(vector<int> &array, int k) {
    if (array.empty() || array.size() < k || k <= 0)
        return;

    int start = 0;
    int end = array.size() - 1;
    int index = Partition(array, start, end);
    while (index != k) {
        if (index > k) {
            end = index - 1;
            index = Partition(array, start, end);
        }
        else {
            start = index + 1;
            index = Partition(array, start, end);
        }
    }

    for (int i = 0; i < k; ++i)
        cout << array[i] << " ";
    cout << endl;
}

int main() {
    
    vector<int> array = {1, 2, 3, 2, 2, 2, 2, 5, 6};
    kMinNumber(array, 6);
    kMinNumberWithPartition(array, 6);
    return 0;
} 
