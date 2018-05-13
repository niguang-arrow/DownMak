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
    bool matches(string &word, unordered_map<char, int> &plate) {
        unordered_map<char, int> record;
        for (auto &c : word)
            record[c] ++;
        
        // 这里的两个判断条件在思路中解释了.
        for (auto &iter : plate) {
            if (!record.count(iter.first) ||
                (record.count(iter.first) && record[iter.first] < iter.second))
                return false; 
        }
        return true;
    }
public:
    string shortestCompletingWord(string licensePlate, vector<string>& words) {
      	// 统计 licensePlate 中的字符个数
        unordered_map<char, int> plate;
        for (auto &c : licensePlate)
            if (isalpha(c))
                plate[tolower(c)] ++;
        for (auto &iter : plate)
            cout << iter.first << " " << iter.second << endl;
		
        string res;
        int len = INT32_MAX;
        for (auto &word : words) {
          	// 找到长度最短的 word.
            if (matches(word, plate) && word.size() < len) {
                res = word;
                len = word.size();
            }
        }
        return res;
    }
};

//class Solution {
//public:
    //int kthSmallest(vector<vector<int>>& matrix, int k) {
    //}
//};

int main() {
    TreeNode *root = new TreeNode(3);
    root->left = new TreeNode(2);
    root->right = new TreeNode(5);
    root->right->right = new TreeNode(6);
    root->right->left = new TreeNode(4);
    root->left->left = new TreeNode(1);
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
    vector<int> nums1 = {2, 4};
    vector<int> nums = {1, 2, 3, 4};
    string input = "1s3 PSt";
    vector<string> words = {"step","steps","stripe","stepple"};
    auto res = Solution().shortestCompletingWord(input, words);
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
