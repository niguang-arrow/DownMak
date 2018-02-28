#include <iostream>
#include "myfunction.h"


using namespace std;


// 选择排序要点是用 minIndex 保存最小值的索引,
// 为了提升性能, 当 minIndex 和首个元素 i 不相等时, 使用交换操作.
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


// 插入排序需要注意的要点是: i 从 1 开始, 使用 num 保存 arr[i] 的值
// 另外注意 j >= 0; 由于最后会执行 --j 这个步骤, 所以最后要将 num
// 放置在 j+1 的位置
template <typename T>
void insertionSort(T arr[], int n) {
    //// 写法一, 内层循环使用 for 循环
    //for (int i = 1; i < n; ++i) {
        //T num = arr[i];
        //int j;
        //for (j = i - 1; j >= 0 && num < arr[j]; --j) {
            //arr[j + 1] = arr[j];
        //}
        //arr[j + 1] = num;
    //}
    // 内层循环使用 while 循环
    for (int i = 1; i < n; ++i) {
        T num = arr[i];
        int j = i - 1;
        while (j >= 0 && num < arr[j]) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = num;
    }
}


template <typename T>
void mergeSort

int main() {
    int n = 20;
    int *arr = generate_array<int>(n);
    int *arr2 = copy_array<int>(arr, n);

    print_array(arr, n);
    insertionSort(arr, n);
    print_array(arr, n);

    test_complexity<int>(selectionSort);
    test_complexity<int>(insertionSort);

    delete[] arr;
    delete[] arr2;
    return 0;
}
