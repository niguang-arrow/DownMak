#include <iostream>
#include <thread>


using namespace std;


void function_1() {
    cout << "Beauty is only" << endl;
}

int main() {
    thread t1(function_1);

        for (int i = 0; i < 100; ++i)
            cout << "from main: ... " << i << endl;
    //try {
        //for (int i = 0; i < 100; ++i)
            //cout << "from main: ... " << i << endl;
    //} catch (...) {
        //t1.join();
        //throw;
    //}
    t1.join();

    //t1.join();
    return 0;
}
