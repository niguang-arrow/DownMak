#include <unordered_map>
#include <iostream>


using namespace std;


int main() {

    unordered_map<int, string> map;
    map[10] = "a";
    map[20] = "b";

    unordered_map<string, int> smap;
    smap.insert(std::make_pair("a", 1));
    smap.insert(std::make_pair("b", 2));

    auto it = smap.find("a");
    cout << it->second << endl;
}
