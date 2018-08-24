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
#include <list>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <utility>
#include <cmath>


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
    bool VerifySquenceOfBST(vector<int> sequence) {
        if (sequence.empty()) return false;
        return VerifySquenceOfBST(sequence, 0, sequence.size() - 1, INT32_MIN, INT32_MAX);
    }
private:
    bool VerifySquenceOfBST(vector<int> &nums, int i, int j, int imin, int imax) {
        if (i > j) return true;
        if (!(imin < nums[j] && nums[j] < imax)) return false;
        int idx = binarySearch(nums, i, j - 1, nums[j]);
        cout << idx << endl; 
        if (idx == -1) return VerifySquenceOfBST(nums, i, j - 1, imin, nums[j]);
        return VerifySquenceOfBST(nums, i, idx, imin, nums[j]) &&
            VerifySquenceOfBST(nums, idx + 1, j - 1, nums[j], imax);
    }

    int binarySearch(vector<int> &nums, int l, int r, int target) {
        if (l > r) return -1;
        int end = r;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] < target) l = mid + 1;
            else r = mid - 1;
        }
        return l > end ? -1 : l - 1;
    }

};



//class Solution {
//public:
    //ListNode* FindKthToTail(ListNode* head, unsigned int k) {
        //if (!head || k == 0) return nullptr;
        //auto ps = head, pe = head;
        //for (int i = 0; i < k - 1; ++i) {
            //if (pe->next)
                //pe = pe->next;
            //else
                //return nullptr;
        //}
        //cout << pe->val << endl;
        //while (pe->next) {
            //ps = ps->next;
            //pe = pe->next;
        //}
        //return ps;
    //}
//};


int main() {

    vector<int> nums = {5, 4, 3, 2, 1};

    int arr[] = {1, 2,3,  4, 5};
    auto ls = createLinkedList(arr, sizeof(arr)/sizeof(int));

    //Solution().reOrderArray(nums);
    //printVector(nums);
    //auto res = Solution().FindKthToTail(ls, 5);

    auto res = Solution().VerifySquenceOfBST(nums);
    cout << std::boolalpha << res << endl;
    //printLinkedList(res);
}
