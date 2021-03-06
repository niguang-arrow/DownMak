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

// 用于快排的优化
template <typename T>
void __insertionSort(T arr[], int l, int r) {
    // 内层循环使用 while 循环
    for (int i = l + 1; i <= r; ++i) {
        T num = arr[i];
        int j = i - 1;
        while (j >= l && num < arr[j]) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = num;
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
    // 写法二
    __insertionSort(arr, 0, n - 1);
}


template <typename T>
void __merge(T arr[], int l, int mid, int r) {
    T newarr[r - l + 1];
    // 使用 i 来对 arr[l...mid] 范围内的元素进行索引
    // 使用 j 来对 arr[mid+1...r] 范围内的元素进行索引
    // 使用 k 来对 newarr[0...r-l+1] 范围内的元素进行索引
    int i = l, j = mid + 1, k = 0;
    while (k <= r - l + 1) {
        if (i > mid) {
            newarr[k++] = arr[j++];
        }
        else if (j > r) {
            newarr[k++] = arr[i++];
        } // 只有在以上两个条件不满足的情况下，才需要对 arr 中的值进行比较
        else if (arr[i] < arr[j]) {
            newarr[k++] = arr[i++];
        } else {
            newarr[k++] = arr[j++];
        }
    }

    for (int m = l; m <=r; ++m)
        arr[m] = newarr[m - l];
}


template <typename T>
void __merge2(T arr[], int l, int mid, int r) {
    T newarr[r - l + 1];

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
void __mergeSort(T arr[], int l, int r) {
    if (l >= r)
        return;
    int mid = l + (r - l) / 2;
    __mergeSort(arr, l, mid);
    __mergeSort(arr, mid + 1, r);
    //__merge(arr, l, mid, r);
    __merge2(arr, l, mid, r);
}

template <typename T>
void mergeSort(T arr[], int n) {
    __mergeSort(arr, 0, n - 1);
}


template <typename T>
int partition(T arr[], int l, int r) {
    std::swap(arr[l], arr[rand() % (r - l + 1) + l]);
    T v = arr[l]; // 作为基准点

    // 序列被分为: v | arr[l+1...j] < v | arr[j+1...i) > v | arr[i]
    // 其中 v 是基准点, arr[l+1...j] 范围内的元素小于 v, 而 arr[j+1...i) 范围内
    // 的元素大于v, i 指向当前访问的元素.
    // 所以初始化时 j 为 l, 而 i 为 l + 1
    int j = l;
    for (int i = l + 1; i <= r; ++i) {
        // 只需要考虑 arr[i] < v, 那么只要将当前元素与 arr[j+1...i) 中的第一个元素交换
        // 即可, 然后再将j向后移; 而当 arr[i] > v 时, 只需将 i 移向下一个元素
        if (arr[i] < v) { 
            std::swap(arr[j + 1], arr[i]);
            ++j;
        }
    }
    std::swap(arr[j], arr[l]);
    return j;
}


template <typename T>
void __quickSort1(T arr[], int l, int r) {
    if (r - l <= 15) {
        __insertionSort(arr, l, r);
        return;
    }
    // 首先是进行 partition 操作, 获得基准点最终在排序好序列中的位置
    int p = partition(arr, l, r);
    __quickSort1(arr, l, p - 1);
    __quickSort1(arr, p + 1, r);
}

// 现在首先完成最简单的快排, 注意它对含有大量重复元素的序列
// 可能会退化为 O(n^2)
template <typename T>
void quickSort1(T arr[], int n) {
    srand(time(NULL));
    __quickSort1(arr, 0, n - 1);
}



void merge(vector<int> &nums, int left, int mid, int right) {
    if (nums.empty() || left >= right)
        return;
    vector<int> array(right - left + 1);
    int k = 0; // 用于遍历 array
    int l = left, r = mid + 1;
    while (l <= mid && r <= right) {
        if (nums[l] < nums[r])
            array[k++] = nums[l++];
        else
            array[k++] = nums[r++];
    }
    while (l <= mid)
        array[k++] = nums[l++];
    while (r <= right)
        array[k++] = nums[r++];

    for (int i = 0; i < array.size(); ++i)
        nums[left + i] = array[i];
    return;
}

void mergeSort(vector<int> &nums, int left, int right) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(nums, left, mid);
    mergeSort(nums, mid + 1, right);
    merge(nums, left, mid, right);
}

void mergeSort(vector<int> &nums) {
    if (nums.empty())
        return;
    mergeSort(nums, 0, nums.size() - 1);
}



int main() {
    int n = 20;
    int *arr = generate_array<int>(n);
    int *arr2 = copy_array<int>(arr, n);

    print_array(arr, n);
    quickSort1(arr, n);
    print_array(arr, n);


    int *arr3 = generate_array<int>(10000);
    vector<int> nums(arr3, arr3 + 10000);
    mergeSort(nums);
    int *ptr = &nums[0];
    if (test_sorted(ptr, 10000))
        cout << " true " << endl;
    //test_complexity<int>(selectionSort);
    //test_complexity<int>(insertionSort);
    //test_complexity<int>(mergeSort);
    //test_complexity<int>(quickSort1);

    delete[] arr;
    delete[] arr2;
    return 0;
}
