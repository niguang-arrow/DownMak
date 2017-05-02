#include <iostream>
using namespace std;

//struct Vector {
    //int sz;
    //double *elem;
//};
//void vector_init(Vector &v, int s)
//{
    //v.elem = new double[s];
    //v.sz = s;
//}

class Vector
{
public:
    Vector(int s) : elem {new double[s]}, sz {s} {}
    double& operator[](int i) { return elem[i]; }
    int size() { return sz; }
private:
    double* elem;
    int sz;

};

double read_and_sum(int s)
{
    Vector v(s);
    cout << "input " << v.size() << "elements: " << endl;
    for (auto i = 0; i != v.size(); ++i)
    {
        cin >> v[i];
    }
    double sum = 0;
    for (auto i = 0; i != v.size(); ++i)
    {
        sum += v[i];
    }
    return sum;
}

int main() 
{
    int s;
    cout << "input the number of elements in Vector: " << endl;
    cin >> s;
    auto sum = read_and_sum(s);
    cout << "The result is " << sum << endl;
    return 0;
}
