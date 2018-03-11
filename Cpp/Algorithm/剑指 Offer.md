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



### 面试题 23: 二叉搜索树的后序遍历

题目: 输入一个整数数组, 判断该数组是不是某二叉搜索树的后序遍历的结果. 如果是则返回 true, 否则返回 false. 假设输入的数组中任意两个数字都互不相同.