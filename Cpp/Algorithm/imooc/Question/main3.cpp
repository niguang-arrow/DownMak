// BOBO : leetcode 
#include <iostream>
#include <vector>
#include "function.h"


using namespace std;

class Solution {
public:
    void sortColors(vector<int>& nums) {
        if (nums.empty())
            return;
        
        // nums[0... lt] 保存 0
        // nums[lt+1....i] 保存 1
        // nums[gt....n-1] 保存 2
        int lt = -1, gt = nums.size();
        int i = 0;
        while (i < gt) {
            if (nums[i] == 1)
                ++i;
            else if (nums[i] == 0)
                swap(nums[i++], nums[++lt]);
            else
                swap(nums[i], nums[--gt]);
        }
    }
};
int main() {
    vector<int> array = {1, 0, 0, 2, 2};
    Solution().sortColors(array);
    for (const auto &d : array)
        cout << d << " ";
    cout << endl;
}
