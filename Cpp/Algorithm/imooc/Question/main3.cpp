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
#include <iterator>
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
    int distance(vector<int> &A, vector<int> &B) {
        return (A[0] - B[0]) * (A[0] - B[0]) +
            (A[1] - B[1]) * (A[1] - B[1]);
    }
    bool perpendicular(vector<int> &p1, vector<int> &p2, vector<int> &p3) {
        vector<int> AB = {p2[0] - p1[0], p2[1] - p1[1]};
        vector<int> BC = {p3[0] - p2[0], p3[1] - p2[1]};

        return (AB[0] * BC[0] + AB[1] * BC[1]) == 0;
    }
public:
    bool validSquare(vector<int>& p1, vector<int>& p2, vector<int>& p3, vector<int>& p4) {
        int d12 = distance(p1, p2);
        int d23 = distance(p2, p3);
        int d34 = distance(p3, p4);
        int d41 = distance(p4, p1);
        if (d12 == d23 && d23 == d34 && d34 == d41 && d41 == d12 &&
            perpendicular(p2, p1, p4) &&
            perpendicular(p1, p2, p3) &&
            perpendicular(p3, p4, p1) &&
            perpendicular(p4, p1, p2))
            return true;

        return false;
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
    string input = "thecattlewasrattledbythebattery";
    vector<string> words = {"cat","bat","rat"};
    auto res = Solution().numMatchingSubseq(input, words);
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
