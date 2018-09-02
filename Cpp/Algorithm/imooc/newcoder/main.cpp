#include "function.h"

using namespace std;

class Solution {
public:
    int morethanhalf(vector<int> nums) {
        if (nums.empty()) return 0;
        int vote = nums[0], count = 1;
        for (int i = 1; i < nums.size(); ++i) {
            if (count == 0) {
                vote = nums[i];
                count ++;
            }
            if (vote == nums[i]) count ++;
            else count --;
        }
        count = 0;
        for (auto &d : nums)
            if (vote == d) count ++;
        if (count > (nums.size() / 2)) return vote;
        return 0;
    }
};


int main() {
    vector<int> nums = {6, -3, 2, 2, 2};
    auto res = Solution().morethanhalf(nums);
    cout << res << endl;
    //printMatrix(res);
    //cout << std::boolalpha << res << endl;
}
