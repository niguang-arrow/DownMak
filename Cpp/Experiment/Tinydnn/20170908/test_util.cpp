#include "util.h"
#include <vector>
#include <iterator>
#include <algorithm>

using namespace tiny_cnn;

int main() {

    for (auto i = 0; i != 10; ++i)
        std::cout << uniform_rand(1, 10) << " ";
    std::cout << std::endl;

    for (auto i = 0; i != 10; ++i)
        std::cout << uniform_rand(1.2, 10.3) << " ";
    std::cout << std::endl;

    for (auto i = 0; i != 10; ++i)
        std::cout << gaussian_rand(1.0, 1.0) << " ";
    std::cout << std::endl;

    std::ostream_iterator<int> scout(std::cout, " ");
    std::vector<int> vec(10);
    uniform_rand(vec.begin(), vec.end(), -5, 5);
    std::copy(vec.begin(), vec.end(), scout);
    std::cout << std::endl;

}
