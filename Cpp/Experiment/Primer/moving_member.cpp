#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <utility> 

using namespace std;


int main() {

    vector<string> vec;
    vec.push_back("done");
    ostream_iterator<string> scout(cout, " ");
    copy(vec.begin(), vec.end(), scout);
    cout << endl;

    return 0;
}
