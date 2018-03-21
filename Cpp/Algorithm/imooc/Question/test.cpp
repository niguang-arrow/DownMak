// 实现一棵二叉树, 注意, 不是二叉搜索树, 所以插入的时候使用的
// 是广度优先的遍历方法.
#include <iostream>
#include <queue>
#include <initializer_list>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class BinaryTree {
public:
    TreeNode *root;
    
    void preOrder(TreeNode *node) {
        if (!node)
            return;
        cout << node->val << " ";
        preOrder(node->left);
        preOrder(node->right);
    }
public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree() {
        // TODO: 这个析构函数比较麻烦, 暂时不管
    }
    BinaryTree(initializer_list<int> element) : root(nullptr) {
        for (int d : element)
            insert(d);
    }

    void insert(int num) {
        if (!root) {
            root = new TreeNode(num);
            return;
        }

        queue<TreeNode*> Queue;
        Queue.push(root);
        while (!Queue.empty()) {
            TreeNode *node = Queue.front();
            Queue.pop();
            if (!node->left) {
                node->left = new TreeNode(num);
                return;
            }
            if (!node->right) {
                node->right = new TreeNode(num);
                return;
            }
            Queue.push(node->left);
            Queue.push(node->right);
        }
    }

    void preOrder() {
        if (root) {
            preOrder(root);
            cout << endl;
        }
    }
};

int main() {
    
    BinaryTree tree({1, 2, 3, 4, 5});
    tree.preOrder();
    //cout << tree.root->val << endl;
    //cout << tree.root->left->val << endl;
    //cout << tree.root->right->val << endl;
    cout << std::max(10, 11) << endl;

    stringstream ss("abc abc bbb bbb"), end;
    string s;
    while (ss >> s) {
        cout << s << " ";
    }
    cout << std::boolalpha << ss.eof() << endl;
    cout << endl;

}
