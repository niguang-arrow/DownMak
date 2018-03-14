// 左程云: 二叉树问题: 实现二叉树的深度遍历
#include <iostream>
#include "function.h"
#include <stack>
#include <vector>


using namespace std;


void preOrder(BinaryTreeNode<int> *root) {
    if (root) {
        cout << root->val << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(BinaryTreeNode<int> *root) {
    if (root) {
        inOrder(root->left);
        cout << root->val << " ";
        inOrder(root->right);
    }
}

void postOrder(BinaryTreeNode<int> *root) {
    if (root) {
        postOrder(root->left);
        postOrder(root->right);
        cout << root->val << " ";
    }
}


int maxDepth(BinaryTreeNode<int> *root) {
    if (!root)
        return 0;
    if (!root->left && !root->right)
        return 1;

    int depth = 0;
    depth = max(maxDepth(root->left), maxDepth(root->right)) + 1;
    return depth;

}

int minDepth(BinaryTreeNode<int> *root) {
    if (!root)
        return 0;
    if ((!root->right && root->left)) {
        return minDepth(root->left) + 1;
    }
    if (!root->left && root->right) {
        return minDepth(root->right) + 1;
    }

    int depth = min(minDepth(root->left), minDepth(root->right)) + 1;
    return depth;

}

BinaryTreeNode<int>* invert(BinaryTreeNode<int> *root) {
    if (!root)
        return nullptr;

    root->left = invert(root->left);
    root->right = invert(root->right);
    swap(root->left, root->right);
    //BinaryTreeNode<int> *temp = root->left;
    //root->left = root->right;
    //root->right = temp;
    return root;
}

int main() {
    //BSTree<int> tree = {4, 5, 2, 3, 1, 0, 6};
    BSTree<int> tree = {1, 2, 3, 4, 5, 6};
    //BSTree<int> tree = {4, 5};
    cout << minDepth(tree.root) << endl;
    cout << endl;
    return 0;
}
