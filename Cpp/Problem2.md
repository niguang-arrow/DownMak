# Problem

## 2017 年 8 月 31 日

+ 下列代码编译时会产生错误的是

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

  + 答案是 4; 因为 3 是一个函数声明, 返回值为 Foo, 所以 4 中的 b 就不是 Foo 的对象.

+ C 语言中最简单的数据类型有....:

  + 答: 整型、实型、字符型
  + **注意: C 语言中没有逻辑类型, 如果要表示逻辑类型, 需要使用整型数值替代, 0 表示 false, 非零值表示 true.**

+ ​