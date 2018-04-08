// move semantics
#include <iostream>
#include <initializer_list>

using namespace std;


class Data {
public:
    int *data;
    int count;
    Data(int c) : data(new int[c]), count(c) {}
    Data(initializer_list<int> ls) : data(new int[ls.size()]), count(ls.size()) {
        int i = 0;
        for (auto &d : ls)
            data[i++] = d;
    }
    ~Data() { delete[] data; }
    Data(const Data &comp) : data(new int[comp.count]), count(comp.count) {
        for (int i = 0; i < count; ++i)
            data[i] = comp.data[i];
    }
    Data(Data &&rhs) : data(rhs.data), count(rhs.count) {
        rhs.data = nullptr;
    }
};


int main() {
    
    Data d({10, 20, 30});
    cout << "before: " << d.data << endl;
    Data a = std::move(d);
    cout << "a.data: " << a.data << endl;
    cout << d.count << endl;
    cout << "d.data: " << d.data << endl;
}
