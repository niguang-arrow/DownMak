bool exists(vector<int> &nums) {
    if (nums.size() < 3)
        return false;

    int first = INT32_MAX, second = INT32_MIN;
    for (auto now : nums) {
        if (now <= first) {
            first = now;
            continue;
        }

        if (now > second) {
            second = now;
            continue;
        }

        if (now > first && now < second) {
            //if (second == INT32_MAX)
                //continue;
            //cout << first << " " << now << " " << second << endl;
            return true;
        }
    }
    return false;
}
