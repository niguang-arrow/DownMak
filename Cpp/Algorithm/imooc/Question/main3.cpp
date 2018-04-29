// BOBO : leetcode 
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include "function.h"
#include <queue>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <climits>


using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

 struct Point {
     int x;
     int y;
     Point() : x(0), y(0) {}
     Point(int a, int b) : x(a), y(b) {}
 };

void preOrder(TreeNode *root) {
    if (!root)
        return;

    cout << root->val << " ";
    preOrder(root->left);
    preOrder(root->right);
}


class Solution {
private:
    struct Comp {
        bool operator()(const pair<char, int> &p1, const pair<char, int> &p2) {
            if (p1.first < p2.first)
                return true;
            else if (p1.first == p2.first && p1.second > p2.second)
                return true;
            return false;
        }
    };
public:
    int maximumSwap(int num) {
        string s = to_string(num);
        priority_queue<pair<char, int>, vector<pair<char, int>>, Comp> maxHeap;
        for (int i = 0; i < s.size(); ++i)
            maxHeap.push(make_pair(s[i], i));


        int i = 0, index = -1;
        while (!maxHeap.empty()) {
            auto p = maxHeap.top();
            maxHeap.pop();
            index = p.second;
            if (index != i) 
                break;
            ++i;
        }
        if (i < s.size())
            std::swap(s[index], s[i]);
        return std::stoi(s);
    }
};

int main() {
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(5);
    root->right->right = new TreeNode(6);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(4);
    //root->left->right->left = new TreeNode(7);
    //root->left->right->right = new TreeNode(9);
    //root->right->right = new TreeNode(17);

    //TreeNode *root2 = new TreeNode(0);
    //root2->left = new TreeNode(1);
    //root2->right = new TreeNode(1);


    //int arr[] = {1, 2, 3};
    //auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
 
    //vector<vector<char>> nums =  {{'A','B','C','E'},{'S','F','E','S'},{'A','D','E','E'}};
    //string word = "ABCESEEEFS";
   //vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    //vector<vector<int>> nums = {{0, 0, 0}, {0, 1, 0}, {1, 1, 1}};
    vector<int> nums = {1, 2, 3, 4};
    //string input = "owoztneoer";
    auto res = Solution().maximumSwap(1);
    cout << res << endl;
    //cout << std::boolalpha << res << endl;
    //preOrder(res);
    //cout << endl;
    //cout << res << endl;

    //for (auto &d : res) {
        //for (auto & data : d)
            //cout << data << " ";
        //cout << endl;
    //}
    //printLinkedList(res);
    //cout << res << endl;

    //for (auto &d : res)
        //cout << d << " ";
    //cout << endl;
}
