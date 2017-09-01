#include <iostream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

vector<unsigned> generateRandomArray(size_t n, size_t max_num = 10) {
    default_random_engine e(1234);
    uniform_int_distribution<unsigned> u(0, max_num);
    vector<unsigned> vec;
    for (size_t i = 0; i != n; ++i)
        vec.push_back(u(e));
    return vec;
}

template <typename T>
void printArray(T arr) {
    for (const auto &s : arr)
        cout << s << " ";
    cout << endl;
}

/*
 * 冒泡排序: 
 * 思路是比较相邻的两个元素, 将较小的元素放在前面, 较大的元素放在后面, 
 * 对所有的元素重复上述步骤, (除了最后一个元素), 这样, 最后元素从小到大排列
 */

template <typename T>
void BubbleSort(T &arr) {
    for (size_t i = 0; i < arr.size() - 1; ++i)
        for (size_t j = 0; j < arr.size() - i - 1; ++j)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

/*
 * 选择排序:
 * 思路是在序列中找到最小值, 然后放到序列的起始位置 
 */

template <typename T>
void SelectionSort(T &arr) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        auto minIndex = i;
        for (size_t j = i + 1; j < arr.size(); ++j) {
            if (arr[j] < arr[minIndex])
                minIndex = j;
        }
        if (minIndex != i) 
           swap(arr[minIndex], arr[i]);
    }
}


/*
 * 插入排序: 
 * 思路是对当前选定的元素, 它前面所有的元素必须是已经排序好的, 然后将其插入到
 * 它前面的序列中 (我朴素的思路啊...)
 */
template <typename T>
void InsertionSort(T &arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        auto j = i - 1;
        auto Temp = arr[i];
        while (0 <= j && arr[i] < arr[j]) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = Temp;
    }
}


/*
 * 归并排序:
 * 思路是将已排序的两个子序列归并起来, 变成一个有序列
 */

template <typename T>
void __mergeSort(T &arr, size_t left, size_t right) {
    if (right <= left)
        return;
    auto mid = (right - left) / 2 + left;
    __mergeSort(arr, left, mid);
    __mergeSort(arr, mid + 1, right);

    T newarr = arr;
    size_t i = left, j = mid + 1, k = left; // 把 k 写成 0 是错误的...
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            newarr[k++] = arr[i++];
        else
            newarr[k++] = arr[j++];
    }
    while (i <= mid)
        newarr[k++] = arr[i++];
    while (j <= right)
        newarr[k++] = arr[j++];
    arr = newarr;
}

template <typename T>
void MergeSort(T &arr) {
    __mergeSort(arr, 0, arr.size() - 1);
}

int main() {

    size_t n = 10;
    auto arr = generateRandomArray(n);
    //printArray(arr);

    cout << "BubbleSort" << endl;
    clock_t start = clock();
    BubbleSort(arr);
    clock_t end = clock();
    //printArray(arr);
    cout << "Spend: " << double(end - start) * 1000 / CLOCKS_PER_SEC << "ms" << endl;

    cout << "SelectionSort" << endl;
    start = clock();
    SelectionSort(arr);
    end = clock();
    //printArray(arr);
    cout << "Spend: " << double(end - start) * 1000 / CLOCKS_PER_SEC << "ms" << endl;

    cout << "InsertionSort" << endl;
    start = clock();
    InsertionSort(arr);
    end = clock();
    //printArray(arr);
    cout << "Spend: " << double(end - start) * 1000 / CLOCKS_PER_SEC << "ms" << endl;

    cout << "MergeSort" << endl;
    start = clock();
    MergeSort(arr);
    end = clock();
    //printArray(arr);
    cout << "Spend: " << double(end - start) * 1000 / CLOCKS_PER_SEC << "ms" << endl;

    return 0;
}
