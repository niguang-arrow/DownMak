#include <iostream>
#include <vector>
#include <string>
#include "FileOps.h"
#include "SequenceST.h"


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

    bool contain(Key key) {
        return contain(root, key);
    }

    Value* search(Key key) {
        return search(root, key);
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

    // insert 的非递归写法
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
        return new Node(key, value);
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
};


// 测试二分搜索树与顺序查找表之间的性能

int main() {
    string filename = "bible.txt";
    vector<string> words;
    if (FileOps::readFile(filename, words)) {
        cout << "There are totally " << words.size() << " words in " << filename << endl;
        cout << endl;


        // 测试 BST
        time_t startTime = clock();

        // 统计圣经中所有词的词频
        // 注: 这个词频统计法相对简陋, 没有考虑很多文本处理中的特殊问题
        // 在这里只做性能测试用
        BSTree<string, int> bst = BSTree<string, int>();
        for (vector<string>::iterator iter = words.begin(); iter != words.end(); ++iter) {
            int* val = bst.search(*iter);
            if (val)
                (*val)++;
            else
                bst.insert(*iter, 1);
        }
        // 输出圣经中 god 这个词出现的频率
        if (bst.contain("god"))
            cout << "'god' : " << *bst.search("god") << endl;
        else
            cout << "No word 'god' in " << filename << endl;

        time_t endTime = clock();

        cout << "BST , time: " << double(endTime - startTime) / CLOCKS_PER_SEC << " s." << endl;
        cout << endl;


        // 测试顺序查找表 SST
        startTime = clock();

        // 统计圣经中所有词的词频
        // 注: 这个词频统计法相对简陋, 没有考虑很多文本处理中的特殊问题
        // 在这里只做性能测试用
        SequenceST<string, int> sst = SequenceST<string, int>();
        for (vector<string>::iterator iter = words.begin(); iter != words.end(); iter++) {
            int *res = sst.search(*iter);
            if (res == NULL)
                sst.insert(*iter, 1);
            else
                (*res)++;
        }

        // 输出圣经中god一词出现的频率
        if(sst.contain("god"))
            cout << "'god' : " << *sst.search("god") << endl;
        else
            cout << "No word 'god' in " << filename << endl;

        endTime = clock();

        cout << "SST , time: " << double(endTime - startTime) / CLOCKS_PER_SEC << " s." << endl;
    }
    return 0;
}
