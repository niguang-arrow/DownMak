#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <queue>
#include <utility>


using namespace std;

void dfs(unordered_map<int, vector<int>> &record, int &res,
        vector<int> &index, int start,
        unordered_set<int> &seen,
        unordered_set<int> &already, int &total) {
    int idx = index[start];
    for (auto &node : record[idx]) {
        cout << "node: " << node << " res: " << res << endl;
        if (seen.count(node)) continue;
        else {
            res += 1;
            seen.insert(node);
            already.insert(node);
            dfs(record, res, seen, already, total);
            if ((int)already.size() == total) return;
            res += 1;
            seen.erase(node);
        }
    }
}


int main() {

    int N;
    string line;
    getline(cin, line);
    N = stoi(line);

    unordered_map<int, vector<int>> record;
    unordered_set<int> nodes;
    for (int i = 1; i < N; ++i) {
        getline(cin, line);
        stringstream ss(line);
        int a, b;
        ss >> a >> b;
        nodes.insert(a);
        nodes.insert(b);
        record[a].push_back(b);
    }
    int total = nodes.size();
    nodes.clear();

    priority_queue<pair<int, int>, vector<pair<int, int>>,
        [] (const pair<int, int> &a, const pair<int, int> &b { return a.second < b.second; })> q;
    vector<int> index;
    for (auto &iter : record)
        q.push({iter.first, iter.second.size()})

    while (!q.empty()) {
        index.push_back(q.top().first);
        q.pop();
    }

    int res = 0;
    unordered_set<int> seen;
    unordered_set<int> already;
    seen.insert(1);
    already.insert(1);

    dfs(record, res, index, 0, seen, already, total);
    cout << res << endl;

}
