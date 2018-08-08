#include <iostream>
#include <sstream>
#include <string>

using namespace std;



int main() {
    string command, line;
    while (getline(cin, line)) {
        command += line + "\n";
    }
    stringstream ss(command);
    while (!ss.eof() && ss.peek() != EOF) {
        getline(ss, line, '\n');
        cout << "l: " << line << endl;
        if (!ss) cout << "end" << endl;
        else cout << "not" << endl;
    }
    if (ss.eof()) cout << "out end" << endl;
}
