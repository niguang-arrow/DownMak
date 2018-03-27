#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>

using namespace std;

vector<string> appropriate(vector<pair<int, vector<string>>> &nums, int target) {
    int min = INT32_MAX;
    vector<string> res;
    for (auto &time : nums) {
        if (time.first <= target && time.first < min)
            res = time.second;
    }
    return res;
}

int main() {

    string N;
    getline(cin, N);
    string getup_time, classtime, hour, minute;
    int time;
    int classclock;
    vector<pair<int, vector<string>>> clock;
    for (int i = 0; i < stoi(N); ++i) {
        getline(cin, getup_time);
        istringstream ss(getup_time);
        ss >> hour >> minute;
        time = std::stoi(hour) * 3600 + std::stoi(minute) * 60;
        clock.push_back(std::make_pair(time, vector<string>{hour, minute}));
    }

    string X;
    int delay;
    getline(cin, X);
    delay = stoi(X) * 60;
    getline(cin, classtime);
    istringstream ss(classtime);
    ss >> hour >> minute;
    classclock = std::stoi(hour) * 3600 + std::stoi(minute) * 60;
    int target = classclock - delay;
    vector<string> approp = appropriate(clock, target);
    cout << approp[0] << " " << approp[1] << endl;
    return 0;
}
