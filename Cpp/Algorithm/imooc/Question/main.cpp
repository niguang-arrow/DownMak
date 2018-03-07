#include <iostream>
#include "function.h"
#include <vector>

using namespace std;

class Solution {
private:
    vector<vector<int>> res;
    int mps(const vector<vector<int>> &matrix, int row, int col) {
        if (matrix.empty() || row < 0 || col < 0)
            return INT32_MAX;

        if (row == 0 && col == 0)
            return matrix[row][col];

        if (res[row][col] != 0)
            return res[row][col];

        //vector<vector<int>> res(row + 1, vector<int>(col + 1));
        for (int i = row; i >= 0; --i) {
            for (int j = col; j >= 0; --j) {
                res[i][j] = matrix[i][j] + min(mps(matrix, i - 1, j), mps(matrix, i, j - 1));
                //memo[i][j] = res[i][j];
            }
        }
        //// 可以打印中间过程
        //cout << "res[" << row << "]" << "[" << col << "]: " << res[row][col] << endl;
        return res[row][col];
    }
public:
    int minPathSum(vector<vector<int>>& grid) {
        res.resize(grid.size());
        for (int i = 0; i < grid.size(); ++i)
            res[i].resize(grid[0].size());
        return mps(grid, grid.size() - 1, grid[0].size() - 1);
    }
};


void SortAge(int ages[], int length) {
    if (ages == nullptr || length <= 0)
        return;

    const int oldestAge = 99;
    int timesOfAge[oldestAge + 1];

    for (int i = 0; i < oldestAge + 1; ++i)
        timesOfAge[i] = 0;

    for (int i = 0; i < length; ++i) {
        if (ages[i] < 0 || ages[i] > oldestAge)
            throw "age out of range";
        timesOfAge[ages[i]]++;
    }

    int index = 0;
    for (int i = 0; i <= oldestAge; ++i) {
        for (int j = 0; j < timesOfAge[i]; ++j) {
            ages[index] = i;
            index++;
        }
    }
}


bool test_sorted(int arr[], int length) {
    for (int i = 0; i < length - 1; ++i)
        if (arr[i] > arr[i + 1])
            return false;
    return true;
}


int main() {


    //vector<vector<int>> arr = {{1}};
    //vector<vector<int>> arr = {{1, 3, 1}, {1, 5, 1}};
    //vector<vector<int>> arr = {{1, 2}, {1, 5}};
    //vector<vector<int>> arr = {{1, 3, 1}, {1, 5, 1}, {4, 2, 1}};
    //int res = Solution().minPathSum(arr);
    //cout << res << endl;
    //
    int *a = generateArray(100);
    SortAge(a, 100);
    cout << test_sorted(a, 100) << endl;;
 
    //for (int n = 10; n < 10000; n *= 10) {
        ////int n = 100;
        //int *a = generateArray(n);
        //vector<int> vec(a, a + n);
        //vector<vector<int>> arr = {vec};
        ////vector<vector<int>> arr = {{1, 3, 1}, {1, 5, 1}, {4, 2, 1}};
        ////vector<vector<int>> arr = {{1, 2}, {1, 5}};
        //clock_t start = clock();
        //int res = Solution().minPathSum(arr);
        //clock_t end = clock();
        //cout << res << endl;
        //cout << "time: " << double(end - start) / CLOCKS_PER_SEC << endl;

        //delete[] a;

    //}
    return 0;
}
