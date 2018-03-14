// for leetcode-cpp
#include <iostream>
#include <vector>

using namespace std;

int removeDuplicates(vector<int> &nums) {
    if (nums.size() <= 2)
        return nums.size();

    int k = 1;
    for (int i = 2; i < nums.size(); ++i) {
        if (nums[i] != nums[k - 1]) {
            nums[++k] = nums[i];
        }
    }
    nums.resize(k + 1);
    for (auto a : nums)
        cout << a << " ";
    cout << endl;
    return k + 1;
}

int main() {

    vector<int> array = {1, 2, 2, 2, 2, 3, 3, 4, 5};
    cout << removeDuplicates(array) << endl;
    return 0;
}
