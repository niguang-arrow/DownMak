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

class Solution {
private:
    int partition(vector<int> &nums, int start, int end) {
        if (nums.empty() || start > end)
            return -1;

        // include <cstdlib>
        int ridx = std::rand() % (end - start + 1) + start; 
        swap(nums[ridx], nums[start]);
        int v = nums[start];

        // nums[start+1...lt] < v
        // nums[lt+1....gt) == v
        // nums[gt...end] > v
        int lt = start, gt = end + 1, i = start + 1;
        while (i < gt) {
            if (nums[i] == v)
                i ++;
            else if (nums[i] < v)
                swap(nums[++lt], nums[i++]);
            else
                swap(nums[--gt],nums[i]);
        }
        swap(nums[start], nums[lt]);
        return lt;
    }
    void quickSort(vector<int> &nums, int start, int end) {
        if (nums.empty() || start >= end)
            return;
        int index = partition(nums, start, end);
        quickSort(nums, start, index - 1);
        quickSort(nums, index + 1, end);
    }
public:
    void quickSort(vector<int>& nums) {
        quickSort(nums, 0, nums.size() - 1);
    }
};

int main() {
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    //root->left->right->left = new TreeNode(7);
    //root->left->right->right = new TreeNode(9);
    //root->right->right = new TreeNode(17);

    //TreeNode *root = new TreeNode(0);
    //root->left = new TreeNode(1);
    //root->right = new TreeNode(1);


    int arr[] = {1, 0, -1, 0, 2, -2};
    //int arr[] = {1};
    //auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));
    vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    //auto res = Solution().quickSort(nums);
    Solution().quickSort(nums);
    //cout << res << endl;
    //for (auto &d : res) {
        //for (auto & data : d)
            //cout << data << " ";
        //cout << endl;
    //}
    //printLinkedList(res);
    //cout << res << endl;

    for (auto &d : nums)
        cout << d << " ";
    cout << endl;
    //cout << std::boolalpha << res << endl;

}
