# 面试题

来自 [https://github.com/linw7/Paper-Code](https://github.com/linw7/Paper-Code)

## 分析

+ 数组中重复的数字

比如对 [1, 0, 0, 2, 3, 1], 如果输出所有的重复数字, 那么结果是 0, 1; 作者给出了
两种输出全部重复数字的方法以及一种只输出第一个重复数字的方法, 时间关系只介绍前
两种.

1. 使用排序方法, 输出所有的重复数字, 但会改变数组, 时间 O(nlogn), 空间 O(1)

下面代码的思路是, 首先对数组进行排序, 比如上面的例子结果为 [0, 0, 1, 1, 2, 3],
然后针对排序后数组中的每个元素 arr[i], 判断它之后的元素 arr[i+j] (j 从1开始) 是否
和它相等, 如果 for 循环之后, j 还是 1, 说明后面没有相同的元素, 那么继续处理下一个
元素; 如果 j 不等于 1, 说明有相同元素, 那么打印重复元素, 并将 i 挪到下一个非重复
元素继续处理.

```cpp
#include <iostream>
#include <algorithm>

using namespace std;
void sort_version(int *arr) {
    sort(arr, arr + 7);
    for(int i = 0; i < 7; ++i) {
        int j;
        for(j = 1; arr[i] == arr[i + j]; ++j);
        if(j != 1){
            cout << arr[i] << endl;
            i = i + j - 1;
        }
    }
}
```

2. 使用 map, 输出所有重复数字, 时间 O(n), 空间 O(n)

思路非常清晰, 将元素的个数统计出来即可.

```cpp
#include <iostream>
#include <map>

using namespace std;
void hash_version(int *arr, int length) {
    map<int, int> map;
    // for(auto : arr)
    for(int i =  0; i < length; ++i) {
        ++map[arr[i]];
    }
    for(auto var : map) {
        if (var.second != 1)
            cout << var.first << endl;
    }
}
```
