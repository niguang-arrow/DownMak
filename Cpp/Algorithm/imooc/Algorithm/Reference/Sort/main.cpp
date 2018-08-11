#include "function.h"
#include <iostream>

using namespace std;

template <typename T>
void merge(T arr[], int l, int mid, int r) {
    T newarr[r - l + 1];

    // 对 arr[l...mid] 和 arr[mid+1...r] 数组归并
    // i 用来索引 arr[l...mid], j 用来索引 arr[mid+1...r]
    // k 用来索引 newarr[0...r-l];
    int i = l, j = mid + 1, k = 0;
    while (i <= mid && j <= r) {
        if (arr[i] < arr[j])
            newarr[k++] = arr[i++];
        else
            newarr[k++] = arr[j++];
    }
    while (i <= mid)
        newarr[k++] = arr[i++];
    while (j <= r)
        newarr[k++] = arr[j++];

    for (int m = l; m <= r; ++m)
        arr[m] = newarr[m - l];
}

template <typename T>
void mergeSort(T arr[], int l, int r) {
    if (l >= r)
        return;
    int mid = l + (r - l) / 2;
    mergeSort(arr, l, mid);
    mergeSort(arr, mid + 1, r);
    merge(arr, l, mid, r);
}

template <typename T>
void merge_sort(T arr[], size_t n) {
    mergeSort(arr, 0, n - 1);
}


int main() {
    const size_t N = 10;
    int arr[N];
    test_sort(arr, N, "Merge Sort", merge_sort<int>);

    const size_t LEN = 1000000;
    int arr2[LEN];
    test_sort(arr2, LEN, "Merge Sort", merge_sort<int>);

    return 0;
}
