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
    vector<vector<bool>> seen;
    bool dfs(vector<vector<char>> &board, int i, int j, string word, int index) {
        if (i < 0 || i >= board.size() ||
            j < 0 || j >= board[0].size() ||
            seen[i][j] || index >= word.size() ||
            board[i][j] != word[index]) {
            if (i == 1 && j == 3)
                cout << "index: " << index << (board[i][j] == word[index]) << endl;
            cout << "i: " << i << " " << j << endl;
            return false;
        }

        if (index == word.size() - 1 && board[i][j] == word[index])
            return true;
        seen[i][j] = true;

        cout << "index: " << index << " (i, j): " << i << "," << j << " "
            << std::boolalpha << (board[i][j] == word[index]) << endl;
        return (dfs(board, i - 1, j, word, index + 1) ||
            dfs(board, i + 1, j, word, index + 1) ||
            dfs(board, i, j - 1, word, index + 1) ||
            dfs(board, i, j + 1, word, index + 1));
    }
public:
    bool exist(vector<vector<char>>& board, string word) {
        if (board.empty() || board[0].empty())
            return false;
        //seen = vector<vector<bool>>(board.size(), vector<bool>(board[0].size(), false));
        //cout << "(i, j): " << 0 << "," << 0 << " "
            //<< std::boolalpha << dfs(board, 0, 0, word, 0) << endl;
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[0].size(); ++j) {
                seen = vector<vector<bool>>(board.size(), vector<bool>(board[0].size(), false));
                //cout << "(i, j): " << i << "," << j << " "
                    //<< std::boolalpha << dfs(board, i, j, word, 0) << endl;
                if (board[i][j] == word[0])
                    if(dfs(board, i, j, word, 0))
                        return true;
            }
        }
        return false;
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

    //TreeNode *root = new TreeNode(0);
    //root->left = new TreeNode(1);
    //root->right = new TreeNode(1);


    //int arr[] = {-1, -1, -1, -1, -1, 0};
    //auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
 
    vector<vector<char>> nums =  {{'A','B','C','E'},{'S','F','E','S'},{'A','D','E','E'}};
    string word = "ABCESEEEFS";
   //vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    //vector<vector<int>> nums = {{1, 2, 3}, {4, 5, 6}};
    //vector<int> nums = {1, 2};
    //string s = "babad";
    auto res = Solution().exist(nums, word);
    //cout << res << endl;
    cout << std::boolalpha << res << endl;
    //preOrder(root);
    //cout << endl;
    //cout << res << endl;

    //for (auto &d : res) {
        //for (auto & data : d)
            //cout << data << " ";
        //cout << endl;
    //}
    //printLinkedList(root);
    //cout << res << endl;

    //for (auto &d : res)
        //cout << d << " ";
    //cout << endl;
}
