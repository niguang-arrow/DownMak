#include <iostream>
#include <vector>
#include <string>


using namespace std;


class TrieNode {
private:
    bool IsEnd;
    const int R = 26;
    vector<TrieNode*> children;
public:
    TrieNode() : IsEnd(false) {
        children.reserve(R);
    }
    TrieNode(const char &c) : TrieNode() {
        children[c - 'a'] = new TrieNode[R];
    }
    ~TrieNode() {
        for (auto &child : children)
            delete[] child;
    }

    bool containsKey(const char &ch) {
        return children[ch - 'a'] != nullptr;
    }

    TrieNode* get(const char &ch) {
        return children[ch - 'a'];
    }

    void put(const char &ch, TrieNode *node) {
        children[ch - 'a'] = node;
    }

    void setEnd() {
        IsEnd = true;
    }

    bool isEnd() {
        return IsEnd;
    }
};

class Trie {
private:
    TrieNode *root;
public:
    Trie() : root(new TrieNode()) {}
    Trie(const vector<string> &words) : Trie() {
        for (const auto &word : words)
            insert(word);
    }

    void insert(const string &word) {
        TrieNode *node = root;
        for (int i = 0; i < word.size(); ++i) {
            if (!node->containsKey(word[i]))
                node->put(word[i], new TrieNode());
            node = node->get(word[i]);
        }
        node->setEnd();
    }

    TrieNode* searchPrefix(const string &word) {
        TrieNode *node = root;
        for (int i = 0; i < word.size(); ++i) {
            if (node->containsKey(word[i]))
                node = node->get(word[i]);
            else
                return nullptr;
        }
        return node;

    }
    bool search(const string &word) {
        TrieNode *node = searchPrefix(word);
        return node != nullptr && node->isEnd();
    }

    // Returns if there is any word in the trie
    // that starts with the given prefix.
    bool startsWith(const string &prefix) {
        TrieNode *node = searchPrefix(prefix);
        return node != nullptr;
    }
};

class A {
private:
    const int R = 26;
    // 使用 vector<int> res(26) 初始化是错误的, 会报错 
    // error: expected parameter declarator
    // 这是因为编译器把这里当函数声明了, 希望你传入类型参数.
    // 解决方法是使用大括号, 或者在构造函数初始值列表中初始化,
    // 或者使用下面的方法
    //vector<int> res = vector<int>(26);
    vector<int> res;
public:
    A() : res(26, 1){
        for (auto &i : res)
            cout << i << endl;
    }
};

int main() {
    vector<string> words = {"abc", "trie", "leetcode", "lee"};
    auto tree = Trie(words);

    string prefix = "le";
    auto res1 = tree.search(prefix);
    cout << std::boolalpha << res1 << endl;
    auto res2 = tree.startsWith(prefix);
    cout << std::boolalpha << res2 << endl;

    auto a = A();
}
