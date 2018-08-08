# C++ 基础

2018 年 6 月 26 日

## C++ 11 新特性

c11 的新特性:

```bash
operator type() const;  # 类型转换
explicit complex(...) : initialization list {} # 初始值列表
pointer-like object
function-like object
namespace
template specialization
Standard Library

# 下面是 C11 新特性
variadic template
move constuctor
Rvalue reference 
auto
lambda
range-base for loop
unordered containers
Object Model
```





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




### 虚继承

+ "菱形"继承的问题:
  + 数据冗余
  + 二义性

解决方法: 使用虚继承.

```cpp
class B : virtual public A {}; // 也可以写成 class B : public virtual A {};
class C : virtual public A {};
class D : public A, public B {}; 
```



#### 问题

1. 虚继承是如何实现的?



### 虚函数与多态

+ 多态:
  + 静态多态/静态绑定: 其实就是函数重载(在同一作用域内), 利用函数在编译时重命名标识符来实现.
  + 动态多态/动态绑定: override, 使用基类的指针或引用来调用 override 的虚成员函数.(不同的作用域)
+ ​










##Const

### what is const

**Const**: A compile time constraint that an object can not be modified

```cpp
const int i = 9;
const_cast<int&>(i) = 6; // 将 const 去除

int j;
static_cast<const int&>(j) = 7; // 报错, 此时 j 是 const 的了.

// const 和函数
const string& getName() { return name; }
int getAge() const { return age; } // const 对象可以调用这个函数.
void setVal(const int &v) { ... }

//mutable 与 const
class Array {
  	vector<int> v;
  	mutable int counter;
public:
  	// 虽然 const 函数中修改了 counter, 但是由于 counter 是
  	// mutable 的, 因此能通过编译.
 	int getItem(int idx) const {
  		counter ++;
      	return v[idx];
	}
};

// 使用 const_cast, 但是尽量避免这种方式, mutable 至少可以告诉
// 使用者这个变量是可变的.
class Array {
  	vector<int> v;
  	int counter;
public:
 	int getItem(int idx) const {
  		const_cast<Array*>(this)->counter ++;
      	return v[idx];
	}
};
```



## 构造函数

const 数据成员以及引用数据成员不能被赋值, 只能进行初始化.

```cpp
class dog {};

// 等价于

class dog {
public:
	dog(const dog &rhs) {...} // Member by member initialization
  	// 由于是 Member by member copying, 所以如果有 const 成员或引用成员,
  	// 该函数就不会被编译器产生.
  	dog& operator=(const dog &rhs) {...} // Member by member copying
  	dog() {...} // 1. call base class's default constructor
  				// 2. call member's default constructor
  	~dog() {...} // 1. call base class's default destructor
};				// 2. call member's default destructor	
```





## 引用

引用一定要有初值.

```cpp
int x = 0;
int &r = x;

/*
1. sizeof(r) == sizeof(x)
2. &r = &x

object 和其 reference 的大小相同, 地址也相同(全都是假象)
Java 里头的所有变量都是 reference.
*/
```

+ 函数声明:

以下被视为相同的 `signature`, 所以两者不能同时存在.

```cpp
double imag(const double &im) {...}
double imag(const double im) {...} // Ambiguity
```

另外, 在参数后面还可以加 const, 在类中只有 const 对象才能调用的方法.





## C++ 虚函数表解析

2018 年 7 月 4 日

来自: https://blog.csdn.net/haoel/article/details/1948051

+ C++ 中的虚函数主要是实现了多态的机制. 关于多态, 简而言之就是用父类型的指针指向子类的实例, 然后通过父类的指针调用实际子类的成员函数. 这种技术可以让父类拥有 "多种形态", 这是一种泛型技术: 试图使用不变的代码来实现可变的算法. 比如:

  + 模板技术
  + RTTI 技术
  + 虚函数技术

  要么试图做到在编译时决议, 要么试图做到在运行时决议.

### 虚函数表

