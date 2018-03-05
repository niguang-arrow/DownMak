#include <iostream>
#include <vector>
using namespace std;



int find_onebyone(const vector<int> &vec) {
    int min = INT32_MAX;
    if (!vec.empty()) {
       for (int i = 1; i != vec.size(); ++i) {
           if (min > vec[i])
               min = vec[i];
       }
    }
    return min;
}

int find_part(const vector<int> &vec) {
    int min = INT32_MAX;
    //比如 {12, 13, 14, 0, 1, 2, 3, 4}, l 最多索引到 14 的位置, 而
    //r 最多索引到 0 的位置, 所以在 while 循环进行到最极端的情况, 应该
    //是 l + 1 == r
    if (!vec.empty()) {
        int l = 0, r = vec.size() - 1;
        while (vec[l] > vec[r]) {
            if (l + 1 == r)
                return vec[r];
            int mid = l + (r - l) / 2;
            if (vec[mid] >= vec[l])
                l = mid;
            else
                r = mid;
        }
    }
    return min;
}

int main() {

    vector<int> vec = {12, 13, 14, 0, 1, 2, 3, 4};
    cout << find_onebyone(vec) << endl;
    cout << find_part(vec) << endl;
}
