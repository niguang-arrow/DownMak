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


using namespace std;

//class Solution {
//private:
    //bool contain(BinaryTreeNode<int> *root, int target) {
        //if (!root)
            //return false;

        //if (root->val == target)
            //return true;
        //else if (root->val < target)
            //return contain(root->right, target);
        //else
            //return contain(root->left, target);
    //}
//public:
    //bool findTarget(BinaryTreeNode<int> *root, int k) {
        //if (!root)
            //return false;


    //}
//};
//

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

//class Solution {
//private:
    //bool OnALine(const Point &p, const Point &q) {
        //return !((p.x * q.y) - (p.y * q.x));
    //}
    //struct PointHash
    //{
        //size_t operator()(const Point& rhs) const{
            //return hash<int>()(rhs.x * rhs.x + rhs.y * rhs.y);
        //}
    //};
    //struct PointCmp
    //{
        //bool operator()(const Point& lhs, const Point& rhs) const{
            //return lhs.x == rhs.x && lhs.y == rhs.y;
        //}
    //};
//public:
    //int maxPoints(vector<Point>& points) {
        //if (points.empty())
            //return 0;

        //unordered_map<Point, int, PointHash, PointCmp> pfreq;
        //for (const auto &p : points) {
            //for (auto iter = pfreq.begin(); iter != pfreq.end(); ++iter) {
                //if (OnALine(iter->first, p))
                    //iter->second ++;
            //}
            //pfreq.insert(make_pair(p, 1));
        //}

        //int res = 0;
        //for (const auto &member : pfreq)
            //res = max(res, member.second);
        //return res;
    //}
//};

//class Solution {
//public:
    //bool isPalindrome(int x) {
        //if (x < 0)
            //return false;

        //unsigned int l = 0x80000000;
        //unsigned int r = 0x1;
        //while (l != r) {
            //if (((l & x)) != (r & x))
                //return false;
            //l >>= 1;
            //r <<= 1;
        //}
        //return true;
    //}
//};

class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        vector<int> res;
        if (nums.empty())
            return res;

        int l = 0, r = nums.size() - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] >= target)
                r = mid - 1;
            else
                l = mid + 1;
        }
        cout << r << endl;

        if (r < 0)
            res.push_back(-1);
        else
            res.push_back(r + 1);


        l = 0;
        r = nums.size() - 1;

        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] <= target)
                l = mid + 1;
            else
                r = mid - 1;
        }

        if (l >= nums.size())
            res.push_back(-1);
        else
            res.push_back(l - 1);


        return res;
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


    //string st1 = "anagram", st2 = "nagaram";
    //int digit = 19;

    //Point p1(0, 0), p2(1, 1), p3(2, 2), p4(1, 0);
    //vector<Point> nums = {p1, p2, p3, p4};
    //auto res = Solution().maxPoints(nums);
    //cout << res << endl;

    vector<int> nums = {5, 7, 7, 8, 8};
    auto res = Solution().searchRange(nums, 8);


    //cout << res << endl;

    for (auto &d : res)
        cout << d << " ";
    cout << endl;
    //cout << std::boolalpha << res << endl;

}
