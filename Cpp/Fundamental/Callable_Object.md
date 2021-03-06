# 可调用对象

[TOC]

## 2017 年 10 月 17 日

### lambda 函数

+   一般形式:

    ```bash
    [capture list] (parameter list) -> return type { function body }
    ```

    其中参数列表和返回类型可以省略.

+   如果忽略返回类型, lambda 根据函数体中的代码推断出返回类型. 如果函数体中只有一个 return 语句, 则返回类型从返回的表达式中推断出来. 如果包含除单一的 return 语句之外的内容, 且未指定返回类型, 则返回 void.

+   lambda 不能有默认参数

+   一个 lambda 只有在其捕获列表中捕获了一个它所在函数中的局部变量, 才能在函数体中使用该变量

+   当向一个函数传递一个 lambda 时, 同时定义了一个新类型和该类型的一个对象: 传递的参数就是编译器生成的类类型的未命名对象.

+   值捕获:

    +   采用值捕获的前提是变量可以拷贝, 与参数不同, 被捕获的变量的值是在 lambda 创建时拷贝, 而不是在调用时拷贝, 因此随后对该变量的修改不会影响到 lambda 内对应的值.

+   引用捕获:

    +   例子: 比如捕获 ostream 对象.
    +   在 lambda 中使用通过引用捕获的变量时, 实际上使用的是引用所绑定的对象, 因此, 我们需要确保被引用的对象在 lambda 执行的时候是存在的.

+   函数也可以返回一个 lambda, 但是 lambda 中不能包含局部变量的引用捕获

+   隐式捕获:

    +   让编译器推断捕获列表

    +   应在捕获列表中写一个 `&` 或 `=`, 其中 `&` 告诉编译器采用引用捕获, `=` 采用值捕获

    +   若希望对一部分变量采用引用捕获, 另一部分变量采用值捕获, 可以混合使用隐式捕获和显示捕获, **但捕获列表中的第一个元素必须是一个 & 或 =**.

        ```cpp
        [&, c](const string &s) { os << s << c; } // & 是隐式捕获,  c 是显式捕获
        [=, &os](const string &s) { os << s << c; } // = 是隐式捕获,  os 是显式捕获
        ```

+   可变 lambda

    +   默认情况下, 对于一个值被拷贝的变量, lambda 不会改变其值. 如果我们希望改变一个被捕获的变量的值, 就必须在参数列表后加上 mutable 关键字:

        ```cpp
        void func() {
          size_t v1 = 42;
          auto f = [v1]() mutable { return ++v1; };
          v1 = 0;
          auto j = f();  // j 为 43
        }
        ```

    +   一个引用捕获的变量要修改的话, 要求该引用指向的是一个非 const 类型

+   默认情况下, 如果一个 lambda 体包含 return 之外的任何语句, 则编译器假定此 lambda 返回 void. 当我们需要为一个 lambda 定义返回类型时, 必须使用尾置返回类型.



### (书 507 页) lambda 是函数对象

+   当我们编写一个 lambda 后, 编译器将该表达式翻译为一个未命名类的未命名对象. 在 lambda 表达式产生的类中含有一个重载的函数调用运算符. 比如书上的一个例子, 根据字符串的长度进行排序:

    ```cpp
    stable_sort(words.begin(), words.end(), 
                [](const string &s1, const string &s2) { return s1.size() < s2.size(); });
    ```

    其行为类似于下面这个类的一个未命名对象:

    ```cpp
    // 注意到函数调用运算符是 const 的
    class ShorterString {
    public:
      bool operator()(const string &s1, const string &s2) const {
        return s1.size() < s2.size();
      }
    };

    // 可以用这个类重写 stable_sort
    stable_sort(words.begin(), words.end(), ShorterString());
    ```

    **在默认情况下, lambda 不能改变它捕获的变量. 因此在默认情况下, 由 lambda 产生的类当中的函数调用运算符是一个 const 成员函数.** 如果 lambda 被声明为可变的, 则调用运算符就不是 const 的了.

+   当 lambda 通过引用捕获变量时, 将由程序负责确保 lambda 执行时引用所引的对象确实存在. 因此, 编译器可以**直接使用该引用而无须在 lambda 产生的类中将其存储为数据成员**.

+   相反, 通过值捕获的变量被拷贝到 lambda 中. 因此, 这种 lambda 产生的类必须为每个值捕获的变量建立对应的数据成员, **同时创建构造函数, 令其使用捕获的变量的值来初始化数据成员**.

+   注意: **lambda 表达式产生的类不含默认构造函数, 赋值运算符以及默认析构函数**(如上面的 ShorterString). 而对于使用值捕获的 lambda 表达式, 其产生的类将含有数据成员以及一个用于初始化该数据成员的构造函数. 由于该类并不含默认构造函数, 所以要想使用这个类的话, 必须提供实参. (比如 `Greater(sz)` 而不能仅仅是 `Greater()`).



### C++11中能否显式声明一个lambda类型的变量，而不用auto?

看 primer 时写了一段代码:

使用 `std::function` 保存各个可调用对象, 比如函数名 add, 函数指针 `pa`, 可调用对象 `Add()` 以及 lambda 对象 `add_a`. 但注意我在声明 `add_a` 时使用的是 auto, 那么现在的问题是, 如果不用 auto, `add_a` 的类型会是什么? 如果写 `int(int, int)` 是错误的.

先给出结论, 后面说为什么: 要显式声明 lambda, 需要使用 `std::function` 一类的玩意将 `int(int, int)` 给包起来, 比如: `function<int(int, int)>`. 因为即使是一样的 lambda 表达式, 它们所代表的类型都是不一样的, 因此, 需要使用 `function` 将它们统一到一个类型中.

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <map>
#include <unordered_map>

using namespace std;


int add(int a, int b) {
    return a + b;
}

static int (*pa) (int, int) = add;

class Add {
public:
    int operator()(int a, int b) {
        return a + b;
    }
};

using T = int(int, int);

static function<int(int, int)> add_a = [] (int a, int b) { return a + b; };



int main() {
    vector<function<T>> record{add, Add(), add_a, pa};

    for (auto &func : record)
        cout << func(1, 10) << endl;
}
```



在知乎上 https://www.zhihu.com/question/48165767 也看到了这个问题, 

https://www.zhihu.com/question/48165767/answer/114686508 的回答是:

> 所以，不存在符合标准的（不利用编译器内部实现特征）的显式声明lambda型变量的方法，是这样吗？

是。

```cpp
#include <iostream>
#include <typeinfo>

template <typename T>
void printType(T obj) {
    std::cout << typeid(obj).name() << std::endl;
}

int main() {
    printType([](){});
    printType([](){});
    printType(0L);
    return 0;
}
```

用 clang 编译后运行输出为：

```bash
Z4mainE3$_0
Z4mainE3$_1
l
```

你看，即使是一样的 lambda 表达式，它们的类型也是不同的。所以你只能用 `std::function` 一类的玩意把它包起来。



### 条件表达式和等价的 if 语句还是有不同的

比如:

```cpp
int f1(int i) { return i < 0 ? -i : i; }
int f2(int i) {
  	if (i < 0) return -i;
	else return i;
}
```

但它们的不同体现在 lambda 表达式中, 如果没有显式写出返回类型, 比如下面的例子:

```cpp
[] (int i) { return i < 0; -i : i}
[] (int i) {
  	if (i < 0) return -i;
	else return i;
}
```

那么第一种情况会将返回类型推断为 `int`, 而第二种情况推断为 `void`.