// 左成云: 用递归和非递归实现二叉树的先序,中序,后序遍历
#include <iostream>
#include <vector>
#include <stack>
#include "function.h"
#include <unordered_map>
#include <queue>
#include <cassert>
using namespace std;


void preOrder(BinaryTreeNode<int>* root) {
    if (!root)
        return;
    cout << root->val << " ";
    preOrder(root->left);
    preOrder(root->right);
}

void inOrder(BinaryTreeNode<int>* root) {
    if (!root)
        return;
    inOrder(root->left);
    cout << root->val << " ";
    inOrder(root->right);
}

void postOrder(BinaryTreeNode<int>* root) {
    if (!root)
        return;
    postOrder(root->left);
    postOrder(root->right);
    cout << root->val << " ";
}


struct Command {
    string s; // go or print
    BinaryTreeNode<int> *node;

    Command(string str, BinaryTreeNode<int> *n)
        : s(str), node(n) {}
};

// 使用一个栈来模拟系统栈对指令的分析
void preOrderTraversal(BinaryTreeNode<int> *root) {
    if (!root)
        return;

    stack<Command> Stack;
    Stack.push(Command("go", root));

    while (!Stack.empty()) {
        Command command = Stack.top();
        Stack.pop();

        if (command.s == "print")
            cout << command.node->val << " ";
        else {
            assert(command.s == "go");
            if (command.node->right)
                Stack.push(Command("go", command.node->right));
            if (command.node->left)
                Stack.push(Command("go", command.node->left));
            Stack.push(Command("print", command.node));
        }
    }
}

void inOrderTraversal(BinaryTreeNode<int> *root) {
    if (!root)
        return;

    stack<Command> Stack;
    Stack.push(Command("go", root));

    while (!Stack.empty()) {
        Command command = Stack.top();
        Stack.pop();

        if (command.s == "print")
            cout << command.node->val << " ";
        else {
            assert(command.s == "go");
            if (command.node->right)
                Stack.push(Command("go", command.node->right));
            Stack.push(Command("print", command.node));
            if (command.node->left)
                Stack.push(Command("go", command.node->left));
        }
    }
}

void postOrderTraversal(BinaryTreeNode<int> *root) {
    if (!root)
        return;

    stack<Command> Stack;
    Stack.push(Command("go", root));

    while (!Stack.empty()) {
        Command command = Stack.top();
        Stack.pop();

        if (command.s == "print")
            cout << command.node->val << " ";
        else {
            assert(command.s == "go");
            Stack.push(Command("print", command.node));
            if (command.node->right)
                Stack.push(Command("go", command.node->right));
            if (command.node->left)
                Stack.push(Command("go", command.node->left));
        }
    }
}

int main() {
    BSTree<int> tree = {4, 5, 2, 1, 3, 0, 6};
    preOrder(tree.root);
    cout << endl;
    preOrderTraversal(tree.root);
    cout << endl;
    inOrder(tree.root);
    cout << endl;
    inOrderTraversal(tree.root);
    cout << endl;
    postOrder(tree.root);
    cout << endl;
    postOrderTraversal(tree.root);
    cout << endl;
    return 0;
}
