// for leetcode-cpp
#include <iostream>
#include <vector>

using namespace std;

int BinarySearch(vector<int> &nums, int l, int r, int target) {
    if (nums.empty() || l > r)
        return -1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (nums[mid] == target)
            return mid;

        if (nums[mid] < target)
            l = mid + 1;
        else
            r = mid - 1;
    }

    return -1;
}

int search(vector<int> &nums, int target) {
    if (nums.empty())
        return -1;

    int l = 0, r = nums.size() - 1;

    while (nums[l] > nums[r]) {
        if (l + 1 == r)
            break;
        int mid = l + (r - l) / 2;
        if (nums[l] < nums[mid])
            l = mid;
        else if (nums[mid] < nums[r])
            r = mid;
    }

    int index;
    if (target >= nums[0])
        index = BinarySearch(nums, 0, r - 1, target);
    else
        index = BinarySearch(nums, r, nums.size() - 1, target);
    return index;
}

int main() {

    //vector<int> array = {4, 5, 6, 0, 1, 2, 3};
    vector<int> array = {1, 3};
    cout << search(array, 1) << endl;
    return 0;
}
