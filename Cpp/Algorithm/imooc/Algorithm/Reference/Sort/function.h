#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>

using namespace std;

// 产生一个随机的数组, 数组元素的范围有 min 和 max 控制
template <typename T>
void generate_array(T arr[], size_t len, T min = 0, T max = 10) {

    static default_random_engine e(1234);
    static uniform_int_distribution<int> uniform(min, max);

    T *parr = arr;
    for (size_t i = 0; i != len; ++i)
        *parr++ = uniform(e);
}

// 打印数组
template <typename T>
void print_array(T arr[], size_t len) {
    T *parr = arr;
    for (size_t i = 0; i != len; ++i)
        cout << *parr++ << " ";
    cout << endl;
}

// 确认数组是否已经排序好, 如果没排序好, 通知你 Mission failed!
template <typename T>
void confirm_sorted(T arr[], size_t len) {
    for (size_t i = 0; i != len - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            cout << "Mission failed!" << endl;
            return;
        }
    }
    cout << "Congratulations! Mission accomplished!" << endl;
}

/**
* 测试排序算法, 参数分别是:
* arr : 数组指针
* len : 数组长度
* SortName : 排序算法名称, 可以是 C 风格字符串, 比如 "Bubble Sort"
* sort_alg : 排序算法函数
* 该函数会随机生成数组, 然后给数组排序, 并判断数组是否排序成功, 
* 同时输出排序花费时间, 当数组的长度太长的话, 就不打印出来, 如果要
* 打印, 数组长度应该小于 20.
*/
template <typename T>
void test_sort(T arr[], size_t len, string SortName, void (*sort_alg)(T[], size_t), T min = 0, T max = 10) {
    generate_array(arr, len, min, max);
    if (len < 20) {
        cout << "Before Sorting: ";
        print_array(arr, len);
    }
    clock_t t = clock();
    sort_alg(arr, len);
    t = clock() - t;
    if (len < 20) {
        cout << "After  Sorting: ";
        print_array(arr, len);
    }
    confirm_sorted(arr, len);
    cout << SortName << ":\n\t" << len << " elements\n\t"
        << "spend " << setprecision(4) << (float)t / CLOCKS_PER_SEC << "s" 
        << endl;
}
