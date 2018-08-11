#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>


using namespace std;

string nextPermutation(string &s) {
    int i, j, n = s.size();
    for (i = n - 2; i >= 0; --i) {
        if (s[i + 1] >= s[i]) {
            for (j = n - 1; j > i; --j) {
                if (s[j] >= s[i]) break;
            }
            swap(s[i], s[j]);
            reverse(s.begin() + i + 1, s.end());
            return s;
        }
    }
    reverse(s.begin(), s.end());
    return s;
}

int main() {
    string line;
    getline(cin, line);
    stringstream ss(line);
    int n, m, k;
    ss >> n >> m >> k;
    string input = string('a', n) + string('z', m);
    cout << m << n << k << endl;
    for (int i = 0; i < k; ++i)
        input = nextPermutation(input);
    cout << input << endl;
}
