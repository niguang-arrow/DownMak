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
#include <algorithm>
#include <cstdlib>


using namespace std;

int combinations(vector<int> &nums, int start, unordered_map<int, int> &memo, int target) {
    if (memo.find(target) != memo.end())
        return memo[target];
    if (memo[target] != -1)
        return memo[target];

        int res = 0;
        for (int i = start; i < nums.size(); ++i) {
            if (target >= nums[i])
                res += combinations(nums, start, memo, target - nums[i]);
        }
        memo[target] = res;
        return res;
    }
//vector<vector<int>> combinations(vector<int> &array1, int start1, unordered_map<int, int> &memo, int target) {
    ////if (memo.find(target) != memo.end())
        ////return memo[target];

    //vector<vector<int>> res;
    //for (int i = start1; i < array1.size() && array1[i] <= target; ++i) {
            //if (array1[i] == target)
                //res.push_back({array1[i]});
            //auto conditions = combinations(array1, i, memo, target - array1[i]);
            //if (!conditions.empty()) {
                //for (auto &condition : conditions) {
                    //condition.push_back(array1[i]);
                    //res.push_back(condition);
                //}
            //}
        //}
        //return res;
    ////for (int i = start1; i < array1.size() && target >= array1[i]; ++i) {
        ////cout << "i: " << i << " start: " << start1 << " target: " << target << endl;
        ////res += combinations(array1, i, memo, target - array1[i]);
    ////}
    ////memo[target] = res;
    ////return res;
//}

//int combinations(vector<int> &array1, int start1, vector<int> &array2, int start2, unordered_map<int, int> &memo, int target) {
    //if (memo.find(target) != memo.end())
        //return memo[target];

    //int res = 0;
    //for (int i = start1; i < array1.size() && target >= array1[i]; ++i) {
        //cout << "i: " << i << " start: " << start1 << " target: " << target << endl;
        //res += combinations(array1, i, array2, start2, memo, target - array1[i]);
    //}
    ////for (int j = start; j < array2.size(); ++j)
        ////if (target >= array2[j])
            ////res += combinations(array1, array2, start + 1, memo, target - array2[j]);
    //memo[target] = res;
    //return res;
//}

int main() {
    
    int n1, n2, m;
    string line;
    getline(cin, line);
    stringstream ss(line);
    ss >> n1 >> n2 >> m;
    getline(cin, line);
    ss.clear();
    ss.str(line);
    int i = 0, data;
    vector<int> array1(n1), array2(n2);
    while (ss >> data)
        array1[i++] = data;
    getline(cin, line);
    ss.clear();
    ss.str(line);
    i = 0;
    while (ss >> data)
        array2[i++] = data;

    unordered_map<int, int> record;
    record[0] = 1;
    //int res = combinations(array1, 0, array2, 0, record, m);
    auto res = combinations(array1, 0, record, m);
    cout << res << endl;
     //for (auto &d : res) {
        //for (auto &a : d) {
            //cout << a << " ";
        //}
        //cout << endl;
    //}
    return 0;
}
    //for (auto &d : array2)
        //cout << d << endl;
    //return 0;

//bool satisfy(unordered_set<int> &record, vector<int> &nums, int T) {
    //for (int i = 0; i < nums.size(); ++i) {
        //if (nums[i] + T <= nums.back() && record.find(nums[i] + T) == record.end())
            //return false;
        //else if (nums[i] + T > nums.back())
            //break;
    //}
    //for (int i = nums.size() - 1; i >= 0; --i) {
        //if (nums[i] - T >= nums[0] && record.find(nums[i] - T) == record.end())
            //return false;
        //else if (nums[i] - T < nums[0])
            //break;
    //}
    //return true;
//}

//int returnT(vector<int> &nums) {
    //unordered_set<int> record(nums.begin(), nums.end());
    //for (int i = 1; i < nums.size(); ++i) {
        //int T = nums[i] - nums[0];
        //if (satisfy(record, nums, T))
            //return T;
    //}
    //return (nums.back() - nums[0]);

//}

//int main() {
    
    //int N, n;
    //string line;
    //vector<int> res;
    //getline(cin, line);
    //stringstream ss(line);
    //ss >> N;
    //while(N--) {
        //getline(cin, line);
        //ss.clear();
        //ss.str(line);
        //ss >> n;
        //int i = 0, data;
        //vector<int> array(n);
        //while (ss >> data)
            //array[i++] = data;
        
        //res.push_back(returnT(array));
    //}
    //for (auto &d : res)
        //cout << d << endl;
    //return 0;

//}
