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


using namespace std;

int CountPair(vector<int> &nums, int k) {
    unordered_map<int, int> record;
    for (auto &d : nums) {
        auto iter = record.find(d);
        if (iter != record.end())
            iter->second ++;
        else
            record.insert(make_pair(d, 1));
    }

    int count = 0;
    for (auto &elem : record) {
        auto iter = record.find(elem.first + k);
        if (iter != record.end()) {
            if (elem.first + k == iter->first && iter->second < 2)
                continue;
            count += 1;
            iter->second --;
        }
    }
            
    return count;
}


int Times(int n) {
    if (n <= 0)
        return 0;

    if (n <= 3)
        return n - 1;

    if ((n & 0x1))
        return n - 1;

    if (n % 3 == 0)
        return n / 3 + 1;
    
    if ((n & 0x1) == 0)
        return n / 2;
    return 0;

}


vector<vector<string>> Result(long long eq) {
    string num = to_string(eq);
    cout << num << endl;

    vector<vector<string>> res(num.size(), vector<string>());
    for (int i = 0; i < num.size(); ++i) {
        switch (num[i]) {
            case '0':
                res[i].push_back(string("66666"));
                res[i].push_back(string("6...6"));
                res[i].push_back(string("6...6"));
                res[i].push_back(string("6...6"));
                res[i].push_back(string("66666"));
                break;
            case '1':
                res[i].push_back(string("....6"));
                res[i].push_back(string("....6"));
                res[i].push_back(string("....6"));
                res[i].push_back(string("....6"));
                res[i].push_back(string("....6"));
                break;
            case '2':
                res[i].push_back(string("66666"));
                res[i].push_back(string("....6"));
                res[i].push_back(string("66666"));
                res[i].push_back(string("6...."));
                res[i].push_back(string("66666"));
                break;
            case '3':
                res[i].push_back(string("66666"));
                res[i].push_back(string("....6"));
                res[i].push_back(string("66666"));
                res[i].push_back(string("....6"));
                res[i].push_back(string("66666"));
                break;
            case '4':
                res[i].push_back(string("6...6"));
                res[i].push_back(string("6...6"));
                res[i].push_back(string("66666"));
                res[i].push_back(string("....6"));
                res[i].push_back(string("....6"));
                break;
            case '5':
                res[i].push_back(string("66666"));
                res[i].push_back(string("6...."));
                res[i].push_back(string("66666"));
                res[i].push_back(string("....6"));
                res[i].push_back(string("66666"));
                break;
            case '6':
                res[i].push_back(string("66666"));
                res[i].push_back(string("6...."));
                res[i].push_back(string("66666"));
                res[i].push_back(string("6...6"));
                res[i].push_back(string("66666"));
                break;
            case '7':
                res[i].push_back(string("66666"));
                res[i].push_back(string("....6"));
                res[i].push_back(string("....6"));
                res[i].push_back(string("....6"));
                res[i].push_back(string("....6"));
                break;
            case '8':
                res[i].push_back(string("66666"));
                res[i].push_back(string("6...6"));
                res[i].push_back(string("66666"));
                res[i].push_back(string("6...6"));
                res[i].push_back(string("66666"));
                break;
            case '9':
                res[i].push_back(string("66666"));
                res[i].push_back(string("6...6"));
                res[i].push_back(string("66666"));
                res[i].push_back(string("....6"));
                res[i].push_back(string("66666"));
                break;
        }
    }
    return res;
}

long long parse(string equation) {
    long long res = 0;
    int sign = 1;
    for (auto &d : equation) {
        if (isalnum(d))
            res += sign * d;
        else if (d == '-')
            sign = -1;
        else if (d == '+')
            sign = 1;
    }
    return res;
}

int main() {

    //int arr[] = {1, 1, 1, 1};
    //vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    //cout << CountPair(nums, 0) << endl;
    
    //int len, k;
    //cin >> len >> k;

    //int arr[len];
    //for (int i = 0; i < len; ++i)
        //cin >> arr[i];

    int arr[] = {1, 3, 5, 1, 4};
    vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));

    cout << CountPair(nums, 0) << endl;


    //int n;
    //cin >> n;
    //int count = 0;
    //string equation;
    //vector<vector<vector<string>>> res(n);
    //while (count < n) {
        //getline(cin, equation);
        //long long num = parse(equation);
        //res.push_back(Result(num));
        //count++;
    //}

    //for (auto &str : res) {
        //for (int i = 0; i < 5; ++i) {
            //for (int j = 0; j < str.size(); ++j) {
                //if (j == str.size() - 1)
                    //cout << str[j][i] << endl;
                //else
                    //cout << str[j][i] << "..";
            //}
        //}
        //cout << endl;
    //}
}
