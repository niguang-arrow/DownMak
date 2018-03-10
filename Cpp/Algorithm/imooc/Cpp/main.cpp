//左成云: 第四章: 矩阵最小路径和
#include <iostream>
#include <vector>

using namespace std;

// 使用辅助的数组 dp, 用于记录到达 (i, j) 位置处的最小路径和
// 时间复杂度和空间复杂度均为 O(M * N)
int minPathSum1(const vector<vector<int>> &mat) {
    if (mat.empty())
        return 0;
    int row = mat.size();
    int col = mat[0].size();
    vector<vector<int>> dp = vector<vector<int>>(row, vector<int>(col, 0));
    dp[0][0] = mat[0][0];

    for (int i = 1; i < col; ++i)
        dp[0][i] = dp[0][i - 1] + mat[0][i];

    for (int i = 1; i < row; ++i)
        dp[i][0] = dp[i - 1][0] + mat[i][0];

    for (int i = 1; i < row; ++i) {
        for (int j = 1; j < col; ++j) {
            dp[i][j] = mat[i][j] + min(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    return dp[row - 1][col - 1];
}

// 使用空间压缩对上面的方法进行改进, 使用一维的大小为
// min{M, N} 的 arr 数组记录达到某一行或某一列(看你怎么扫描)
// 的最短路径和
int minPathSum2(vector<vector<int>> mat) {
    if (mat.empty())
        return 0;

    int row = mat.size();
    int col = mat[0].size();

    vector<int> arr(col, mat[0][0]);
    for (int i = 1; i < col; ++i)
        arr[i] = arr[i - 1] + mat[0][i];

    for (int i = 1; i < row; ++i) {
        arr[0] += mat[i][0];
        for (int j = 1; j < col; ++j) {
            // 当要判断 (i, j) 位置处的最短路径和时, 便要比较
            // (i, j - 1) 与 (i - 1, j) 处的最短路径和, 由于
            // (i, j -1) 的最短路径和已经保存在 arr[j - 1] 中,
            // 而 (i - 1, j) 中的最短路径和还保存在 arr[j] 中,
            // 此时就需要判断 arr[j-1] 与 arr[j] 的最小值, 并
            // 更新 arr[j].
            arr[j] = min(arr[j - 1], arr[j]) + mat[i][j];
        }
    }

    return arr[col - 1];
}

int main() {

    vector<vector<int>> mat = {
        {1, 3, 5, 9},
        {8, 1, 3, 4},
        {5, 0, 6, 1},
        {8, 8, 4, 0}
    };
    cout << minPathSum1(mat) << endl;
    cout << minPathSum2(mat) << endl;
    return 0;
}
