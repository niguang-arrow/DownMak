#ifndef MY_FUNCTION_H
#define MY_FUNCTION_H

#include <iostream>
#include <random>
#include <cmath>
#include <ctime>

template <typename T>
struct return_type_chooser;

template <>
struct return_type_chooser<int> {
    using type = int*;
};

template <>
struct return_type_chooser<float> {
    using type = float*;
};

template <>
struct return_type_chooser<double> {
    using type = double*;
};

template <typename T>
typename return_type_chooser<T>::type generate_array(int n) {
    static std::default_random_engine e(1234);
    static std::uniform_int_distribution<T> uni(1.0, 20.0);

    T* arr = new T[n];
    for (int i = 0; i != n; ++i)
        *(arr + i) = uni(e);
    return arr;
}

template <typename T>
typename return_type_chooser<T>::type generate_ordered_array(int n) {
    T* arr = new T[n];
    for (int i = 0; i != n; ++i)
        *(arr + i) = i;
    return arr;
}

template <typename T>
void print_array(T* arr, int n) {
    for (int i = 0; i != n; ++i)
        std::cout << *(arr + i) << " ";
    std::cout << std::endl;
}

template <typename T>
void test_complexity(void (*sort)(T*, int)) {
    for (int i = 1; i < 10; ++i) {
        int num = 10 * std::pow(2, i);
        T *arr = generate_array<T>(num);
        std::clock_t start = std::clock();
        sort(arr, num);
        std::clock_t end = std::clock();
        std::cout << "time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << std::endl;
        delete[] arr;
    }
}

template <typename T>
int binarySearch(T arr[], int n, T target) {
    int l = 0, r = n - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (arr[mid] == target)
            return mid;
        if (arr[mid] < target)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return -1;
}

#endif
