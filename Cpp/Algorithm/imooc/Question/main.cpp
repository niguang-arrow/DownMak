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


class Solution {
public:
    int findNumberOfLIS(vector<int>& nums) {
        if (nums.empty())
            return 0;
        int n = nums.size();
        vector<int> dp(n+1, 0);
        unordered_map<int, int> record;
        dp[0] = 1;

        for (int i = 1; i < n; ++i) {
            int k = i - 1;
            while (k >= 0) {
                if (nums[k] < nums[i]) {
                    dp[i] = max(dp[i],  dp[k] + 1);
                    record[dp[i]] ++;
                }
                k --;
            }
            if (k < 0)
                dp[i] = 1;
            cout << "i: " << i << " dp:" << dp[i] << endl;
        }

        std::sort(dp.begin(), dp.end());
        int res = 1;
        for (int i = n - 1; i >= 0; --i)
            if (dp[i] == dp[n])
                res ++;
        return res;
    }
};

////674. Longest Continuous Increasing Subsequence
//class Solution {
//public:
    //int findLengthOfLCIS(vector<int>& nums) {
        //if (nums.empty())
            //return 0;
        //int i = 0;
        //int res = 1;
        //// nums[i...j) 保存最长的连续递增子串
        //for (int j = 1; j < nums.size(); ++j) {
            //if (nums[j] <= nums[j - 1]) {
                //i = j;
            //}
            //res = max(res, j - i + 1);
        //}
        //return res;
    //}
//};

int main() {

    vector<int> nums = {1, 3, 5, 4, 7};
    auto res = Solution().findNumberOfLIS(nums);
    cout << res << endl;
    return 0;
}
