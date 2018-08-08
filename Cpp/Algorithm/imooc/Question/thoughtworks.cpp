#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <exception>
#include <sstream>

using namespace std;

struct cellhash {
public:
    // don't forget `const`
    std::size_t operator()(const pair<int, int> &p) const {
        auto h1 = std::hash<int>{}(p.first);
        auto h2 = std::hash<int>{}(p.second);
        return h1 ^ h2;
    }
};

inline bool isNumeric(const string &s) {
    return (s.find_first_not_of("0123456789") == string::npos);
}

class Maze {
private:
    using Proj = pair<int, int>;
    int m, n;
    vector<vector<string>> mat;
    unordered_map<Proj, Proj, cellhash> record;
    vector<vector<pair<int, int>>> relation;

public:
    Maze(int rows = 1, int cols = 1, const vector<vector<pair<int, int>>> &connect = {}) {
        if (rows <= 0 || rows > INT32_MAX || cols <= 0 || cols > INT32_MAX)
            throw invalid_argument("Number out of range.");
        m = 2 * rows + 1, n = 2 * cols + 1;
        int idx = 0;
        mat = vector<vector<string>>(m, vector<string>(n));
        relation = connect;
        for (auto &pairs : relation)
            if (!valid(pairs))
                throw invalid_argument("Maze format error.");
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if ((i % 2) && (j % 2)) {
                    mat[i][j] = "[R]";
                    record.insert({{idx / cols, idx % cols}, {i, j}});
                    idx ++;
                }
                else mat[i][j] = "[W]";
            }
        }
    }
    string Render() {
        for (auto &pairs : relation) {
            auto p = pairs[0], q = pairs[1];
            p = record[p];
            q = record[q];
            auto px = int((p.first + q.first) / 2), py = int((p.second + q.second) / 2);
            mat[px][py] = "[R]";
        }
        string res;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                res += mat[i][j];
                if (i != m - 1 && j == n - 1) res += "\n";
                else res += " ";
            }
        }
        return res;
    }
private:
    bool valid(vector<pair<int, int>> &pairs) {
        if (pairs.empty()) return true;
        if (pairs.size() != 2) return false;
        auto p = pairs[0], q = pairs[1];
        if (p.first == q.first && abs(p.second - q.second) == 1)
            return true;
        if (p.second == q.second && abs(p.first - q.first) == 1)
            return true;
        return false;
    }
};

class MazeFactory {
public:
    static Maze* Create(const string &command) {
        if (command.empty() || command[0] == ' ')
            throw invalid_argument("Incorrect command format.");
        stringstream ss(command);
        string line;
        int rows, cols;
        vector<vector<pair<int, int>>> connect;
        bool processing_1 = true;
        while (getline(ss, line, '\n')) {
            if (processing_1) {
                if (line.empty() || line[0] == ' ' || !isdigit(line.back()))
                    throw invalid_argument("Incorrect command format.");
                stringstream lss(line);
                string w1, w2;
                lss >> w1 >> w2;
                if (w1.empty() || w2.empty())
                    throw invalid_argument("Incorrect command format.");
                if (!isNumeric(w1) || !isNumeric(w2))
                    throw invalid_argument("Invalid number format.");
                try {
                    rows = stoi(w1);
                    cols = stoi(w2);
                }catch(exception e) {
                    throw invalid_argument("Invalid number format.");
                }
                if (!lss.eof()) throw invalid_argument("Incorrect command format.");
                processing_1 = false;
            }
            else {
                if (line.empty() || !isdigit(line.back()))
                    throw invalid_argument("Incorrect command format.");
                stringstream lss(line);
                // processing ';'
                string pairs;
                while (getline(lss, pairs, ';')) {
                    if (pairs.empty() || pairs[0] == ' ')
                        throw invalid_argument("Incorrect command format.");
                    stringstream pss(pairs);
                    string cell;
                    vector<pair<int, int>> link;
                    while (pss >> cell) {
                        int row, col;
                        auto pos = cell.find_first_of(',');
                        if (pos == string::npos) throw invalid_argument("Incorrect command format.");
                        if (!isNumeric(cell.substr(0, pos)) || !isNumeric(cell.substr(pos + 1, cell.size())))
                            throw invalid_argument("Invalid number format.");
                        try {
                            row = stoi(cell.substr(0, pos));
                            col = stoi(cell.substr(pos + 1, cell.size()));
                        }catch(exception e) {
                            throw invalid_argument("Invalid number format.");
                        }
                        if (row < 0 || row >= rows || col < 0 || col >= cols)
                            throw invalid_argument("Number out of range.");
                        link.push_back({row, col});
                        if (link.size() > 2) throw invalid_argument("Incorrect command format.");
                    }
                    connect.push_back(link);
                }

                if (ss.peek() != EOF) 
                    throw invalid_argument("Incorrect command format.");
                return new Maze(rows, cols, connect);
            }
        }
        throw invalid_argument("Incorrect command format.");
    }
};

int main() {

    string command, line;
    while (getline(cin, line)) {
        command += line + "\n";
    }
    Maze *maze = MazeFactory::Create(command);
    auto res = maze->Render();
    cout << res << endl;
    delete maze;
}
