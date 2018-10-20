#include "function.h"

using namespace std;

class Solution {
public:
    bool verify(vector<int> seq) {
        if (seq.empty()) return false;
        return verify(seq, 0, seq.size(), - 1, INT32_MIN, INT32_MAX);
    }
private:
    bool verify(vector<int> &nums, int i, int j, int imin, int imax) {
        if (i > j) return true;
        if (!(imin < nums[i] && nums[i] < imax)) return false;
        int idx = std::lower_bound(nums.begin() + i, nums.begin() + j, nums[j]) - nums.begin();
        if (idx == j)
    }
};


int main() {
    vector<int> nums = {6, -3, 2, 2, 2};
    auto res = Solution().morethanhalf(nums);
    cout << res << endl;
    //printMatrix(res);
    //cout << std::boolalpha << res << endl;
}
