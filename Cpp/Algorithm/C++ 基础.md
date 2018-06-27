# C++ 基础

2018 年 6 月 26 日



## 三大函数

拷贝构造, 拷贝赋值运算符, 析构函数

+ class with non-pointer members (比如 Complex)
+ class with pointer members (比如 string)







面向对象 class 与 class 的三种关系:

+ Composition (复合) : 比如 Adapter
+ Delegation (委托) : 类似复合, 但内部保存的是指针.
+ Inheritance (继承) : 和虚函数搭配最有价值.

base class 的 dtor 必须是 virtual, 否则会出现 undefined behavior.

###继承

#### 继承与虚函数

+ non-virtual 函数: 你不希望 derived class 重新定义(override)它
+ virtual 函数: 你希望 derived class 重新定义(override)它, 且你在父类中对它已有默认的定义.
+ pure virtual 函数: 你希望 derived class 一定要重新定义(override) 它, 你在父类中对它没有默认的定义.

```cpp
class Shape {
public:
	virtual void draw() = 0;  // 纯虚函数, Shape 是抽象的, 没法画出来, 不用定义 draw
  	virtual void error(const string &msg); // 虚函数, 子类可以定义自己的错误函数
  	int objectID() const; // non-virtual 函数, 不需要子类来处理 ID 的问题.
};

class Rectangle : public Shape { ... };
```



#### 设计模式

+ Handle/Body : 委托
+ Observer : 委托 + 继承
+ Adapter : 复合
+ Template Method
+ Composite : 委托 + 继承
+ Prototype : 委托 + 继承 (Design Pattern Explained Simply)