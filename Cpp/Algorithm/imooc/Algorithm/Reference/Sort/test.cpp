#include <iostream>
#include <vector>


using namespace std;

void moveZeroes(vector<int>& nums) {
    int len = nums.size();
    int i = 0;
    while (i < len) {
        if (nums[i] == 0) {
            for (int j = i + 1; j < len; ++j) {
                nums[j - 1] = nums[j];
            }
            nums[len - 1] = 0;
            --len;
        }
        else
            ++i;
    }
}


void moveZeroes2(vector<int> &nums) {
    int len = nums.size();
    int j = 0;
    for (int i = 0; i < len; ++i) {
        if (nums[i]) {
            nums[j] = nums[i];
            ++j;
        }
    }
    for (int k = j; k < len; ++k)
        nums[k] = 0;
}

void moveZeroes3(vector<int> &nums) {
    int len = nums.size();
    for (int i = 0, j = 0; i < len; ++i) {
        if (nums[i]) {
            swap(nums[i], nums[j++]);
        }
    }
}

int main() {
    //vector<int> vec{0, 1, 3, 0, 3};
    vector<int> vec{1, 2};
    moveZeroes3(vec);
    for (const auto &a : vec)
        cout << a << " ";
    cout << endl;


    return 0;
}

