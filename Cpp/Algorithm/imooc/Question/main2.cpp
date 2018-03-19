// 左成云: 生成窗口的最大值数组
#include <iostream>
#include <vector>
#include <stack>
#include "function.h"
#include <unordered_map>
#include <queue>
#include <cassert>
using namespace std;

int maxElement(vector<int> &nums, int start, int width) {
    int maximum = INT32_MIN;
    if (nums.empty())
        return maximum;

    for (int i = start; i < start + width; ++i)
        maximum = max(nums[i], maximum);
    return maximum;
}

vector<int> maxWindowElement(vector<int> &nums, int w) {
    if (nums.empty())
        return vector<int>();

    int n = nums.size();
    if (n <= w)
        return vector<int>(maxElement(nums, 0, n));
    vector<int> res;
    res.push_back(maxElement(nums, 0, w));
    for (int i = w; i < n; ++i) {
        if (nums[i] >= res.back())
            res.push_back(nums[i]);
        else
            res.push_back(maxElement(nums, i - w + 1, w));
    }
    return res;
}


int main() {
    vector<int> array = {4, 3, 5, 4, 3, 3, 6, 7};
    auto res = maxWindowElement(array, 3);
    for (auto &a : res)
        cout << a << " ";
    cout << endl;

}