+ 虚函数是通过一张虚函数表(virtual table) 来实现的, 简称 V-Table. C++ 编译器应该保证虚函数表的指针存在于对象实例中的最前面的位置(这是为了保证取到虚函数表有最高的性能 -- 如果有多层继承或多重继承的情况下). 这意味着我们可以**通过对象实例的地址**得到这张虚函数表, 然后可以遍历其中的函数指针, 并调用其中的相应的函数.

#### 注意要点

+ 任何妄图使用父类指针调用子类中未覆盖父类的成员函数的行为都会被编译器视为非法(不能使用父类指针调用子类中的非虚函数), 这种行为无法通过编译.



### C++ 内存对象布局

https://blog.csdn.net/haoel/article/details/3081328

#### 对象的影响因素

一个类可能会受到以下因素的影响:

1. 成员变量
2. 虚函数 (产生虚函数表)
3. 单一继承 (只继承于一个类)
4. 多重继承 (继承多个类)
5. 重复继承 (继承的多个父类中其父类有相同的超类)
6. 虚拟继承 (使用 virtual 继承, 为了保证继承后父类的内存布局只会存在一份)
7. 编译器的影响 (优化)
8. 字节对齐

后两者不讨论



## 理解接口规则

[[深入理解C++（二）]理解接口继承规则](http://www.cnblogs.com/kesalin/archive/2012/11/06/interface_inherit.html)

### 重载(overload), 重写(override), 屏蔽(hide)

**重载(overload)**：在相同作用域内，函数名称相同，参数或常量性（const）不同的相关函数称为重载。重载函数之间的区分主要在参数和常量性（const）的不同上，若仅仅是返回值或修饰符 virtual，public/protected/private的不同不被视为重载函数（无法通过编译）。不同参数是指参数的个数或类型不同，而类型不同是指各类型之间不能进行隐身类型转换或不多于一次的用户自定义类型转换（关于类型转换，请参考前文：[类型转型（Type Casting）](http://www.cppblog.com/kesalin/archive/2012/10/28/type_cast.html)）。当调用发生时，编译器在进行重载决议时根据调用所提供的参数来选择最佳匹配的函数。

**重写(override)**：派生类重写基类中同名同参数同返回值的函数（通常是虚函数，这是推荐的做法）。同样重写的函数可以有不同的修饰符virtual，public/protected/private。

**屏蔽(hide)**：一个内部作用域（派生类，嵌套类或名字空间）内提供一个同名但不同参数或不同常量性（const）的函数，使得外围作用域的同名函数在内部作用域不可见，编译器在进行名字查找时将在内部作用域找到该名字从而停止去外围作用域查找，因而屏蔽外围作用域的同名函数。

（注：编译器在决定哪一个函数应该被调用时，依次要做三件事：名字查找，重载决议，访问性检查。后续文章将详细介绍这个决定过程。）

### 函数继承规则

1，**优先使用组合而非继承。**

2，**纯虚函数继承规则**-声明纯虚函数的目的是让派生类来继承函数接口而非实现, 唯一的例外就是需要纯析构函数提供实现（避免资源泄漏）。

3，**非纯虚函数继承规则**-声明非纯虚函数的目的是让派生类继承函数接口及默认实现。

4，**非虚函数继承规则**-**永远也不要重写基类中的非虚函数**。

5，**尽量不要屏蔽外围作用域（包括继承而来的）名字。**

6，**基类的析构函数应当为虚函数，以避免资源泄漏。** 假设有如下情况，带非虚析构函数的基类指针 pb 指向一个派生类对象 d，而派生类在其析构函数中释放了一些资源，如果我们 delete pb; 那么派生类对象的析构函数就不会被调用，从而导致资源泄漏发生。因此，应该声明基类的析构函数为虚函数。

7，**避免** **private 继承 **

8，**不要改写继承而来的缺省参数值。** 前面已经说到非虚函数继承是种不好的做法，所以在这里的焦点就放在继承一个带有缺省参数值的虚函数上了。为什么改写继承而来的缺省参数值不好呢？因为虚函数是动态绑定的，而缺省参数值却是静态绑定的，这样你在进行多态调用时：函数是由动态类型决定的，而其缺省参数却是由静态类型决定的，违反直觉。

```cpp
class Base
{
public:
    // 前面的示例为了简化代码没有遵循虚析构函数规则，在这里说明下
    virtual ~Base() {}; 
    virtual void f(int defaultValue = 10)
    {
        cout << "Base::f() value = " << defaultValue << endl;
    }
};

class Derived : public Base
{
public:
    virtual void f(int defaultValue = 20)
    {
        cout << "Derived::f() value = " << defaultValue << endl;
    }
};
```

这段代码的输出为：

`Derived::f() value = 10`

调用的是动态类型 d -派生类 Derived的函数接口，但缺省参数值却是由静态类型 pb-基类 Base 的函数接口决定的，这等隐晦的细节很可能会浪费你一下午来调试，所以还是早点预防为好。

9，还有一种流派认为**不应公开（public）除虚析构函数之外的虚函数接口**，而应公开一个非虚函数，在该非虚函数内 protected/private 的虚函数。这种做法是将**接口何时被调用**（非虚函数）与**接口如何被实现**（虚函数）分离开来，以达到更好的隔离效果。在设计模式上，这是一种策略模式。通常在非虚函数内内联调用（直接在头文件函数申明处实现就能达到此效果）虚函数，所以在效率上与直接调用虚函数相比不相上下。

```cpp
class Base
{
public:
    virtual ~Base() {}
    
    void DoSomething()
    {
        StepOne();
        StepTwo();
    }
private:
    virtual void StepOne() = 0;
    virtual void StepTwo() = 0;
};

class Derived : public Base
{
private:
    virtual void StepOne()
    {
        cout << "Derived StepOne： do something." << endl;
    }
    virtual void StepTwo()
    {
        cout << "Derived StepTwo： do something." << endl;
    }
};
```





## Effective C++ 阅读

2018 年 7 月 17 日

### 第 2 章 构造/析构/赋值运算

#### 条款 05: 了解 C++ 默默编写并调用哪些函数

+ 即使是一个空类, 被 C++ 编译器处理之后, 会自动声明编译器版本的一个 copy 构造函数, 一个拷贝赋值运算符以及一个析构函数, 此外, 如果你没有声明任何构造函数, 编译器也会自动声明一个 default 构造函数. 所有这些函数都是 public 且 inline 的.

```cpp
class Empty {};
// 等价于
class Empty {
public:
    Empty() { ... }
    Empty(const Empty &rhs) { ... }
    ~Empty() { ... }

    Empty& operator=(const Empty &rhs) { ... }
};
```

+ **注意编译器产生出来的析构函数是 non-virtual 的, 除非这个 class 的基类自身声明了 virtual 的析构函数, 这种情况下编译器产生的析构函数的虚属性主要来自基类.**
+ default 构造函数和析构函数主要是给编译器一个地方用来放置 "藏身幕后" 的代码, 比如调用基类和 non-static 成员变量的构造函数和析构函数.
+ 而 copy 构造函数与 copy assignment 操作符, 编译器创建的版本只是将源对象的每一个 non-static 成员变量拷贝到目标对象. 但如果这个类包含 `reference` 或者 `const` 成员, 编译器就不会自动生成 copy assignment 操作符(拒绝对引用成员或 const 成员的赋值操作.)
+ 如果你声明了类的构造函数, 那么编译器就不会创建 default 构造函数.



#### 条款 06: 若不想使用编译器自动生成的函数, 就该明确拒绝

+ 比如希望 copy 构造函数与 copy assignment 操作符失效(不能使用), 那么只能声明为 private 的(声明使得编译器不会创建默认的版本, 而令这些函数为 private 的, 可以防止人们调用它们; 只要不去定义这些函数, 那么虽然成员函数与友元函数可以访问它们, 由于只有声明而没有定义, 会得到连接错误(linkage error)). "将成员函数声明为 private 而且故意不实现它们" 这一伎俩是如此为大家接受, 因而被用在 C++ iostream 库中阻止 copying 行为.
+ 为了驳回编译器自动提供的功能, 可以将相应的成员函数声明为 private 并且不予实现:

之后还可以让派生类继承 Uncopyable 这个类阻止 copying.

```cpp
class Uncopyable {
public:
    Uncopyable() {} // 允许 derived 对象构造和析构
    ~Uncopyable() {}
private:
    Uncopyable(const Uncopyable&); // 但是阻止 copying
    Uncopyable& operator=(const Uncopyable&);
};
```

+ 这部分内容书上没有说明, C++11 提供了 delete 关键字:

```cpp
#include <iostream>
using namespace std;

class A {
private:
    int data;
public:
    A(int x = 100) : data(x) {}
    A(const A &a) = delete;

    A& operator=(const A &a) = delete;
};

int main(){
    A a1;
    A a2(a1); // error: call to deleted constructor of 'A'
}
```



#### 条款 07: 为多态基类声明 virtual 析构函数

+ 任何一个 class 只要带有 virtual 函数都几乎确定应该也要有一个 virtual 析构函数. 

+ **如果 class 不含 virutal 函数, 通常表示它并不意图被用作一个 base class**. 当 class 不企图被当做 base class, 令其析构函数为 virtual 往往是个馊主意.(书上列举的例子是, 当 class 不企图作为 base class, 而胡乱设置析构函数为 virtual 会增加对象占用的空间大小, 因为要增加一个 vptr 指针! 在某些情况下, 使代码不能被移植(和 C 或 Fortran 相互移植, 具体看书))

  > 欲实现出 virtual 函数, 对象必须携带某些信息, 主要用来在运行期决定哪一个 virtual 函数该被调用. 这份信息通常是由一个 vptr(virtual table pointer) 指针指出. vptr 指向一个由函数指针构成的数组, 称为 vtbl(virtual table); 每一个带有 virtual 函数的 class 都有一个相应的 vtbl. 当对象调用某一虚函数, 实际被调用的函数取决于该对象的 vptr 指针所指的那个 vtbl, -- 编译器在其中寻找适当的函数指针.

+ 因此: **只有当 class 内含至少一个 virtual 函数, 才为它声明 virtual 析构函数**.

+ 另外, **不要企图继承标准容器或任何其他带有 non-virtual 析构函数的 class.** 比如 string 类是没有 virtual 析构函数的, 若:

  ```cpp
  class MyStr : public string {
  	... 
  };

  string *ps = new MyStr("abc");

  delete ps; // 未有定义! 此时 MyStr 的析构函数没有被调用, 
  		// 因为 string 中的析构函数不是虚函数.
  ```

  从上面这个例子可以看出, 对于继承, 要记住前面的一个结论: 不含有 virutal 函数的类通常表示它并不意图被用作一个 base class. string 类以及 STL 中的容器类都不是设计为作为 base class 使用的.

+ **有时候令 class 带有一个纯虚析构函数(pure virtual)可能颇为便利.** 因为纯虚函数会导致抽象的 class, 而抽象的 class 常用作为 base class, 又由于 base class 应该有个 virtual 析构函数, 因此令希望抽象的 class 声明一个纯虚析构函数, 同时, **你得给这个纯虚析构函数提供一份定义**:

  ```cpp
  class A {
  public:
      virtual ~A() = 0;
  };
  A::~A() {}
  ```

  因为析构函数的运作方式是: 最深层派生(most derived)的那个 class, 其析构函数最先被调用, 然后是每一个 base class 的析构函数依次被调用. 编译器会在 A 的派生类的析构函数中创建一个对 `~A()` 的调用动作, 所以需要对 `~A()` 提供一份定义, 如果不这样做, 编译器会发出抱怨.

+ 总结:

  + polymorphic (带多态性质的) base classes 应该声明一个 virtual 析构函数. 如果 class 带有任何 virutal 函数, 它就应该拥有一个 virtual 析构函数.
  + classes 的设计目的如果不是作为 base class 使用, 或不是为了具备多态性, 就不该声明为 virtual 析构函数.



#### 条款 09: 决不在构造和析构过程中调用 virtual 函数

+ 先给出总结: 在构造和析构期间不要调用 virtual 函数, 因为这类调用从不下降到 derived class(这句话的意思是, 你在派生类对象的构造函数中调用虚函数, 是没有办法调用到派生类的虚函数的, 因此此时派生类还没有完成构造, 要调用也是调用 base class 的虚函数, 因此, 这类调用从不下降到 derived class. 另外, 不要做这样的事情, 看完下面的内容知道会有很多麻烦).

+ 不过说实话, 我自己实验了一下, 发现在构造函数中调用虚函数依旧是有效的, 但是, 天知道什么时候会出现问题, 所以还是不要做这样的事情. 下面将解释原因.

  ```cpp
  class Base {
  public:
      Base() {}
      virtual void func() = 0;
      virtual ~Base() {}
  };

  class Derived : public Base {
  public:
      Derived() {
          func();
      }
      virtual void func() { cout << "Derived" << endl; }
      ~Derived() {}
  };

  int main(){
      Derived d;
  }
  // 输出是 "Derived", 与书中描述不符, 但谁知道什么时候会出错呢..还是要遵循条款.
  ```

+ 不让在构造和析构函数中调用虚函数的原因是: 由于 base class 的构造函数的执行更早于 derived class 的构造函数, 当 base class 的构造函数在执行时, derived class 的成员变量尚未初始化. 如果在此期间调用的虚函数下降至 derived class 这一层, 由于 derived class 中的成员变量还没有初始化, 而 derived class 的成员函数(包括目前调用的虚函数)几乎必然取用这些成员变量, 那么如果该虚函数真的调用成功, 就相当于"要使用对象内部尚未初始化的成分", 这是危险的代名词(**这将是一张通往不明确行为和彻夜调试大会串的直达车票**), 因此 C++ 不让你走这条路.

+ 另一方面更为根本的原因在于: 在 derived class 对象的 base class 构造期间, 对象的类型是 base class 而不是 derived class.  不止虚函数会被编译器解析至 base class 对应的虚函数, 若使用运行期类型信息(runtime type information, 如 `dynamic_cast` 和 typeid), 也会把对象视为 base class 类型.

+ 相同的道理也适用于析构函数: **当派生类的析构函数开始执行时, 对象内的派生类成员变量便呈现出未定义值**. 所以 C++ 视它们仿佛不再存在. 进入 base class 的析构函数后对象成为一个 base class 对象, 而 C++ 的任何部分包括 virtual 函数, `dynamic_cast` 等等也是这样认为的.

  >为什么不能在析构函数中调用虚函数？ - 唐沂的回答 - 知乎
  >https://www.zhihu.com/question/34640808/answer/59578406
  >
  >C++中派生类在构造时会先调用基类的构造函数再调用派生类的构造函数，析构时则相反，先调用派生类的析构函数再调用基类的构造函数。
  >
  >假设一个派生类的对象进行析构，首先调用了派生类的析构，然后在调用基类的析构时，遇到了一个虚函数，这个时候有两种选择：Plan A是编译器调用这个虚函数的基类版本，那么虚函数则失去了运行时调用正确版本的意义；Plan B是编译器调用这个虚函数的派生类版本，但是此时对象的派生类部分已经完成析构，“数据成员就被视为未定义的值”，这个函数调用会导致未知行为。

+ **特别要警惕的是将 virtual 函数放在一个 non-virtual 函数中, 然后再将这个 non-virtual 函数加入到构造或析构函数中**:

  ```cpp
  class Derived : public Base {
  public:
    Derived() {
      init();  // 依旧存在访问未定义变量的风险.
    }
    void init() {
      func();
    }
    virtual void func() { ... }
  };
  ```

+ 由于无法使用 virtual 函数从 base classes 向下调用, 在构造期间, 你可以藉由 "令 derived classes 将必要的构造信息向上传递至 base class 构造函数" 替换之而加以弥补.

+ 书上给了一个使用 static 函数的例子, 使用 static 的意义是: **可以防止访问未成熟对象中尚未初始化的成员变量**.



### 第 6 章 继承与面向对象

#### 前言

本章将解释 C++ 不同特性的真正意义, 例如 "public 继承" 意味着 "is-a"; virtual 函数意味着 "接口必须被继承", non-virtual 函数意味着 "接口和实现都必须被继承".

#### 条款 32: 确定你得 public 继承塑模出 is-a 关系

+ "public 继承" 意味着 **is-a**, 使用于 base 类对象的任何操作都能使用于 derived 类的任何对象. 因为每个 derived 对象都是一个 base 类对象.
+ **is-a** 并非唯一存在于 classes 之间的关系, 另外两种常见的关系是 **has-a**(有一个) 以及 **is-implemented-in-terms-of**(根据某物实现出).
+ 虽然 **is-a** 这种继承关系指明任何施加在 base 类对象上的操作都能作用于 derived 类, 但实际得小心, 因为有很多操作并不能对派生类对象使用.



#### 条款 33: 避免遮掩继承而来的名称

+ derived classes 内的名称会遮掩 base classes 内的名称. 在 public 继承下从来没有人希望如此. 因为如果它在 local 作用域内找到了这个名称, 那么它就不会再去其他作用域查找了, 即使该名称有着不同的参数类型也适用. 派生类的作用域被嵌套在基类的作用域内.
+ 为了让这样的名称再见天日, 可以使用 `using` 声明式或转交函数 (forwarding function)
  + using 声明式可以将被掩盖的 base class 名称带入一个 derived class 作用域内.
  + 使用转交函数: 下面的例子只是为了说明什么是转交函数, 可能例子不太好. 使用转交函数的原因是有的时候你不想继承 base classes 中的所有函数, 特别是在你使用 private 的方式继承 Base 类时.

```cpp
#include <iostream>

using namespace std;

class Base {
public:
    virtual void f() { cout << "Base::f()" << endl; }
    virtual void f(int x) { cout << "Base::f(int)" << endl; }
    virtual ~Base() {}
};

class Derived : public Base {
public:
    virtual void f() { cout << "D::f()" << endl; }
};

int main() {
    Derived d;
    d.f(); // 正确
    d.f(1); // 错误! Base::f(int) 被 hide 了.
}

// 处理方法1: 使用 using 声明式
class Derived : public Base {
public:
    using Base::f;
    virtual void f() { cout << "D::f()" << endl; }
};

// 处理方法2: 使用 forwarding 函数
class Derived : public Base {
public:
    virtual void f() { cout << "D::f()" << endl; }
 	virtual void f(int x) { Base::f(x); } // 转交函数, inline
};
```



#### 条款 34: 区分接口继承和实现继承

+ 类中成员函数的不同声明带来的暗示:

  + 纯虚函数: 纯虚函数有两个最突出的特性: 它们必须被任何 "继承了它们" 的具体 class 重新声明, 而且它们在抽象 class 中通常没有定义.

    + 也就是说, **声明一个 pure virtual 函数的目的是为了让派生类只继承函数接口.** 此外, 我们可以为纯虚函数提供一份定义, 但是调用它的唯一途径是 "调用时明确指出其 class 名称".

      ```cpp
      Shape *ps = new Shape; // 错误! 因为 Shape 为抽象 class, 不能实例化
      Shape *ps = new Circle;
      ps->Shape::draw(); // 通过明确指出类名调用 Shape 中定义的 draw.
      ```

  + **声明 impure virtual 函数的目的是让派生类继承该函数的接口和缺省实现. 它可能被派生类覆盖 (override).**

  ```cpp
  class Shape {
  public:
     virtual void error(const string &msg);
  };
  ```

  Shape 中 error 函数在告诉派生类的设计者: "你必须支持一个 error 函数, 但如果你不想自己写一个, 可以使用 Shape class 提供的默认的版本".

  + 允许非纯虚函数同时指定函数声明和函数的缺省行为, 有可能造成危险. 书上举了一个航空公司的例子, 最后的结论是可以在纯虚函数中定义缺省行为, 而派生类要使用这些缺省行为的话需要显式使用基类的名称.

    ```cpp
    class Shape {
    public:
      virtual void draw() = 0 { ... } // 含默认实现.
    };

    class Derived : public Shape {
    public:
      void draw() {
         Shape::draw(); // 显式调用基类的默认行为.
      }
    };
    ```

  + 如果成员函数是非虚函数(non-virtual), 意味着它并不打算在 derived class 中有不同的行为. 因此, **声明一个 non-virtual 函数的目的是为了让派生类继承函数的接口以及一份强制性实现. 它绝不应该在 derived class 中被重新定义.**

+ 总结一下前面的话: pure virtual 函数, impure virtual 函数和 non-virtual 函数之间的差异使得你可以精确指定想要派生类继承的东西: 只继承接口, 或是继承接口和一份缺省实现, 或是继承接口和一份强制实现.

+ 在 public 继承之下, 派生类总是继承基类的接口.



#### 条款 35: 考虑 virutal 函数以外的其他选择

+ 可以考虑使用 Strategy 模式.



#### 条款 36: 绝不重新定义继承而来的 non-virtual 函数

+ 比如下面的代码:

```cpp
class Base {
public:
    void func() { cout << "Base::" << endl; }
};

class Derived : public Base {
public:
    void func() { cout << "Derived::" << endl; }
};

int main(){
    Derived x;
  	
    Base *pb = &x;
    pb->func();
  
    Derived *px = &x;
    px->func();
}
// 输出
Base::
Derived::
```

它们输出是不同的, 然而按理来说, 它们两者都是通过对象 x 调用成员函数 `func` 的, 行为应该一样才对. 出现问题的原因在与 Derived 派生类定义了自己的 func 这个非虚函数.

因此, **任何情况下都不该重新定义一个继承而来的 non-virtual 函数**.

+ 另外, 结合条款 7, 多态性的 base class 内的析构函数应该是 virtual 的, 如果违反了条款 7, 那就说明也违反了本条款, 因为此时派生类重新定义了非虚的析构函数.



#### 条款 37: 绝不重新定义继承而来的缺省参数值

+ 你只能继承两种函数: 虚函数和 non-virutal 函数, 然而重定义一个继承而来的 non-virtual 函数永远是错误的. 所以本条款只讨论 "继承一个带有缺省参数的 virtual 函数". 这种情况下, 本条款成立的理由就直接而明确了: **虚函数是动态绑定的(dynamically binding), 而缺省参数值却是静态绑定(statically binding)**.
+ 下面先复习静态绑定和动态绑定. **对象所谓的静态类型, 就是它在程序中被声明使所采用的类型**.

```cpp
Shape *ps;
Shape *pc = new Circle;
Shape *pr = new Rectangle;
```

不论它们真正指向什么, 它们的静态类型都是 `Shape*`.

**对象所谓的动态类型, 则是指 "目前所指对象的类型"**. 如 pc 的动态类型为 `Circle*`, pr 的动态类型为 `Rectangle*`. 动态类型可以在程序执行过程中改变(通常是经由赋值操作):

```cpp
ps = pc; // ps 的动态类型是 Circle*
ps = pr; // ps 的动态类型是 Rectangle*
```

+ 虚函数是动态绑定的, 而缺省参数值是静态绑定, 那么你可能会在 "调用一个定义于派生类中的虚函数" 的同时, 却使用基类为它所指定的缺省参数值.

+ C++ 采用这种方式是为了效率, 因为如果缺省参数值也是动态绑定的话, 那么编译器就必须有某种办法在运行期为 virtual 函数决定适当的参数缺省值. 这比目前实行的在 "编译期决定" 的机制更慢更复杂.

+ 另外, 如果派生类中的定义的缺省参数值和基类中的缺省参数值是一样的, 这样也不好, 比如:

  ```cpp
  class Shape{
    virtual void func(int x = 1); 
  };
  class Derived : public Shape {
    void func(int x = 1);
  };
  ```

  那么当基类的缺省函数值发生变化时, 如果派生类中的缺省函数值不同步的发生变化, 就违反了本条款.

+ 为了处理这种情况, 可以考虑条款 35 中提到的 NVI(non-virtual interface) 手法: 令 base class 内的一个 public non-virtual 函数调用 private virtual 函数, **注意后者可以被派生类重新定义, 我们可以让 non-virtual 函数指定缺省参数, 而 private virtual 函数负责真正的工作. 另外根据条款 36, 我们决不能在派生类中重新定义这个 non-virtual 函数**.

```cpp
class Base {
public:
    void pubf(int x = 1) {
        func(x);
    }
    virtual ~Base() {}
private:
    virtual void func(int x) { cout << "Base::" << x << endl; }
};

class Derived : public Base {
public:
  // 在派生类中重定义基类的 private 的虚函数, 注意这里不需要指定
  // 默认的参数.
    void func(int x) { cout << "Derived::" << x << endl; }
};

int main(){
    Derived x;
    Base *pb = &x;
    pb->pubf(10);

    Derived *px = &x;
    px->pubf(100);
}
// 输出
Derived::10
Derived::100
```



#### 条款 38: 通过复合塑模出 has-a 或"根据某物实现出"

标题英文是: Model "has-a" or "is-implemented-in-terms-of" through composition

复合(composition) 是类型之间的一种关系, 当某种类型的对象内含其他种类型的对象, 便是复合关系.

```cpp
class A {...};
class B {
private:
  	A a;      // composed object
  	string name; // 同上
};
```

复合意味着 "has-a"(有一个)或 "is-implemented-in-terms-of"(根据某物实现出)



#### 条款 39: 明智而审慎地使用 private 继承

+ C++ 会在空对象中插入一个 char. 比如:

```cpp
class Empty {};
class A {
public:
    int x;
    Empty a; // 空对象中有 1 个字节
};

// 但是如果是继承的话, 那么 sizeof(B) 就等于 sizeof(int) 了.
class B : public Empty {
public:
    int x;
};

int main(){
    cout << std::boolalpha << (sizeof(A) == sizeof(int)) << endl;l
    cout << std::boolalpha << (sizeof(B) == sizeof(int)) << endl;
}

// 结果:
false
true
```



#### 条款 40: 明智而审慎地使用多重继承

+ 使用虚基类来处理 "钻石型的继承".

```cpp
class A : virtual pubkic B {...};
```

+ 多重继承比单一继承复杂. 虚继承会增加大小,速度,初始化以及赋值复杂度等等成本. 如果虚基类不带任何数据, 将最具有实用价值.





### 第 7 章 模板与泛型编程

#### 条款 43: 学习处理模板化基类内的名称

可在派生类模板中通过 `this->` 去引用基类模板中的成员, 或者直接使用 `base::` 资格修饰符.

比如下面的代码, 是会编译错误的, 原因是 C++ 知道 base class templates 有可能会被特化, 而那个特化的版本中可能不会提供和一般性的 template 相同的接口(比如下面代码中, 我也可以写一个没有定义 `f()` 的 Base 类的特化版本). 因此它**往往拒绝在模板化的基类中寻找继承而来的名称**. (就某种意义而言, 当我们从 Object Oriented C++ 跨进 Template C++(条款 1), 继承就不像以前那样畅行无阻了.)

解决的方法就是条款中说的, 在 `g()` 中 `f()` 的前面加上 `this->`. 

```cpp
#include <iostream>

using namespace std;

template<typename T>
class Base {
public:
    void f() { cout << "Base::f()" << endl; }
    void f(int x) { cout << "Base::f(int)" << endl; }
    ~Base() {}
};

template<typename T>
class Derived : public Base<T> {
public:
    void g() {
        f(); // 这一行, 编译时会报 error: use of undeclared identifier 'f'
      	// 解决方法是:
      	// 1. this->f(); 让编译器去模板化的基类中查找 f(),
      	// 2. 使用 using 声明, 在 Derived 中使用 using Base<T>::f;
      	// 3. 使用明确的资格修饰符, Base<T>::f(); 但问题在于, 如果 f() 是虚函数,
      	// 那么就会关闭 `virtual 绑定行为`. 这往往是最不让人满意的一种解法.
      	// 所以还是用 this-> 吧.
    }
};

int main() {
    Derived<int> d;
    d.g();
}

```





















