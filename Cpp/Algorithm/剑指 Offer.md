# 剑指 Offer

2018 年 3 月 10 日

## 第四章 解决面试题的思路

### 面试题 19: 二叉树的镜像

题目: 请完成一个函数, 输入一个二叉树, 该函数输出它的镜像.

我为了写好测试用例, 还实现了一棵二叉搜索树中的部分操作. 之后, 我会把这个实现放到 `function.h` 文件中, 如果以后这本书还有相关的题, 就直接使用, 不用再重新写了.

```cpp
#include <iostream>
#include <vector>
#include <initializer_list>
using namespace std;

// 二叉树的节点
template <typename T>
struct BinaryTreeNode {
    T val;
    BinaryTreeNode *left;
    BinaryTreeNode *right;
    BinaryTreeNode(T v) : val(v), left(nullptr), right(nullptr) {}
};

// 二分搜索树, 只实现了插入, 深度优先遍历, 以及题目要求的求
// 二叉树的镜像 reverse() 函数.
template <typename T>
class BSTree {
private:
    BinaryTreeNode<T> *root;
    int count;
	
  	// 注意插入的时候, 要维护 count 的数值.
    BinaryTreeNode<T>* insert(BinaryTreeNode<T> *node, T val) {
        if (node == nullptr) {
            count++;
            return new BinaryTreeNode<T>(val);
        }

        if (val == node->val)
            node->val = val;
        else if (val < node->val)
            node->left = insert(node->left, val);
        else
            node->right = insert(node->right, val);

        return node;
    }
	
  	// 注意 public 的 preOrder 使用了 const, 那么这里也需要 const
  	// 否则编译无法通过.
    void preOrder(BinaryTreeNode<T> *node) const {
        if (node) {
            cout << node->val << " ";
            preOrder(node->left);
            preOrder(node->right);
        }
    }

    void inOrder(BinaryTreeNode<T> *node ) const {
        if (node) {
            inOrder(node->left);
            cout << node->val << " ";
            inOrder(node->right);
        }
    }

    void postOrder(BinaryTreeNode<T> *node ) const {
        if (node) {
            postOrder(node->left);
            postOrder(node->right);
            cout << node->val << " ";
        }
    }
	
  	// 用于 BSTree 的析构, 使用后向遍历的思路
    void clear(BinaryTreeNode<T> *node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }
	
  	// reverse 的思路是, 遍历非叶子节点, 判断条件是: 当前节点不为空, 
  	// 并且总有一个孩子(也就是左右孩子不能同时为 nullptr)
  	// 将非叶子节点的左右孩子进行交换.
    void reverse(BinaryTreeNode<T> *node) {
        if (!node || (!node->left && !node->right))
            return;
        BinaryTreeNode<T> *temp = node->left;
        node->left = node->right;
        node->right = temp;
        reverse(node->left);
        reverse(node->right);
    }

public:
    BSTree() : root(nullptr), count(0) {}
    BSTree(initializer_list<T> ls) : root(nullptr), count(0) {
        for (const auto &d : ls)
            insert(d);
    }
    ~BSTree() {
        clear(root);
    }

    int size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }
    void insert(T val) {
        root = insert(root, val);
    }

    void preOrder() const {
        preOrder(root);
        cout << endl;
    }

    void inOrder() const {
        inOrder(root);
        cout << endl;
    }

    void postOrder() const {
        postOrder(root);
        cout << endl;
    }

    void reverse() {
        reverse(root);
    }
};


int main() {
    BSTree<int> tree = {4, 5, 2, 3, 1, 0, 6};
    cout << tree.size() << endl;
    tree.preOrder();
    tree.inOrder();
    tree.postOrder();
    tree.reverse();
    tree.preOrder();
    tree.inOrder();
    tree.postOrder();
    return 0;
}

// Output:
7
// 翻转前的结果
4 2 1 0 3 5 6 
0 1 2 3 4 5 6 
0 1 3 2 6 5 4 
// 翻转后的结果
4 5 6 2 3 1 0 
6 5 4 3 2 1 0 
6 5 3 0 1 2 4
```



### 面试题 23: 从上往下打印二叉树

这就是二叉树的广度优先遍历, 实现方法是引入一个队列, 将要访问的节点入队, 然后判断队列是否为空, 若不为空, 则将节点出队, 并将该节点的左右孩子入队; 重复以上过程直到队列为空;

将下面的代码加在面试题 19: 二叉树的镜像中的 public 代码中. 

```cpp
    // 广度优先遍历
    void BreadthFirst() {
        queue<BinaryTreeNode<T>*> que;
        if (root) {
            que.push(root);
            while (!que.empty()) {
                BinaryTreeNode<T> *node = que.front();
                que.pop();
                if (node) {
                    cout << node->val << " ";
                    que.push(node->left);
                    que.push(node->right);
                }
            }
            cout << endl;
        }
    }

// 测试
int main() {
    BSTree<int> tree = {4, 5, 2, 3, 1, 0, 6};
    tree.BreadthFirst();
    return 0;
} //Output: 4 2 5 1 3 6 0
```



### 面试题 24: 二叉搜索树的后序遍历

题目: 输入一个整数数组, 判断该数组是不是某二叉搜索树的后序遍历的结果. 如果是则返回 true, 否则返回 false. 假设输入的数组中任意两个数字都互不相同.

思路是注意二叉搜索树本身的递归结构, 后序遍历的最后一个节点是根节点, 然后它前面的节点是它的左右子树; 接下来要在它前面的节点中将左右子树分开, 只需要找到第一个小于它的节点, 就是左右子树的分界点. 然后再对左右子树递归操作.

```cpp
bool isBSTree(vector<int> &post, int start, int end) {
  	// 如果一个子节点都没有, 那么返回 true
    if (start > end)
        return true;
  	// 最后一个节点是根节点
    int root = post[end];
  	// 判断根节点前面的第一个小于它的节点
    int i = end - 1;
    while (i >= start) {
        if (post[i] < root)
            break;
        i--;
    }
	
  	// 找到第一个小于根节点的左右子树的分界点后, 还需要判断分界点
  	// 左边的节点是否都小于 root, 并且右边的节点是否都大于 root
    for (int k = end - 1; k > i; k--) {
        if (post[k] < root)
            return false;
    }

    for (int k = i; k >= start; k--)
        if (post[k] > root)
            return false;
	// 然后得保证左右子树同时满足条件.
    return isBSTree(post, start, i) && isBSTree(post, i + 1, end - 1);
}

int main() {

    BSTree<int> tree = {4, 5, 2, 3, 1, 0, 6};
    tree.postOrder();
    
    vector<int> post = {0, 1, 3, 2, 6, 5, 4};
    cout << isBSTree(post, 0, post.size() - 1) << endl;
    return 0;
} 
```



### 面试题 25: 二叉树中和为某一值的路径 

题目: 输入一棵二叉树和一个整数, 打印出二叉树中节点值的和为输入整数的所有路径. 从树的根节点开始往下一直到叶节点所经过的节点形成一条路径.

代码写在 main2.cpp 中, 还需要调试.



### 面试题 26: 复杂链表的复制

题目: 请实现函数 `ComplexListNode* Clone(ComplexListNode *pHead)`, 复制一个复杂链表. 在复杂链表中, 每个节点除了有一个 `next` 指针指向下一个节点外, 还有一个 `pSibling` 指向链表中的任意节点或者 NULL.