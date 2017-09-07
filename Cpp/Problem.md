# 牛客网习题

## 2017 年 8 月 31 日

+   自己复习了一下排序, 写了很简单的排序算法:

    +   归并排序没写好, 效率比插入排序还要低.

    ```cpp
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

        size_t n = 100000;
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
    ```



## 2017 年 8 月 31 日

-   下列代码编译时会产生错误的是

    ```cpp
    #include <iostream>
    using namespace std;
    struct Foo {
        Foo() {}
      	Foo(int) {}
      	void fun() {}
    };
    int main(void) {
        Foo a(10); // 1
      	a.fun(); // 2
      	Foo b(); // 3
      	b.fun(); // 4
      	return 0;
    }
    ```

    -   答案是 4; 因为 3 是一个函数声明, 返回值为 Foo, 所以 4 中的 b 就不是 Foo 的对象.

-   C 语言中最简单的数据类型有....:

    -   答: 整型、实型、字符型
    -   **注意: C 语言中没有逻辑类型, 如果要表示逻辑类型, 需要使用整型数值替代, 0 表示 false, 非零值表示 true.**

## 2017 年 8 月 30 日

+   若要重载+、=、<<、=和[]运算符，则必须作为类成员重载的运算符是: `=` 和 `[]`

    +   解释：

    +   (1)只能使用成员函数重载的运算符有：=、()、[]、->、new、delete。

    +   (2)单目运算符最好重载为成员函数。

    +   (3) 对于复合的赋值运算符如+=、-=、*=、/=、&=、!=、~=、%=、>>=、<<=建议重载为成员函数。

    +   (4) 对于其它运算符，建议重载为友元函数。

        运算符重载的方法是定义一个重载运算符的函数，在需要执行被重载的运算符时，系统就自动调用该函数，以实现相应的运算。也就是说，运算符重载是通过定义函数实现的。运算符重载实质上是函数的重载。

+   下面代码的结果是:

    ```cpp
    char a=101;
    int sum=200;
    a+=27;sum+=a;
    printf("%d\n",sum);
    ```

    +   结果是

        ```bash
        a为char型,-128~127,a=101,a+=27后溢出a=-128:
        a=127时不溢出      01111111 (127补码)
        a+=1时溢出         10000000 (-128补码)
                                    
        sum += a;
        sum为int型，a(char提升为int)
        10000000  -------->
        11111111  11111111   11111111   10000000(-128补码)
                                                                  
        所以,sum=200-128:
              00000000    00000000   00000000    11001000
         +    11111111    11111111   11111111    10000000
         ----------------------------------------------------------
              00000000  00000000  00000000  01001000    (64+8=72)   
        ```

        ​