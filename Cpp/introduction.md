# Introduction

## 2017 年 7 月 2 日

### Getting Started

+   在 `while` 语句中的条件判断中读取数据:

    ```cpp
    while (std::cin >> value)
    ```

    在判断 `while` 中的条件时, 执行表达式 `std::cin >> value`, 该表达式从标准输入读取数据并存储在 `value` 中. `>>` 操作符返回它左边的操作对象, 即 `std::cin`, 因此条件判断是测试 `std::cin`.

    当我们使用 `istream` 对象作为条件判断, 效果等同于测试 stream 的状态. 如果 stream 的是有效的 (即 stream 没有遇到错误), 那么测试成功; 一个 `istream` 对象是无效的是当我们输入 `end-of-file` 或者

    遇到无效的输入, 比如 `value` 为 `int` 类型但我们输入了 `float` 类型的数据. 当 `istream` 是无效的状态时会使得条件判断产生 `false`. 因此 `while` 不断执行, 知道遇到 `end-of-file` 或者一个输入错误. 当条件判断

    失败时, `while` 语句结束.

### Basics

大多数语言都有一些共性, 比如:

+   Built-in types: 如整型, 字符型等
+   Variables: 可以给我们需要使用的对象进行命名
+   Expressions and statements: 可以让我们对各种类型的值进行操作
+   Control structures: 比如 `if`, `while` 等, 允许我们在一定条件下或者重复执行某些操作
+   Functions: 运行我们定义可以调用的计算单元.

Python 是在运行时进行类型检查, 而 C++ 为静态语言, 在编译时对类型进行检查.



## Learning C++

+ Language features exist to support a variety of programming styles and techniques. Consequently, the task of learning a language should focus on mastering the native and natual styles for that language - not on understanding of every little detail of every language feature.
+ Fundamental application concepts are representedas abstractions(e.g.,classes, templates, and class hierarchies) in libraries. Many of the most fundamental progrmming concepts are represented in the standard library. Thus, learning the standard library is an integral part of learning C++.
+ The purpose of learning a programming language is to become a better programmer, that is, to become more effective at designing and implementing new systems and at maintaining old ones. For this, an appreciation of programming and design techniques is far more important than understanding all the details. The understanding of technical detals comes with time and practice.

