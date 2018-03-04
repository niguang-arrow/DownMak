#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;


struct Node {
    int data;
    Node *left;
    Node *right;

    Node(const int &d) : data(d), left(nullptr), right(nullptr) {}
};


Node* reconstruct(vector<int>::iterator &beg,
                vector<int>::iterator &end,
                const vector<int> &pre,
                const vector<int> &in) {
    if (beg == end)
        return nullptr;
    Node *root = &(*beg);
    vector<int>::iterator pos = find(in.begin(), in.end(), *beg);
    root->left = reconstruct(++beg, --pos);
    root->right = reconstruct(++pos, end);
}
void construct(const vector<int> &pre, const vector<int> &in) {
    assert(pre.size() == in.size());
    if (!pre.empty()) {
        Node *root = new Node(pre[0]);
        vector<int>::const_iterator pos = find(in.begin(), in.end(), pre[0]);

        root->left = reconstruct();
        root->right = reconstruct();

    }
}
