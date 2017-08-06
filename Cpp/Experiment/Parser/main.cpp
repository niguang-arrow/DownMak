#include <iostream>
#include <fstream>

#include "IO.h"

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

using namespace ssql;

int main(int argc, const char *argv[]) {
    //Lexer *lexptr; 
    ofstream out;
    ifstream in;

    if (argc > 1) {
        in.open(argv[1], ifstream::in);
        if (in.is_open()) {
            //lexptr = new Lexer(in);
            cout << "Success" << endl;
        } else {
            cout << "Fail to open " << argv[1] << endl;
            exit(1);
        }
    } else {
        //lexptr = new Lexer(cin);
        cout << "Success" << endl;
    }


    //delete lexptr;
    return 0;
}
