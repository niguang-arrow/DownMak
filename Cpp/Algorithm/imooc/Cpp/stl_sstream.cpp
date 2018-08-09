#include <iostream>
#include <sstream>
#include <string>

using namespace std;



int main() {
    string command = "This is a test\nSecond Test\n";
    stringstream ss(command);
    string line;
    //while (ss >> line);
    ss >> line;
    char c;
    while (ss.get(c))
        cout << c << " ";
    cout << endl;
    if (ss.eof()) cout << "ss eof" << endl;
    cout << ss.peek() << endl;

    //while (!ss.eof() && ss.peek() != EOF) {
        //getline(ss, line, '\n');
        //cout << "l: " << line << endl;
        //if (!ss) cout << "end" << endl;
        //else cout << "not" << endl;
    //}
    //if (ss.eof()) cout << "out end" << endl;
}
