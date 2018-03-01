# 排序算法总结

首先是各个文件的作用, 有哪些函数什么的总结在这里:

```bash
myfunction.h : 提供多种功能的头文件, 如: (注明, 由于我尝试使用 traits 技术来求模板函数
        的返回值类型, 因此像 generate_array 产生新的数组的函数应显式给出元素的类型, 比如
        generate_array<int>)
    int* generate_array(int n) : 生成大小为 n 的随机数组
    int* generate_ordered_array(int n) : 生成大小为 n 的顺序数组
    void print_array(T* arr, int n) : 将数组打印
    void test_complexity(void (*)(T*, int)) : 用于测试排序算法的复杂度, 传入排序的函数即可
    int  binarySearch(T arr[], int n, T target) : 非递归的二分查找法实现

sort.cpp : 排序算法的具体实现
    void selectionSort(T arr[], int n) : 选择排序
    void insertionSort(T arr[], int n) : 插入排序, 代码中的注释是另一种写法
```

选择排序: 每次将数组中最小的元素选出来放在未排序部分的最前方, 使用一个 `minIndex` 记录
    最小值的索引;

插入排序: 将扑克牌排序, 将未排序部分的第一个元素依次和已排好序的部分中的每一个元素比较,
    选择合适的位置; 实现要点是使用一个额外的空间保存当前要排序的元素, 已排好序的元素可以
    依次后移. 插入排序一般比选择排序高效是因为它可以提前结束比较, 而不用和已排好序中的
    所有元素都比较.

选择排序和插入排序是稳定的排序算法, 而归并排序和快速排序是不稳定的排序算法, 稳定的定义是:
排序后相同元素的相对位置仍然不变.

自顶向下的归并排序: 对序列不断进行二分, 然后将各个小序列进行排序后进行归并操作, 归并排序
    在进行归并操作时需要开辟额外的空间来放置排序后的序列.

自底向上的归并排序: 从小序列大小为 1 时开始排序, 印象中似乎可以实现原地排序, 使用 swap;

快速排序: 核心在于 partition 操作, 需要考虑的要点是:
        1. 为了防止算法退化为 O(n^2) 操作, 应该随机选择序列中的某个值作为基准点, 然后
        和首个元素进行交换, 这样可以防止序列二分不均匀;
        2. 优化的一个 tip 为, 当元素较少的时候, 可以使用插入排序;
        3. 为了优化当处理序列中有大量重复元素的情况, 应使用三路快速排序.

堆排序: 堆排序需要先构建一个最大堆数据结构, 由于最大堆是一棵完全二叉树, 因此可以使用数组
    来实现. (最大堆要求当前节点的值比左右子节点的值大) 一种经典的实现是从数组索引为 1 
    的位置开始保存数据. 在最大堆中添加新元素的时候, 可以使用 ShiftUp 操作, 时间复杂度为
    O(logn), 而将元素从最大堆中去除, 可以使用时间复杂度为 O(logn) 的 ShiftDown 操作.
    由于每次从堆中取出元素的值都是最大的, 所以只要反向输出就能得到按从小到大排序的序列.

    堆排序还有一种优化是使用原地排序 Heapify 操作, 核心要点是要知道这样一个性质, 第一个
    非叶子节点的序号是 count / 2; count 是堆中所有元素的个数(画图就知道了) 然后依次遍历
    所有的非叶子节点, 对每个节点使用 ShiftUp 以及 ShiftDown 操作, 就能获得最大堆, 从而
    实现排序.


二分查找: 注意是针对已排序好的序列才能使用二分查找. 排序经常作为其他操作的子过程.
    二分查找也分递归的与非递归的版本. 需要注意的要点是:
    1. 对于边界问题, 需要时刻明确变量的含义
    2. 在求中点时, 需要使用减法而不是加法: `mid = l + (r - l)/2`;

二分搜索树: 特征:
    1. 它是一棵二叉树, 不需要是完全二叉树, 因此无法使用数组来实现, 此时可以使用指针;
    2. 对于树中的每个节点, 它的值都要大于它的左孩子并小于它的右孩子;
    3. 以左右孩子为根节点的子树仍然是一棵二分搜索树; 这说明二分搜索树中天然的带有递归结构.


## 代码展示

+ 选择排序

```cpp
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
```

+ 插入排序

```cpp
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
```

+ 归并排序

```cpp
// 自顶向下的归并排序的核心是 merge 操作(第一种写法, 下面还有第二种写法)
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


// merge 操作的另一种写法, 似乎更为清晰一些, 第一种写法是直观的想法, 以遍历 newarr 为主;
// 下面这种写法先遍历将要归并的两个子序列, 再使用两个 while 循环处理剩下的元素, i, j 总有
// 一个先到边界.
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
```

+ 快速排序

```cpp
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

// 序列被分为: 
// v | arr[l+1...j] < v | arr[j+1...i) > v | arr[i]
// 其中 v 是基准点, arr[l+1...j] 范围内的元素小于 v, 而 arr[j+1...i) 范围内
// 的元素大于v, i 指向当前访问的元素.
// 所以初始化时 j 为 l, 而 i 为 l + 1, 只使用 i 来遍历序列中的元素
template <typename T>
int partition(T arr[], int l, int r) {
    std::swap(arr[l], arr[rand() % (r - l + 1) + l]);
    T v = arr[l]; // 作为基准点

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
```
