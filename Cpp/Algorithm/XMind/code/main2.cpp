#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
using namespace std;


int gridSum(vector<vector<int>> &grid,
        vector<vector<int>> &subgrid,
        int r, int c) {
    int sum = 0;
    for (int i = 0; i < subgrid.size(); ++i) {
        for (int j = 0; j < subgrid[0].size(); ++j) {
            sum += (subgrid[i][j] - grid[r + i][c + j]) *
                (subgrid[i][j] - grid[r + i][c + j]);
        }
    }
    return sum;
}


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    string line;
    int R, C;
    getline(cin, line);
    stringstream ss(line);
    ss >> R >> C;
    int a;
    int k = 0;
    vector<vector<int>> grid(R, vector<int>(C));
    while (getline(cin, line)) {
        ss.clear();
        ss.str(line);
        while (ss >> a) {
            grid[k / C][k % C] = a;
            ++k;
        }
        if (k / C >= R)
            break;
    }
    int H, W;
    getline(cin, line);
    ss.clear();
    ss.str(line);
    ss >> H >> W;
    vector<vector<int>> subgrid(H, vector<int>(W));
    k = 0;
    while (getline(cin, line)) {
        ss.clear();
        ss.str(line);
        while (ss >> a) {
            subgrid[k / W][k % W] = a;
            ++k;
        }
        if (k / W >= H)
            break;
    }


    int imax = INT32_MAX, idx = 0, idy = 0;
    for (int r = 0; r < R - H + 1; ++r) {
        for (int c = 0; c < C - W + 1; ++c) {
            int sum = gridSum(grid, subgrid, r, c);
            if (sum < imax) {
                imax = sum;
                idx = r + 1;
                idy = c + 1;
            }
        }
    }
    //return imax;

    cout << imax << endl;
    cout << idx << " " << idy << endl;

    //for (auto &data : grid) {
        //for (auto &d : data)
            //cout << d << " ";
        //cout << endl;
    //}
    //cout << endl;
    return 0;
}

//#include <iostream>
//#include <memory>

//using namespace std;


//class Data {
//private:
    //int data;
//public:
    //Data(int x = 0) { data = x; }
    //~Data() {}
    //Data(const Data &d) { data = d.data; }
    //void print() const { cout << "Data is " << data << endl; }
//};


//unique_ptr<Data> make_data() {
    //return unique_ptr<Data>(new Data(100));
//}

//int main() {

    //unique_ptr<Data> pdata(new Data(10));
    //unique_ptr<Data> pmove(make_data());

    //pmove->print();
//}
