#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;


template <typename Key, typename Value>
class BSTree {
public:
    BSTree() : root(nullptr), count(0) {}
    ~BSTree() {
        // TODO: ~BSTree()
    }

    int size() const {
        return count;
    }
    
    bool isEmpty() const {
        return count == 0;
    }

    void insert(Key key, Value value) {
        // 向以 root 为根节点的子树插入一个新的节点
        root = insert(root, key, value);
    }

    // 非递归的实现
    void insert2(Key key, Value value) {
        root = insert2(root, key, value);
    }

    bool contain(Key key) {
        return contain(root, key);
    }

    Value* search(Key key) {
        return search(root, key);
    }

    void preOrder() {
        preOrder(root);
    }

    void inOrder() {
        inOrder(root);
    }

    void postOrder() {
        postOrder(root);
    }

    void layerOrder() {
        layerOrder(root);
    }

private:
    struct Node {
        Key key;
        Value value;
        Node *left;
        Node *right;
        Node(const Key &k, const Value &v) : 
            key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node *root;
    int count;

private:
    // 向以 node 为根的二叉搜索树插入新节点, 返回插入新节点
    // 后的二叉搜索树的根.
    Node* insert(Node *node, Key key, Value value) {

        if (node == nullptr) {
            count++;
            return new Node(key, value);
        }

        if (node->key == key)
            node->value = value;
        else if (key < node->key)
            node->left = insert(node->left, key, value);
        else
            node->right = insert(node->right, key, value);
        return node;
    }

    // insert 的非递归写法 TODO: 消除 Bug
    Node* insert2(Node *node, Key key, Value value) {

        Node *p = node;
        while (p) {
            if (key == p->key) {
                p->value = value;
                return node;
            } 
            else if (key < p->key)
                p = p->left;
            else
                p = p->right;
        }

        count++;
    }

    bool contain(Node *node, Key key) {

        if (node == nullptr)
            return false;

        if (key == node->key)
            return true;
        else if (key < node->key)
            return contain(node->left, key);
        else
            return contain(node->right, key);
    }

    Value* search(Node *node, Key key) {

        if (node == nullptr)
            return nullptr;
        if (key == node->key)
            return &(node->value);
        else if (key < node->key)
            return search(node->left, key);
        else
            return search(node->right, key);
    }

    void preOrder(Node *node) {
        if (node) {
            cout << node->key << endl;
            preOrder(node->left);
            preOrder(node->right);
        }
    }

    void inOrder(Node *node) {
        if (node) {
            inOrder(node->left);
            cout << node->key << endl;
            inOrder(node->right);
        }
    }

    void postOrder(Node *node) {
        if (node) {
            postOrder(node->left);
            postOrder(node->right);
            cout << node->key << endl;
        }
    }


    void layerOrder(Node *node) {
        queue<Node*> q;
        q.push(node);
        while (!q.empty()) {
            Node *n = q.front();
            cout << n->key << endl;
            q.pop();
            if (n->left)
                q.push(n->left);
            if (n->right)
                q.push(n->right);
        }
    }
};


int main() {

    BSTree<int, int> bst = BSTree<int, int>();
    for (int i = 0; i < 10; ++i)
        bst.insert2(i, i);
    bst.layerOrder();
    return 0;
}
