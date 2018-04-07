# C++ 零碎知识

2018 年 4 月 1 日

## C++ 基础

### 移动构造函数

推荐资源:

+ [What are move semantics?](https://stackoverflow.com/questions/3106110/what-are-move-semantics)

先给出实例代码, 根据代码讲解:

```cpp
#include <cstring>
#include <algorithm>

class string
{
    char* data;

public:

    string(const char* p)
    {
        size_t size = strlen(p) + 1;
        data = new char[size];
        memcpy(data, p, size);
    }
  
  	~string()
    {
        delete[] data;
    }

    string(const string& that)
    {
        size_t size = strlen(that.data) + 1;
        data = new char[size];
        memcpy(data, that.data, size);
    }
  
  	string(string&& that)   // string&& is an rvalue reference to a string
    {
        data = that.data;
        that.data = nullptr;
    }
  	
  	string& operator=(string that)
    {
        std::swap(data, that.data);
        return *this;
    }
};
```

上面定义的 string 类主要实现了构造函数, 拷贝构造函数, 以及移动构造函数与拷贝赋值运算符. 按照下面三种方法使用 string 这个类时:

```cpp
string a(x);                                    // Line 1
string b(x + y);                                // Line 2
string c(some_function_returning_a_string());   // Line 3
```

不多说了, 看原文. 总之, 比如 Line2 和 3 中的参数, 它们是右值. 右值表示在即将销毁的临时对象. 重要的是右值销毁的时间: rvalues denote temporary objects which are destroyed at the next semicolon (to be more precise: at the end of the full-expression that lexically contains the rvalue). This is important because during the initialization of `b` and `c`, we could do whatever we wanted with the source `string`, and *the client couldn't tell a difference*!

观察拷贝构造函数和移动构造函数就可以发现, 拷贝构造函数会将参数 that.data 在堆中的数据全部拷贝到新的对象当中, 而移动构造函数仅仅是将指针进行赋值, 而不用将堆中的数据全部拷贝一份. 拷贝构造函数进行的深拷贝, 这样可以保证源数据不会被修改, 而移动构造函数将源对象的指针拷贝之后, 再将指针设置为 nullptr, 这样是没有问题的, 因为用户不需要再去访问源对象了(源对象为右值.)

比如我写了一个例子:

```cpp
#include <iostream>
```



### 右值引用

https://stackoverflow.com/questions/3106110/what-are-move-semantics/11540204#11540204

+ 这个回答是移动构造函数中的进阶. 考虑左值以及右值引用, 再加上 const, 那么现在有 4 种引用了, 它们能绑定什么类型的 X?

```bash
            lvalue   const lvalue   rvalue   const rvalue
---------------------------------------------------------              
X&          yes
const X&    yes      yes            yes      yes
X&&                                 yes
const X&&                           yes      yes
```

注意 `const X&&` 其实没啥用...

+ 隐式转换:

  `X&&` 右值引用能绑定第一个不同类型 Y 的值类型(左值或右值), 只要存在 Y 到 X 的隐式转换. 比如:

```cpp
void some_function(std::string&& r);

some_function("hello world");
```

存在 "hello world" (`const char[12]`) 通过 `const char*` 转换为 `std::string`. 此时一个临时的 string 对象生成了, 因此 r 可以绑定到那个对象.



### delete 后的对象还能使用?

[Why doesn't delete destroy anything?](https://stackoverflow.com/questions/3280410/why-doesnt-delete-destroy-anything/3280465#3280465)

注意, 当使用已释放的对象时, 会导致未定义的行为, 即使很多时候看似能得到正确的结果. 释放对象后, 操作系统不会 reset the memory, 或者做一些其他特殊的操作, 因为这样是会浪费时间, 毕竟释放的对象不会再被使用了.



### split string

https://www.hackerrank.com/challenges/simple-array-sum/problem

分开 string:

```cpp
#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/*
 * Complete the simpleArraySum function below.
 */
int simpleArraySum(vector<int> ar) {
    /*
     * Write your code here.
     */
    int sum = 0;
    for (auto &d : ar)
        sum += d;
    return sum;

}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int ar_count;
    cin >> ar_count;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string ar_temp_temp;
    getline(cin, ar_temp_temp);

    vector<string> ar_temp = split_string(ar_temp_temp);

    vector<int> ar(ar_count);

    for (int ar_itr = 0; ar_itr < ar_count; ar_itr++) {
        int ar_item = stoi(ar_temp[ar_itr]);

        ar[ar_itr] = ar_item;
    }

    int result = simpleArraySum(ar);

    fout << result << "\n";

    fout.close();

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}

```





## STL 相关

### 成员方法 vs 算法函数

推荐视频教程 [Advanced C++ STL 标准模板库进阶](https://www.bilibili.com/video/av7701290/)

+ 比如下面的一些实例:

  ```cpp
  // Functions with same name
  // List
  void remove(const T);  template <typename Comp> void remove_if(Comp);
  void unique(const T);  template <typename Comp> void unique(Comp);
  void sort(const T);  template <typename Comp> void sort(Comp);
  void merge(const T);  template <typename Comp> void merge(Comp);
  void reverse();

  // Associative Container
  size_type count(const T&) const;
  iterator find(const T&) const;
  iterator lower_bound(const T&) const;
  iterator upper_bound(const T&) const;
  pair<iterator, iterator> equal_range(const T&) const;
  // Note: they don't have generalized form, because comparison 
  // is defined by the container.

  // Unordered Container:
  size_type count(const T&) const;
  iterator find(const T&);
  pair<iterator, iterator> equal_range(const T&);
  // Note: No generalized form; use hash function to search.
  ```

+ 每个容器都会定义一些和算法函数同名的成员方法, 比如 sort, remove, find 等. 一般优先选择成员方法, 因为算法函数只能通过迭代器来方法容器中的元素, 那么会出现以下一些情况:

  + 链表中使用算法函数删除元素复杂度高, 比如:

  ```cpp
  list<int> ls = {1, 2, 4, 3};

  // O(1), 只需要改变指针指向即可(list 是双向链表), ls 结果是
  // {1, 2, 3}, 发生了变化, 因为容器的方法可以修改容器本身
  ls.remove(4);  

  // O(n), 要将 3 以及 3 以后的元素分别向前一个元素复制, 所以最后的结果
  // 为 {1, 2, 3, 3}, 也就是 4 被后面的 3 覆盖, 这是因为使用迭代器访问
  // 容器, 不能对容器大小进行修改. 
  // 之后为了删除最后的 3, 要使用 ls.erase(iter, ls.end());
  auto iter = std::remove(ls.begin(), ls.end(), 4); 
  ```

  + 链表无法使用排序算法函数, 比如:

  ```cpp
  ls.sort(); // 正确

  // 未定义的行为, 因为sort需要随机访问迭代器,但是链表只有顺序访问的迭代器.
  std::sort(ls.begin(), ls.end()); 
  ```

  + 查找函数复杂度高, 比如:

  ```cpp
  map<char, string> mymap = {{'a', "Apple"}, {'b', "Bob"}};

  mymap.find('a'); // O(logN)

  // O(n), 另一方面还需要注意, std::find 不仅要查找 'a', 还要比较
  // "Apple" 这个 value 是否也相等, 但是 mymap.find 不用.
  std::find(mymap.begin(), mymap.end(), make_pair('a', "Apple"));
  ```

  ​

























