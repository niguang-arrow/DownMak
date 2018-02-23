#include <iostream>
#include "myfunction.h"


using namespace std;

template <typename T>
void selectionSort(T arr[], int n) {
    for (int i = 0; i < n; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex])
                minIndex = j;
        }
        if (minIndex != i)
            swap(arr[i], arr[minIndex]);
    }
}


int main() {
    int n = 10;
    int *arr = generate_array<int>(n);
    print_array(arr, n);
    selectionSort(arr, n);
    print_array(arr, n);

    test_complexity<int>(selectionSort);

    arr = generate_ordered_array<int>(n);
    print_array(arr, n);
    std::cout << binarySearch(arr, n, 4) << std::endl;

    delete[] arr;
    return 0;
}
