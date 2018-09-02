#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <initializer_list>
#include <algorithm>
#include <stack>
#include <queue>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <memory>
#include <utility>
#include <deque>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <list>

using namespace std;

void printMatrix(vector<vector<int>> &mat) {
    for (auto &d : mat) {
        for (auto &a : d)
            cout << a << " ";
        cout << endl;
    }
}

#endif
