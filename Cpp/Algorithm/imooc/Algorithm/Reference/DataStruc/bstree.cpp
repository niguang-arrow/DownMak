// 二分搜索树的实现
#include <iostream>
#include <sstream>

using namespace std;


// 由于二分搜索树用于实现字典, 所以它要保存键值以及相应的值
template <typename Key, typename Value>
class BST {
private:
    struct Node {
        Key key;
        Value value;
        Node *left;
        Node *right;
        Node(Key k, Value v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node *root; // 树的根节点
    int count; // 用于统计当前树节点的个数

    // 向以node为根的二叉搜索树插入新节点, 返回插入新节点后 BST 的根
    Node* insert(Node *node, Key key, Value value);
    // 是否包含键值为 key 的节点
    bool contain(Node *node, Key key);
    // search 主要要注意返回值类型的设置, 由于Node 是 private 的, 所以不能直接
    // 那么, 如果返回值设置为 Value, 如果在search 中查找不到, 那就不好返回了,
    // 这时, 设置为 Value* 的话可以返回 nullptr
    Value* search(Node *node, Key key);

public:
    BST() : root(nullptr), count(0) {}
    BST(Key key, Value value) : root(new Node(key, value)), count(1) {}
    ~BST() {
        // TODO: ~BST
    }

    int size() const { return count; }
    bool isEmpty() const { return count == 0; }

    void insert(Key key, Value value) {
        // 内部的 insert 要返回插入新节点后二叉搜索树的根
        root = insert(root, key, value);
    }

    bool contain(Key key) {
        return constain(root, key);
    }

    Value* search(Key key) {
        return search(root, key);
    }
};

template <typename Key, typename Value>
typename BST<Key, Value>::Node*
BST<Key, Value>::insert(Node *node, Key key, Value value) {
    if (node == nullptr) {
        count++; // 千万不要忘了这个啊!!!
        return new Node(key, value);
    }

    if (key == node->key)
        node->value = value;
    else if (key < node->key)
        node->left = insert(node->left, key, value);
    else
        node->right = insert(node->right, key, value);

    return node;
}

template <typename Key, typename Value>
bool
BST<Key, Value>::contain(Node *node, Key key) {
    if (node == nullptr)
        return false;

    if (key == node->key)
        return true;
    else if (key < node->key)
        return contain(node->left, key);
    else
        return contain(node->right, key);
}

template <typename Key, typename Value>
Value*
BST<Key, Value>::search(Node *node, Key key) {
    if (node == nullptr)
        return nullptr;

    if (key == node->key)
        return &(node->value);
    else if (key < node->key)
        return search(node->left, key);
    else
        return search(node->right, key);
}


int main() {
    string sentence = "this this is is is a a a a real ha ha ha";
    istringstream ss(sentence);
    string str;

    // bstree 用于统计每个单词的频次.
    BST<string, int> bstree;
    while (ss.good()) {
        ss >> str;
        int* res = bstree.search(str);
        if (res)
            ++(*res);
        else
            bstree.insert(str, 1);
    }

    int *res = bstree.search("a");
    cout << *res  << endl;

    return 0;
}
