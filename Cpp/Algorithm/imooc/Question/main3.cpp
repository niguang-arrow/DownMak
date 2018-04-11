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
    void print(vector<int> &nums) {
        for (auto &d : nums)
            cout << d << " ";
        cout << endl;
    }
    void print2(vector<vector<int>> &nums) {
        for (auto &data : nums) {
            for (auto &d : data)
                cout << d << " ";
            cout << endl;
        }
        cout << endl;
    }
public:
    vector<vector<int>> subsets(vector<int>& nums) {

        //vector<vector<int>> res{
            //{},
            //{1, 2}
        //};
        //vector<int> cur;

        //auto start = res.begin();
        //auto end = res.end();
        //while (start != end) {
            //cur = *start;
            //cout << "start" << endl;
            //print(cur);
            //cur.push_back(5);//nums[i]);
            //print(cur);
            //res.push_back(cur);
            //cout << "res" << endl;
            //print2(res);
            //start++;
        //}
        //return vector<vector<int>>{};
        int n = nums.size();
        int i = 0;
        vector<vector<int>> res;
        vector<int> cur;
        res.push_back(cur);
        while (i < n) {
            int start = 0;
            int end = res.size();

            while (start < end) {
                cur = res[start];
                cur.push_back(nums[i]);
                ++start;
                res.push_back(cur);
            }
            ++i;
        }
        return res;
    }
};

//class Solution {
//public:
    //bool increasingTriplet(vector<int>& nums) {
        //int c1 = INT32_MAX, c2 = INT32_MAX;
        //for (int x : nums) {
            //if (x <= c1) {
                //c1 = x;           // c1 is min seen so far (it's a candidate for 1st element)
            //} else if (x <= c2) { // here when x > c1, i.e. x might be either c2 or c3
                //c2 = x;           // x is better than the current c2, store it
            //} else {              // here when we have/had c1 < c2 already and x > c2
                //return true;      // the increasing subsequence of 3 elements exists
            //}
            //cout << c1 << " " << c2 << endl;
        //}
        //return false;
//}
//};

//class Solution {
//public:
    //string reverseWords(string s) {

        //stringstream ss(s);
        //string str;
        //string res;
        //while (ss >> str) {
            //std::reverse(str.begin(), str.end());
            //res += str + " ";
        //}
        //res.pop_back();
        //return res;
    //}
//};

//class Solution {
//public:
    //string simplifyPath(string path) {
        //stack<
    //}
//};

//class Solution {
//public:
    //int maximumProduct(vector<int>& nums) {
        //int n = nums.size();
        //std::sort(nums.begin(), nums.end());
        //int res = max(nums[0] * nums[1] * nums[2],
                //max(nums[n - 3] * nums[n - 2] * nums[n - 1],
                //nums[0] * nums[1] * nums[n - 1]));
        //return res;
    //}
//};

//class Solution {
//private:
    //// 对 nums[start, ... end] 进行 partition
    //int partition(vector<int> &nums, int start, int end) {
        //if (start > end)
            //return -1;
        //int v = nums[start];
        //// nums[start + 1...lt] < v
        //// nums[lt + 1, ...gt) == v
        //// nums[gt ... end] > v
        //// i 为当前访问的元素
        //int lt = start, gt = end + 1, i = start + 1;
        //while (i < gt) {
            //if (nums[i] == v)
                //i ++;
            //else if (nums[i] < v)
                //swap(nums[++lt], nums[i++]);
            //else
                //swap(nums[--gt], nums[i]);
        //}

        //swap(nums[lt], nums[start]);
        //return lt;
    //}
//public:
    //int majorityElement(vector<int>& nums) {
        
        //int  n = nums.size();
        //int index = partition(nums, 0, n - 1);
        //while (index != n / 2) {
            //if (index < n / 2)
                //index = partition(nums, index + 1, n - 1);
            //else
                //index = partition(nums, 0, index - 1);
        //}

        //return nums[index];
    //}
//};


//class Solution {
//public:
    //int findMaxConsecutiveOnes(vector<int>& nums) {
        //int i = 0, j = -1;
        //int res = 0;
        //while (i < nums.size()) {
            //if (nums[j + 1] == 1)
                //j ++;
            //else {
                //res = max(res, j - i + 1);
                //i = ++j + 1;
            //}
        //}
        //return res;
    //}
//};

//class Solution {
//private:
    //TreeNode* copyTree(TreeNode *root) {
        //if (!root)
            //return nullptr;
        
        //TreeNode *newroot = new TreeNode(root->val);
        //newroot->left = copyTree(root->left);
        //newroot->right = copyTree(root->right);
        //return newroot;
    //}

//public:
    //vector<TreeNode*> generateTrees(int n) {
        //if (n < 1)
            //return vector<TreeNode*>{};
        //if (n == 1)
            //return vector<TreeNode*>{new TreeNode(n)};

        //auto treeSet = generateTrees(n - 1);
        //vector<TreeNode*> res;
        //for (auto &subroot : treeSet) {
            //TreeNode *root = new TreeNode(n);
            //root->left = subroot;
            //res.push_back(root);
            //auto ptr = subroot;
            //while (ptr) {
                //TreeNode *node = new TreeNode(n);
                //TreeNode *root = copyTree(subroot); // 将子树复制一遍.
                //auto ptr2 = root;
                //while (ptr2->val != ptr->val)// 使ptr2移动到和 ptr 对应的位置
                    //ptr2 = ptr2->right;
                //auto temp = ptr2->right;
                //ptr2->right = node;
                //node->left = temp;
                //res.push_back(root);
                //ptr = ptr->right;
            //}
        //}
        //return res;
    //}
//};

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


    int arr[] = {-1, -1, -1, -1, -1, 0};
    //int arr[] = {1};
    //auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
    //vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    //auto res = Solution().quickSort(nums);
    //vector<vector<int>> grid = {{1, 1, 1}, {0, 1, 0},
                                //{1, 1, 1}};
    //vector<vector<int>> grid = {{0}, {1}};
    //vector<vector<char>> grid = {{'X','X','X','X'},
                                 //{'X','O','O','X'},
                                 //{'X','O','O','X'},
                                 //{'X','O','X','X'}};
    //vector<vector<char>> grid = {{'O','O','O','O',
                                 //'O','O','O','O'}};
    //Solution().solve(grid);
    //vector<string> str = {"ab", "ac", "a", "c", "b"};
    //string str = "Flag";
    int N = 4;
    //vector<vector<int>> nums = {{83}, {64}, {2}};
    vector<int> nums = {1, 2, 3};
    string s = "Let's take LeetCode contest";
    auto res = Solution().subsets(nums);
    //cout << res << endl;
    //cout << std::boolalpha << res << endl;
    //preOrder(root);
    //cout << endl;
    //cout << res << endl;

    //for (auto &d : grid) {
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
