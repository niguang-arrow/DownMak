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
#include <stack>


using namespace std;

void print(vector<vector<int>> &matrix) {
    for (auto &data : matrix) {
        for (auto &d : data) {
            cout << d << " ";
        }
        cout << endl;
    }
}

int main() {
}
    //string line;
    //getline(cin, line);
    //stringstream ss(line);
    //int m, n;
    //ss >> m >> n;
    //vector<vector<int>> matrix(m, vector<int>(n, 0));
    //int k = 0;
    //while (k / n < m) {
        //getline(cin, line);
        //ss.clear();
        //ss.str(line);
        //while (!ss.eof()) {
            //ss >> matrix[k / n][k % n];
            //k ++;
        //}
    //}

    //vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    //print(matrix);
