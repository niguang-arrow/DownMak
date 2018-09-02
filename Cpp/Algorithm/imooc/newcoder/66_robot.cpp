#include "function.h"

using namespace std;

class Solution {
public:
    int movingCount(int threshold, int rows, int cols) {
        int res = 0;
        m = rows, n = cols;
        seen = vector<vector<bool>>(m, vector<bool>(n, false));
        bfs(res, threshold, 0, 0);
        return res;
    }
private:
    int m, n;
    vector<vector<bool>> seen;
    void bfs(int &res, int threshold, int row, int col) {
        queue<pair<
        if (row < 0 || row >= m || col < 0 || col >= n || seen[row][col]
            || greater(threshold, row, col)) return;
        seen[row][col] = true;
        res += 1;
        vector<vector<int>> dirs = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
        for (auto d : dirs) {
            int dx = d[0], dy = d[1];
            dfs(res, threshold, row + dx, col + dy);
        }
    }
    bool greater(int threshold, int i, int j) {
        int sum = 0;
        while (i || j) {
            sum += i % 10 + j % 10;
            i /= 10;
            j /= 10;
        }
        return (sum > threshold);
    }
};
