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
public:
    int findPairs(vector<int>& nums, int k) {
        unordered_map<int, int> record;
        for (auto &d : nums) {
            auto iter = record.find(d);
            if (iter != record.end())
                iter->second ++;
            else
                record.insert(make_pair(d, 1));
        }

        int count = 0;
        if (k == 0) {
            for (auto &elem : record) {
                if (elem.second > 1)
                    count ++;
            }
        }
        else {
            for (auto &elem : record) {
                auto iter = record.find(elem.first + k);
                if (iter != record.end()) {
                    count ++;
                }
            }
        }

        return count;
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



    vector<int> nums = {1, 3, 4, 5, 1};
    auto res = Solution().findPairs(nums, 2);
    
    cout << res << endl;

    //for (auto &d : res)
        //cout << d << " ";
    //cout << endl;
    //cout << std::boolalpha << res << endl;

}
