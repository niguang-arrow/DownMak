# Introduction

## 2017 年 8 月 9 日

### 13.6 对象移动

+   在很多情况下都会发生对象拷贝. 在其中某些情况下, 对象拷贝后就立即被销毁了. 在这些情况下, 移动而非拷贝对象会大幅度提升性能.

+   在重新分配内存的过程中, 从旧内存将元素拷贝到新内存是不必要的, 更好的方式是移动元素. 使用移动而不是拷贝的另一个原因源于 IO 类或 `unique_ptr` 这样的类. 这些类都包含不能被共享的资源 (如指针或 IO 缓冲). 因此, 这些类型的对象不能被拷贝但可以移动.

+   标准库容器, string 和 `shared_ptr` 类既支持移动也支持拷贝. IO 类和 `unique_ptr` 类可以移动但不能拷贝.

+   右值引用

    +   为了支持移动操作, 新标准引入了一种新的引用类型 -- 右值引用 (rvalue Reference). 所谓右值引用就是必须绑定到右值的引用. 我们通过 `&&` 而不是 `&` 来获得右值引用. **右值引用有一个重要的性质 -- 只能绑定到一个将要销毁的对象.** 因此, 我们可以自由地将一个右值引用的资源 "移动" 到另一个对象中.

    +   一般而言, 一个左值表达式表示的是一个对象的身份, 而一个右值表达式表示的是对象的值.

    +   我们不能将左值引用绑定到要求转换的表达式, 字面值常量或是返回右值的表达式. 右值引用有着完全相反的绑定特性: 我们可以将一个右值引用绑定到这类表达式上, 但不能将一个右值引用直接绑定到一个左值上.

        ```cpp
        int i = 42;
        int &r = i; // 正确, r 引用 i
        int &&rr = i;  // 错误: 不能将一个右值引用绑定到一个左值上
        int &r2 = i * 42; // 错误: i*42 是一个右值
        const int &r3 = i * 42; // 正确: 我们可以将一个 const 的引用绑定到一个右值上
        int &&rr2 = i * 42; // 正确: 将 rr2 绑定到乘法结果上.
        ```

        +   返回左值表达式的例子: 返回左值引用的函数, 连同赋值, 下标, 解引用和**前置递增/递减运算符** 
        +   返回右值表达式的例子: 返回非引用类型的函数, 连同算术, 关系, 位以及**后置递增/递减运算符**

+   左值持久, 右值短暂: 由于右值引用只能绑定到临时对象, 我们得知

    1.  所引用的对象将要被销毁
    2.  该对象没有其他用户

    这两个特性意味着: 使用右值引用的代码可以自由地接管所引用的对象的资源.

+   变量是左值: 

    +   变量可以看作只有一个运算对象而没有运算符的表达式. 

    +   **由于变量表达式都是左值, 因此我们不能将一个右值引用绑定到一个右值引用类型的变量上:**

        ```cpp
        int &&rr1 = 42; // 正确: 字面常量是右值
        int &&rr2 = rr1; // 错误: 表达式 rr1 是左值
        ```

+   标准库 move 函数

    +   定义在头文件 `#include <utility>` 中

    +   我们可以显式地将一个左值转换为对应的右值引用类型; 我们还可以通过调用 `std::move` 新标准库函数来获得绑定到左值上的右值引用;

        ```cpp
        int &&rr3 = std::move(rr1); // ok
        ```

        +   **move 调用告诉编译器: 我们有一个左值, 但我们希望像一个右值一样处理它.** 我们必须认识到, 调用 move 就意味着承诺: 除了对 rr1 赋值或销毁它外, 我们将不再使用它. 在调用 move 之后, 我们不能对移后源对象的值做任何假设.
        +   我们可以销毁一个移后源对象, 也可以赋予它新值, 但不能使用一个移后源对象的值.
        +   与大多数标准库名字的使用不同, **对 move 我们不提供 using 声明, 我们直接调用 `std::move` 而不是 move** (原因将在 707 页解释)

+   移动构造函数和移动赋值运算符

    +   为了让我们自己的类型支持移动操作, 需要为其定义移动构造函数和移动赋值运算符, 这两个成员类似对应的拷贝操作, 但是它们从给定对象 "窃取" 资源而不是拷贝资源.

    +   类似拷贝构造函数, 移动构造函数的第一个参数是该类类型的一个引用, 但是这个引用是一个右值引用. 与拷贝构造函数一样, 任何额外的参数都必须有默认实参.

    +   除了完成资源移动, 移动构造函数还必须确保移后源对象处于这样一个状态 -- 销毁它是无害的. 特别是, **一旦资源完成移动, 源对象必须不再指向被移动的资源** -- 这些资源的所有权已经归属于新创建的对象.

    +   为 StrVec 类定义移动构造函数:

        ```cpp
        StrVec::StrVec(StrVec &&s) noexcept // 移动操作不应抛出任何异常
          // 成员初始化器接管 s 中的资源
          : elements(s.elements), first_free(s.first_free), cap(s.cap) {
        	// 令 s 进入这样的状态 -- 对其运行析构函数是安全的
            s.elements = s.first_free = s.cap = nullptr;
        }
        ```

        +   与拷贝构造函数不同, 移动构造函数不分配任何新内存: 它接管给定的 StrVec 中的内存, 在接管内存之后, 它将给定的对象中的指针都置为 nullptr. 这样就完成了从给定对象的移动操作, 此对象将继续存在. 最终, 移后源对象会被销毁, 意味着将在其上运行析构函数. StrVec 的析构函数在 `first_free` 上调用 `deallocate`. 如果我们忘记了改变 `s.first_free`, 则销毁移后源对象就会释放掉我们刚刚移动的内存.

            ```cpp
            // 要知道, StrVec 的析构函数调用了 free() 方法
            // 而 free 方法的定义如下
            void StrVec::free() {
                // 不能传递给 deallocate 一个空指针, 如果 elements 为 0,
                // 函数什么也不做
                if (elements) {
                    // 逆序销毁旧元素
                    for (auto p = first_free; p != elements; )
                        alloc.destroy(--p);
                    alloc.deallocate(elements, cap - elements);
                }
            }
            ```

            +   `free` 方法是从 `first_free` 指针开始处理的, 因此如果忘记了改变 `s.first_free`, 那么销毁移后源就会释放掉我们刚刚移动的内存.
            +   移后源指的是 `&&s` 所引用的那个对象. 由于 `&&s` 只能引用右值, 所以我们需要使用 `std::move` 将左值当成右值一样使用, 但是之后我们就不能在使用经过 `std::move` 处理过的对象, 除了销毁或者对其进行赋值.

    +   移动操作, 标准库容器和异常

        +   由于移动操作 "窃取" 资源, 它通常不分配任何资源, 因此移动操作通常不会抛出任何异常. 当编写一个不抛出异常的移动操作时, 我们应该将此事通知标准库. 我们将看到, 除非标准库知道我们的移动构造函数不会抛出异常, 否则它会认为移动我们的类对象时可能会抛出异常, 并且为了处理这种可能性而做一些额外的工作.
        +   一种通知标准库的方法是使用 `noexcept`. 我们在一个函数的参数列表后指定 noexcept. 我们必须在类头文件的声明和定义中都指定 noexcept. **为啥使用 noexcept 的具体原因看书上 474 页, 大致说来应该是: 由于移动一个对象通常会改变它的值, 比如 vector, 如果在重新分配内存时发生异常, 它就必须使用拷贝构造函数而不是移动构造函数(毕竟只有这样才能保证它原有的元素仍然存在), 因此, 如果我们希望在 vector 重新分配内存这类情况下对我们自定义类型的对象进行移动而不是拷贝, 就必须显式地告诉标准库我们的移动构造函数可以安全使用**.

    +   移动赋值运算符

        +   **移动赋值运算符执行和析构函数和移动构造函数相同的工作**. 与移动构造函数一样, 如果我们的移动赋值运算符不抛出任何异常, 我们就应该将它标记为 noexcept. 类似拷贝赋值运算符, 移动赋值运算符必须正确处理自赋值:

            ```cpp
            StrVec& operator=(const StrVec &&rhs) noexcept {
              if (this != &rhs) {  // 直接检查 this 指针是否与 rhs 相同
                free(); // 释放已有元素
                elements = rhs.elements;
                first_free = rhs.first_free;
                cap = rhs.cap;
                // 将 rhs 置于可析构的状态
                rhs.elements = rhs.first_free = rhs.cap = nullptr;
              }
              return *this;
            }
            ```

        +   移后源对象必须是可析构的

            +   **从一个对象移动数据并不会销毁此对象, 但有时在移动操作完成后, 源对象会被销毁. 因此, 当我们编写一个移动操作时, 必须确保移后源对象进入一个可析构的状态.** 除了将移后源对象置为析构安全的状态之外, 移动操作还必须保证对象仍然是有效的. 一般来说, 对象有效就是指可以安全地为其赋予新值或者可以安全地使用而不依赖其当前值.
            +   **接上面所说的, 在移动操作之后, 移后源对象必须保持有效的, 可析构的状态, 但是用户不能对其进行任何假设.**

+   合成移动操作

    +   **只有当一个类没有定义任何自己版本的拷贝控制成员, 且类的每个非 static 数据成员都可以移动时, 编译器才会为它合成移动构造函数或移动赋值运算符. 编译器可以移动内置类型成员, 如果一个成员是类类型, 且该类有对应的移动操作, 编译器也能移动这个成员.**

        ```cpp
        // 编译器会为 X 和 hasX 合成移动操作
        struct X {
          int i;  // 内置类型可以移动
          string s; // string 定义了自己的移动操作
        };
        struct hasX {
          X mem; // X 有合成的移动操作
        };
        X x, x2 = std::move(x);  // 使用合成的移动构造函数
        hasX hx, hx2 = std::move(x); // 使用合成的移动构造函数
        ```

        ​

### 13.3 交换操作

+   除了定义拷贝控制成员外, 管理资源的类通常还定义了一个名为 swap 的函数. 对于那些与重排元素顺序的算法一起使用的类, 定义 swap 是很重要的. 如果一个类定义了自己的 swap, 那么算法将使用自定义的版本, 否则, 使用标准库定义的 swap. (为了交换两个对象, 我们需要一次拷贝和两次赋值, 但我们更希望交换指针而不是分配新的副本)

+   定义自己的 swap 函数来重载 swap 的默认行为:

    ```cpp
    class HasPtr {
    friend void swap(HasPtr&, HasPtr&);
    // 其他成员定义和前面一样
    };

    inline
    void swap(HasPtr &lhs, HasPtr &rhs) {
      using std::swap;
      swap(lhs.ps, rhs.ps); // 交换指针, 而不是 string 数据
      swap(lhs.i, rhs.i);  // 交换 int 成员
    }
    ```

    +   将 swap 声明为 friend 便于访问类的私有成员. 由于 swap 的存在就是为了优化代码 我们将其声明为 inline 函数.

+   swap 函数应该调用 swap 而不是 `std::swap`:

    ```cpp
    // Foo 类有 HasPtr 类型的成员, 现在要定义 Foo 类的交换函数
    void swap(Foo &lhs, Foo &rhs) {
      using std::swap; 
      swap(lhs.h, rhs.h); // 此处使用的是 HasPtr 版本的 swap
    }
    ```

    +   注意上面的 swap, 虽然函数中使用了 `using std::swap` 声明, 但是最后调用的 swap 函数仍然是 HasPtr 版本的 swap, 而不是标准库中定义的 swap.
    +   **具体原因见书**! (459 页)

+   在赋值运算符中使用 swap

    +   定义 swap 的类通常使用 swap 来定义它们的赋值运算符. 这些运算符使用了一种名为**拷贝并交换** (copy and swap) 的技术. 这种技术将左侧运算对象和右侧运算对象的一个副本进行交换:

        ```cpp
        // 注意 rhs 是按值传递的, 意味着 HasPtr 的拷贝构造函数
        // 将右侧运算对象中的 string 拷贝到 rhs
        HasPtr& HasPtr::operator=(HasPtr rhs) {
          // 交换左侧运算对象和局部变量 rhs 的内容
          swap(*this, rhs); // rhs 现在指向本对象曾经使用的内存
          return *this;  // rhs 被销毁, 从而 delete 了 rhs 中的指针
        }
        ```

        +   注意在这个函数中, 参数并不是一个引用, 我们将右侧运算对象以传值方式传递给了赋值运算符.

    +   **这个技术的有趣之处是它自动处理了自赋值情况且天然是异常安全的.**

+   拷贝控制示例

    +   **拷贝赋值运算符通常执行拷贝构造函数和析构函数中也要做的工作. 这种情况下, 公共的工作应该放在 private 工具函数中完成**.

+   动态内存管理类

    +   某些类需要在运行时分配可变大小的内存空间. 这种类通常可以使用标准库容器来保存它们的数据. 但是, 这一策略并不是对每个类都适用: 某些类需要自己进行内存分配, 这些类一般来说必须定义自己的拷贝控制成员来管理所分配的内存.

+   StrVec 类的设计

    +   vector 类将其元素保存在连续内存中, 为了获得可接受的性能, vector 预先分配足够的内存来保存可能需要的更多元素. vector 的每个添加元素的成员函数会检查是否有空间容纳更多的元素. 如果有, 成员函数会在下一个可用位置构造一个对象, 如果没有可用空间, vector 就会重新分配空间: 它获得新的空间, 将已有元素移到新空间中, 释放旧空间, 并添加新元素.

    +   给出 StrVec 的代码:

    +   说实话, 这个例子以后应该多看看, 将前面学习的知识都总结起来了.

        ```cpp
        #include <iostream>
        #include <string>
        #include <memory>
        #include <utility> // 提供 move

        using namespace std;

        class StrVec {
        public:
            StrVec() : 
                elements(nullptr), first_free(nullptr), cap(nullptr) {}
          	// 使用委托构造函数
            StrVec(const string &s) : StrVec() { push_back(s); }
            StrVec(const StrVec&);
            StrVec& operator=(const StrVec&);
            ~StrVec();
            void push_back(const string&);
            size_t size() const { return first_free - elements; }
            size_t capacity() const { return cap - elements;  }
            string* begin() const { return elements;  }
            string* end() const { return first_free;  }

        private:
            static allocator<string> alloc;
            void chk_n_alloc()
                { if (size() == capacity()) reallocate(); }
            // 返回一个 pair, 两个指针分别指向新空间的开始位置
            // 和拷贝的尾后位置
            pair<string*, string*> alloc_n_copy
                (const string*, const string*);
            void free();
            void reallocate();
            string *elements;
            string *first_free;
            string *cap;
        };

        // 静态成员要在类外初始化
        allocator<string> StrVec::alloc;

        void StrVec::push_back(const string &s) {
          chk_n_alloc(); // 确保有空间容纳新元素
          // 在 first_free 指向的元素中构造 s 的副本
          alloc.construct(first_free++, s);
        }

        pair<string*, string*>
          StrVec::alloc_n_copy(const string *b, const string *e) {
          // 分配空间保存给定范围中的元素
          auto data = alloc.allocate(e - b);
          // 初始化并返回一个 pair, 该 pair 由 data 和 uninitialize_copy 返回
          // 的返回值构成
          return {data, uninitialized_copy(b, e, data)};
        }

        void StrVec::free() {
          // 不能传递给 deallocate 一个空指针, 如果 elements 为 0,
          // 函数什么也不做
          if (elements) {
            // 逆序销毁旧元素
            for (auto p = first_free; p != elements; )
              alloc.destroy(--p);
            alloc.deallocate(elements, cap - elements);
          }
        }

        StrVec::StrVec(const StrVec &s) {
          // 调用 alloc_n_copy 分配空间以容纳与 s 中一样多的元素
          auto newdata = alloc_n_copy(s.begin(), s.end());
          elements = newdata.first;
          first_free = cap = newdata.second;
        }

        // 析构函数调用 free 分配的内存空间
        StrVec::~StrVec() { free(); }

        StrVec& StrVec::operator=(const StrVec &rhs) {
          // 调用 alloc_n_copy 分配内存, 大小与 rhs 中元素占用空间一样多
          auto data = alloc_n_copy(rhs.begin(), rhs.end());
          free();
          elements = data.first;
          first_free = cap = data.second;
          return *this;
        }

        void StrVec::reallocate() {
          // 每次重新分配内存时将容量加倍,
          // 如果 StrVec 为空, 我们将分配容纳一个元素的空间
          auto newcapacity = size() ? 2 * size() : 1;
          auto newdata = alloc.allocate(newcapacity);
          auto dest = newdata; // 指向新数组中下一个空闲位置
          auto elem = elements; // 指向旧数组中下一个元素
          for (size_t i = 0; i != size(); ++i)
            alloc.construct(dest++, std::move(*elem++));
          free(); // 一旦我们移动完元素就释放旧内存空间
          elements = newdata;
          first_free = dest;
          cap = elements + newcapacity;
        }

        int main(int argc, const char* argv[]) {

          //StrVec vec("string");
          StrVec vec;
          string str;
          vec.push_back("string");
          while (cin >> str)
            vec.push_back(str);
          for (const auto &s : vec) {
            cout << s << " ";
          }
          cout << endl;
          return 0;
        }
        ```


## 2017 年 8 月 8 日

(蔡忠强师弟)

12.3 节使用标准库: 文本查询程序 没有看. 之后直接学习第 13 章.

### 13.1.2 拷贝赋值运算符

+   一定要看博客 [C++的一大误区--深入解释直接初始化和拷贝初始化的区别](http://blog.csdn.net/ljianhui/article/details/9245661)

-   拷贝赋值运算符

    -   如果类未定义自己的拷贝赋值运算符, 编译器会为它合成一个.

    -   重载运算符本质上是函数, 其名字是由 operator 关键字后接表示要定义的运算符的符号组成. 因此, 赋值运算符就是一个名为 `operator=` 的函数.

    -   某些运算符, 包括赋值运算符, 必须定义为成员函数, 如果一个运算符是一个成员函数, 其左侧运算对象就绑定到隐式的 this 参数. 对于一个二元运算符, 例如赋值运算符, 其右侧运算对象作为显式参数传递. 

    -   **赋值运算符通常返回一个指向其左侧运算对象的引用** (联想一下 decltype 类型推断, 对表达式返回的是引用.)

    -   注意初始化和赋值的区别

    -   合成拷贝赋值运算符

    -   习题 13.8: 为习题 13.5 中的 HasPtr 类编写赋值运算符. 类似拷贝构造函数, 你的赋值运算符应该将对象拷贝到 ps 指向的位置.

        ```cpp
        // 443 页习题 13.5 中的 HasPtr 类
        class HasPtr {
        public:
          HasPtr(const string &s = string()) : 
          	ps(new string(s)), i(0) {}
        private:
          string *ps;
          int i;
        };

        // 编写拷贝赋值运算符
        HasPtr& HasPtr::operator=(const HasPtr &rhs) {
          auto newps = new string(*rhs.ps);  // 拷贝指针指向的对象
          delete ps;   // 销毁原 string
          ps = newps;  // 指向新的 string
          i = rhs.i; 
          return *this;
        }
        ```

        +   注意习题中的销毁原指针的做法. 先生成拷贝, 再销毁, 再赋值.

-   析构函数

    -   构造函数初始化对象的非 static 数据成员, (还可能做一些其他的工作); 析构函数释放对象使用的资源, 并销毁对象的非 static 数据成员.

        ```cpp
        class Foo {
        public:
          ~Foo(); // 析构函数, 波浪线接类名, 没有返回值, 不接受参数
          // 因此不能被重载, 是唯一的.
        };
        ```

        -   如同构造函数有一个初始化部分和一个函数体, 析构函数也有一个函数体和一个析构部分. 在一个构造函数中, 成员初始化是在函数体执行之前完成的, 并且按照它们在类中出现的顺序进行初始化. 在一个析构函数中, 首先执行函数体, 然后销毁成员. 成员按初始化顺序的逆序销毁. 在一个析构函数中, 不存在类似构造函数中初始化列表的东西来控制成员如何销毁, **析构部分是隐式的**.
        -   与普通指针不同, **指针指针是类类型, 所以具有析构函数**, 智能指针成员在析构阶段会被自动销毁.

    -   合成析构函数

    -   **认识到析构函数体自身并不直接销毁成员是非常重要的, 成员是在析构函数体之后隐含的析构阶段中被销毁的. 在整个对象销毁过程中, 析构函数体是作为成员销毁步骤之外的一部分而进行的.**

    -   习题 13.11: 为前面练习中的 HasPtr 类添加一个析构函数:

        ```cpp
        ~HasPtr() { delete ps; }
        ```

    -   习题 13.12: 在下面的代码片段中会发生几次析构函数的调用?

        ```cpp
        bool fcn(const Sales_data *trans, Sales_data accum) {
          Sales_data item1(*trans), item2(accum);
          return item1.isbn() != item2.isbn();
        }
        ```

        +   这段代码会出现 3 次析构函数的调用:

            1.  函数结束时, 局部变量 item1 
            2.  item2
            3.  参数 accum 的生命周期结束, 被销毁

            在函数结束时, trans 的生命周期也结束了, 但它是 `Sales_data` 的指针, 并不是它指向的 `Sales_data` 对象的生命期结束. (只有 delete 指针时, 指向的动态对象的生命期才结束), 所以不会引起析构函数的调用.

-   **三/五法则**

    -   拷贝构造函数, 拷贝赋值运算符和析构函数可以控制类的拷贝操作; 当然在新标准下, 还可以定义移动构造函数和移动赋值运算符.
    -   **需要析构函数的类也需要拷贝和赋值操作**: 当我们决定一个类是否需要定义它自己版本的拷贝控制成员时, 一个基本原则是首先确定这个类是否需要一个析构函数. 如果一个类需要一个析构函数, 我们几乎可以肯定它也需要一个拷贝构造函数和一个拷贝赋值运算符. (比如前面练习中的 HasPtr 类, 在构造函数中动态分配内存, 合成析构函数不会 delete 一个指针数据成员. 因此此类需要定义一个析构函数来释放构造函数分配的内存)
        -   书上 448 页的例子解释了 **如果一个类需要自定义析构函数, 几乎可以肯定它也需要自定义拷贝赋值运算符和拷贝构造函数**. (因为如果使用合成的拷贝构造函数, 那么这些函数只会简单拷贝成员).
    -   **需要拷贝操作的类也需要赋值操作, 反之依然**, 但未必需要析构函数

-   使用 `=default`

    -   可以将拷贝控制成员定义为 `=default` 来显式的要求编译器生成合成版本的拷贝控制成员:

        ```cpp
        class Sales_data {
        public:
          Sales_data() = default;
          Sales_data(const Sales_data&) = default; // 隐式内联
          Sales_data& operator=(const Sales_data&);
          ~Sales_data() = default;
        };

        Sales_data& Sales_data::operator=(const Sales_data&) = default; // 不是内联的
        ```

        +   **我们只能对具有合成版本的成员函数使用 =default, 即默认构造函数或拷贝控制成员.**

-   阻止拷贝

    -   对某些类来说, 拷贝构造函数和拷贝赋值运算符这些操作是没有合理的意义的. 例如 iostream 类阻止了拷贝, 以避免多个对象写入或读取相同的 IO 缓冲. 为了阻止拷贝, 看起来可能应该不定义拷贝控制成员, 但是这种策略是无效的, 因为编译器生成合成的版本.

    -   定义删除的函数

        -   在新标准下, 我们可以通过拷贝构造函数和拷贝赋值运算符定义为**删除的函数**来阻止拷贝. 删除的函数是这样一种函数: 虽然我们声明了它们, 但不能以任何方式使用它们. 在函数的参数列表后面加上 `=delete` 来指出我们希望将它定义为删除的:

            ```cpp
            struct NoCopy {
              NoCopy() = default;
              NoCopy(const NoCopy&) = delete; // 阻止拷贝
              NoCopy& operator=(const NoCopy&) = delete; // 阻止赋值
              ~NoCopy() = default;
            }; 
            ```

            +   使用 `=delete` 通知编译器以及使用代码的读者我们不希望定义这些成员.
            +   与 `=default` 不同之处之一是, **我们可以对任何函数指定** `=delete`.

        -   **析构函数不能是删除的成员**.

            -   如果析构函数被删除, 那就无法销毁此类型的对象了.
            -   对于析构函数已删除的类型, 不能定义该类型的变量或释放指向该类型动态分配对象的指针.

    -   合成的拷贝控制成员可能是删除的:

        -   原文很多 (450 页下面的部分), 本质上这些规则的含义是: 如果一个类有数据成员不能默认构造, 拷贝, 赋值或销毁, 则对应的成员函数将被定义为删除的.
        -   对于具有引用成员或无法默认构造的 const 成员的类, 编译器不会为其合成默认构造函数.
        -   另外, 如果一个类有 const 成员, 则它不能使用合成的拷贝赋值运算符. 毕竟, 此运算符试图赋值所有成员, 而将一个新值赋予一个 const 对象是不可能的.
        -   **虽然我们可以将一个新值赋予一个引用成员, 但这样做改变的是引用指向的对象的值, 而不是引用本身.**

-   拷贝控制和资源管理

    -   在我们使用过的标准库类中, 标准可以容器和 string 类的行为像一个值. `shared_ptr` 类提供类似指针的行为; IO 类型和 `unique_ptr` 不允许拷贝或赋值, 因此它们的行为既不像值也不像指针.

    -   下面是讲一个例子, 将 HasPtr 类的行为定义为类值行为还是类指针行为. HasPtr 有两个成员, 一个 int 和一个 string 指针. 通常, **类直接拷贝内置类型 (不包括指针) 成员;** 由于这些成员本身就是值, 因此通常应该让它们的行为像值一样. 我们如果拷贝指针成员决定了像 HasPtr 这样的类是具有类值行为还是类指针行为. (HasPtr 参看前面或者看出, 下面给出需重点分析的代码)

        ```cpp
        // 编写拷贝赋值运算符
        HasPtr& HasPtr::operator=(const HasPtr &rhs) {
          auto newps = new string(*rhs.ps);  // 拷贝指针指向的对象
          delete ps;   // 销毁原 string (释放旧内存)
          ps = newps;  // 指向新的 string
          i = rhs.i; 
          return *this;
        }
        ```

        +   **关键概念: 赋值运算符**

            当你编写赋值运算符时, 有两点需要记住:

            1.  如果将一个对象赋予它自身, 赋值运算符必须能够正确的工作
            2.  大多数赋值运算符组合了析构函数或拷贝构造函数的工作

            **当你编写一个赋值运算符时, 一个好的模式是先将右侧运算对象拷贝到一个局部临时对象中.** 当拷贝完成后, 销毁左侧运算对象的现有成员就是安全的了. 一旦左侧运算对象的资源被销毁, 就只剩下将数据从临时对象拷贝到左侧运算对象的成员中了.

        +   为了说明防范自赋值操作的重要性, 考虑如果赋值运算符如下编写将会发生什么:

            ```cpp
            // 这样编写赋值运算符是错误的
            HasPtr&
            HasPtr::operator=(const HasPtr &rhs) {
              delete ps; // 释放对象指向的 string
              // 如果 rhs 和 *this 是同一个对象, 我们就将从
              // 已释放的内存中拷贝数据
              ps = new string(*(rhs.ps));
              i = rhs.i;
              return *this;
            }
            ```

            +   如果 rhs 和本对象是同一个对象, delete ps 会释放 `*this` 和 rhs 指向 string. 接下来, 当我们在 new 表达式中试图拷贝 `*(rhs.ps)` 时, 就会访问一个指向无效内存的指针, 其行为和结果是未定义的.

    -   定义行为像指针的类

        -   对于行为类似指针的类, 我们需要为其定义拷贝构造函数和拷贝赋值运算符, 来拷贝指针成员本身而不是它指向的 string. 我们的类仍然需要自己的析构函数来释放接受 string 参数的构造函数分配的内存. 但是在本例中, 析构函数不能单方面地释放关联的 string. 只有当最后一个指向 string 的 HasPtr 销毁时, 它才可以释放 string.
        -   令一个类展现类似指针的行为的最好方法是使用 `shared_ptr` 来管理类中的资源. 但是有时我们希望直接管理资源, 在这种情况下, 使用引用计数(Reference count) 就很有用了.
        -   **可以将计数器保存在动态数组中, 当创建一个对象时, 我们也分配一个新的计数器. 当拷贝或赋值对象时, 我们拷贝指向计数器的指针. 使用这种方法, 副本和原对象都会指向相同的计数器.**

    -   定义一个使用引用计数的类

        ```cpp
        class HasPtr {
        public:
          // 构造函数分配新的 string 和新的计数器, 将计数器置为 1
          HasPtr(const string &s = string()) :
          	ps(new string(s)), i(0), use(new size_t(1)) {}
          // 拷贝构造函数拷贝所有三个数据成员, 并递增计数器
          HasPtr(const HasPtr &p) :
          	ps(p.ps), i(p.i), use(p.use) { ++*use; }
          HasPtr& operator=(const HasPtr&);
          ~HasPtr();
        private:
          string *ps;
          int i;
          size_t *use; // 用来记录有多少个对象共享 *ps 的成员
        };
        ```

        +   使用 use 来记录有多少对象共享相同的 string. 接受 string 参数的构造函数分配新的计数器, 并将其初始化为 1, 指出当前有一个用户使用本对象的 string 成员.

        +   **析构函数不能无条件地 delete ps -- 可能还有其他对象指向这块内存. 析构函数应该递减引用计数, 指出共享 string 的对象少了一个. 如果计数器变为 0, 则析构函数释放 ps 和 use 指向的内存**

            ```cpp
            HasPtr::~HasPtr() {
              if (--*use == 0) { // 如果引用计数变为 0
                delete ps;  // 释放 string 内存
                delete use; // 释放计数器内存
              }
            }
            ```

        +   **拷贝赋值运算符与往常一样执行类似拷贝构造函数和析构函数的工作, 即它必须递增右侧运算对象的引用计数 (即拷贝构造函数的工作), 并递减左侧运算对象的引用计数, 在必要的时候释放使用的内存 (即析构函数的工作).** 与往常一样, **赋值运算符必须处理自赋值**. 我们先通过递增 rhs 中的计数然后再递减左侧运算对象中的计数来实现这一点. 通过这种方法, 当两个对象相同时, 在我们检查 ps (以及 use) 是否应该释放之前, 计数器就已经被递增过了.

            ```cpp
            HasPtr& HasPtr::operator=(const HasPtr &rhs) {
              ++*rhs.use; // 递增右侧运算对象的引用计数
              if (--*use == 0) { // 然后递减被对象的引用计数, 
                delete ps;       // 如果没有其他用户
                delete use;      // 释放本对象分配的成员
              }
              ps = rhs.ps;
              i = rhs.i;
              use = rhs.use;
              return *this;
            }
            ```


            ```

-   记录一个问题: 上面的 HasPtr 中有 `++*rhs.use` 这样的代码, 可是 `rhs` 是以 `const HasPtr&` 传递过来的, 为何可以对 `*rhs.use` 的值进行修改? 写了两个代码来检验是否可以修改 `const HasPtr&` 中的值?

    ```cpp
    #include <iostream>

    using namespace std;

    class HasPtr {
    public:
        HasPtr(const int &s = int()) :
            ps(new int(s)) {}
        HasPtr& operator=(const HasPtr &rhs) {
            ps = new int(++*rhs.ps);
            return *this;
        }

        int get() const { return *ps;  }
        ~HasPtr() { delete ps;  }

    private:
        int *ps;
    };

    int main(int argc, const char *argv[]) {
        HasPtr ps(10), sp;
        cout << ps.get() << " " << sp.get() << endl;
        sp = ps;
        cout << ps.get() << " " << sp.get() << endl;
        return 0;
    }

    // 得到的结果是
    // 10 0
    // 11 0  // 说明最后 ps 也得到了修改.
    ```

    我在考虑是不是使用动态内存的问题, 但是下面的代码解释了使用栈内存也会得到类似的结论:

    ```cpp
    #include <iostream>

    using namespace std;

    class HasPtr {
    public:
        HasPtr(const int &s = int()) :
            i(s), ps(&i) {}
        HasPtr& operator=(const HasPtr &rhs) {
            ps = &(++*rhs.ps);
            i = rhs.i;
            return *this;
        }

        int get() const { return *ps;  }
        int get_i() const { return i;  }
    private:
        int i = 10;
        int *ps = &i;
    };

    int main(int argc, const char *argv[]) {

        HasPtr ps(10), sp;
        cout << ps.get() << " " <<  ps.get_i() << endl;
        cout << sp.get() << " " <<  sp.get_i() << endl;
        sp = ps;
        cout << ps.get() << " " <<  ps.get_i() << endl;
        cout << sp.get() << " " <<  sp.get_i() << endl;

        return 0;
    }

    // 结果是:
    // 10 10
    //  0  0
    // 11 11 // 说明此时 ps 已经被修改了
    // 11 11  
    ```

### 习题

+   习题 13.14: 假定 numbered 是一个类, 它有一个默认构造函数, 能为每个对象生成一个唯一的序号, 保存在名为 mysn 的数据成员中. 假定 numbered 使用合成的拷贝控制成员, 并给定如下函数:

    ```cpp
    void f(numbered s) { cout << s.mysn << endl; }
    ```

    则下面代码输出什么内容?

    ```cpp
    numbered a, b = a, c = b;
    f(a); f(b); f(c);
    ```

    +   由于使用的是合成的拷贝控制成员, 输出的都是 a 的 mysn 数据成员的值.

+   习题 13.15: 假定 numbered 定义了一个拷贝构造函数, 能生成一个新的序号. 这回改变上一题中调用的输出结果吗? 如果会改变, 为什么? 新的输出结果是什么?

    +   在次程序中, 都是拷贝构造函数在起作用, 因此定义能生成新的序号的拷贝构造函数能改变输出结果.
    +   但注意, 新的输出结果不是 0, 1, 2, 而是 3, 4, 5 (不要把需要当成了函数的调用次数, 也可以是 18, 2323, 1999 之类的, 只是新的序号而已).
    +   因为在定义遍历 a 时, 默认构造函数起作用, 将其序号定义为 0. 当定义 b, c 时, 拷贝构造函数起作用, 将它们的序号分别设定为 1, 2. 但是在每次调用 f 时, 由于参数是 numbered 的类型, 又会触发拷贝构造函数, 使得每一次都将形参 s 的序号设定为新值, 从而导致三次的输出结果是 3, 4, 5.

+   习题 13.18: 定义一个 Employee 类, 它包含雇员的姓名和唯一的雇员证号. 为这个类定义默认构造函数, 以及接受一个表示雇员姓名的 string 的构造函数. 每个构造函数应该通过递增一个 static 数据成员来生成一个唯一的证号.

    ````cpp
    class Employee {
    public:
      Employee() { id = num++; }
      Employee(const string &s) { name = s; id = num++; }
      const string& get_name() { return name; }
      size_t get_id() { return id; }
    private:
      static size_t num = 0;
      string name;
      size_t id = 0;
    };
    ````

### 12.1.4 智能指针和异常

+   使用异常处理程序能在异常发生后令程序流程继续, 但是这种程序需要确保在异常发生后资源能被正确地释放. 一个简单的被释放的方法是使用智能指针.

+   比如使用智能指针, 就是程序块过早结束, 智能指针类也能确保在内存不再需要时将其释放:

    ```cpp
    void f() {
      shared_ptr<int> sp(new int(42)); // 分配一个新对象
      // 这段代码抛出一个异常, 且在 f 中未被捕获
    }// 函数结束时 shared_ptr 自动释放内存
    ```

    +   函数退出有两种可能, 正常处理结束或者发生异常, 无论那种情况, 局部对象都会被销毁. 在上面的程序中, sp 是一个 `shared_ptr` 因此 sp 销毁时会检查引用计数, 在此例中, sp 是指向这块内存的唯一指针, 因此内存会被释放掉. 与之相对的, 当发生异常时, 我们直接管理的内存是不会自动释放的. 如果使用内置指针管理内存, 且在 new 之后在对应的 delete 之前发生了异常, 则内存不会被释放:

        ```cpp
        void f() {
          int *ip = new int(42); 
          // 这段代码抛出一个异常, 且在 f 中未被捕获
          delete ip; 
        }
        ```

        +   如果在 new 和 delete 之间发生异常, 且异常未在 f 中被捕获, 则内存就永远不会被释放了.

+   使用我们自己的释放操作:

    +   **如果你使用智能指针来管理的资源不是 new 分配的内存, 记住传递给它一个删除器**!

    +   这里参见书上 416 页的例子, 对 `shared_ptr` 还定义了

        ```cpp
        shared_ptr<T> p(q, d) // p 接管了内置指针 q 所指的对象的使用权; q 必须能转换为
                              // T* 类型. p 将使用可调用对象 d 来代替 delete
        ```


+   **智能指针陷阱**

    智能指针可以提供动态分配的内存安全而又方便的管理, 但这建立在正确使用的前提下. 为了正确使用智能指针, 我们必须坚持一些基本规范:

    +   不使用相同的内置指针值初始化 (或 reset) 多个智能指针
    +   不 delete get() 返回的指针
    +   不使用 get() 初始化或 reset 另一个智能指针
    +   如果你使用 get() 返回的指针, 记住当最后一个对应的智能指针销毁后, 你的指针就变为无效了.
    +   **如果你使用智能指针来管理的资源不是 new 分配的内存, 记住传递给它一个删除器**!

+   `unique_ptr`: 与 `shared_ptr` 不同, 某个时刻只能有一个 `unique_ptr` 指向一个给定对象. 当 `unique_ptr` 被销毁时, 它所指向的对象也被销毁.

    +   类似 `shared_ptr`, 初始化 `unique_ptr` 必须采用直接初始化的形式:

        ```cpp
        unique_ptr<double> p1; // 可以指向一个 double 的 unique_ptr
        unique_ptr<int> p2(new int(42)); // p2 指向一个值为 42 的 int
        ```

        由于 `unique_ptr` 拥有它指向的对象, 因此 `unique_ptr` 不支持普通的拷贝或赋值操作

        ```cpp
        unique_ptr<string> p1(new string("Test"));
        unique_ptr<string> p2(p1); // 错误: unique_ptr 不支持拷贝
        unique_ptr<string> p3;
                      p3 = p2;     // 错误: unique_ptr 不支持赋值
        ```

    +   `unique_ptr` 的操作: (更详细的见书 418 页)

        ```cpp
        u = nullptr;  // 释放 u 指向的对象, 将 u 置为空
        u.release();  // u 放弃对指针的控制权, 返回指针, 并将 u 置为空
        u.reset();    // 释放 u 指向的对象

        // 所有权从 p1 转移给另一个 unique_ptr
        unique_ptr<string> p2(p1.release()); // release 将 p1 置为空
        unique_ptr<string> p3(new string("ne"));
        // 将所有权从 p3 转移给 p2
        p2.reset(p3.release()); 
        ```

        +   调用 release 会切断 `unique_ptr` 和它原来管理的对象间的联系. release 返回的指针常用来初始化另一个智能指针或给另一个智能指针赋值. **但是, 如果我们不用另一个智能指针来保存 release 返回的指针, 我们的程序就要负责资源的释放**:

            ```cpp
            p2.release();  // 错误: p2 不会释放内存, 而且我们丢失了指针
            auto p = p2.release();  // 正确, 但我们必须记得 delete p
            ```

    +   传递 `unique_ptr` 参数和返回 `unique_ptr`

        不能拷贝 `unique_ptr` 的规则有一个例外: 我们可以拷贝或赋值一个将要被销毁的 `unique_ptr`, 最常见的例子是从函数返回一个 `unique_ptr`:

        ```cpp
        unique_ptr<int> clone(int p) {
          // 正确: 从 int* 创建一个 unique_ptr<int>
          return unique_ptr<int>(new int(p));
        }

        // 还可以返回一个局部对象的拷贝
        unique_ptr<int> clone(int p) {
          unique_ptr<int> ret(new int(p));
          // ...
          return ret;
        }
        ```

+   **核查指针类:**

    +   看了这个才发现自己根本没有理解前面讲过的 `StrBlob` 类
    +   现在发现 `StrBlob` 类实际上是对 `vector<string>` 的封装, 使用 `StrBlob` 可以方便的共享数据, 即 `vector<string>` 中的数据, 这里强调的是共享数据, 如果不这样做的话, 直接拷贝 `vector<string>` 得到的始终是副本. 而 `StrBlob` 利用了 `shared_ptr` 达到了共享数据的效果. 
    +   这里, 又定义了 `StrBlobPtr` 这个核查指针类, 由于 `StrBlob` 本身就是保存着 `vector<string>` 的智能指针, 所以 `StrBlobPtr` 接受的是 `StrBlob` 的对象, 通过解引用和递增操作来访问 `vector<string>` 中的数据, 达到指针的效果. 它是对 `StrBlob` 的补充, 通过 `check` 成员, 可以防止用户访问一个不存在的 vector 对象.

+   动态数组

    +   C++ 提供了两种一次分配一个对象数组的方法. C++ 语言定义了另一种 new 表达式语法, 可以分配并初始化一个对象数组. 标准库中包含有一个名为 allocator 的类, 允许我们将分配和初始化分离.
    +   使用容器的类可以使用默认版本的拷贝, 赋值和析构操作; 分配动态数组的类则必须定义自己版本的操作, 在拷贝, 复制以及销毁对象时管理关联的内存.

+   new 和数组

    +   new 分配一个对象数组, 我们要在类型名之后跟一对方括号, 在其中指明要分配的对象的数目, 返回指向第一个对象的指针, 方括号中的大小必须是整型, 但不必是常量.

        ```cpp
        int *pia = new int[get_size()];

        typedef int arrT[42];
        int *p = new arrT;
        ```

    +   **注意: 要记住我们所说的动态数组并不是数组类型!** 因为当用 new 分配一个数组时, 我们并未得到一个**数组类型**的对象, 而是得到一个**数组元素类型**的指针. 即使我们使用类型别名定义一个数组类型, new 也不会分配一个数组类型的对象. 总之记住 new 返回的是一个元素类型的指针而不是数组类型的指针. **这个事实的影响是:** 由于分配的内存并不是一个数组类型, 因此不能对动态数组调用 begin 或 end (iterator, 参见书上  106 页). 这些函数使用数组维度(回忆一下, 维度也是数组类型的一部分)来返回指向首元素和尾后元素的指针. 出于相同的原因, 也不能用范围 for 语句来处理 (所谓的) 动态数组中的元素.

+   初始化动态分配对象的数组

    +   默认初始化

    +   可以对数组中的元素进行值初始化, 方法是在大小之后跟一对空括号. 在新标准中, 可以提供一个元素初始化器的花括号列表;

        ```cpp
        // 前 4 个使用给定的初始化列表初始化, 剩余的进行值初始化
        string *ps = new string[10]{"a", "b", "c", string(3, 'x')};
        ```

    +   动态分配一个空数组是合法的

        ```cpp
        char arr[0];  // 错误: 不能定义长度为 0 的数组
        char *cp = new char[0]; // 正确: 但 cp 不能解引用
        ```

+   释放动态数组

    +   我们使用特殊形式的 delete -- 在指针前加上一个方括号对:

        ```cpp
        delete p;
        delete [] pa; // pa 指向一个动态分配的数组
        ```

        第二条语句中, **数组中的元素按逆序销毁**.

+   标准库提供了一个可以管理 new 分配的数组的 `unique_ptr` 版本, 为了用 `unique_ptr` 管理动态数组, 我们必须在对象类型后面跟一对空方括号:

    ```cpp
    // up 指向一个包含 10 个未初始化 int 的数组
    unique_ptr<int[]> up(new int[10]);
    up.release();  // 自动调用 delete[] 销毁其指针
    ```

    +   **注意, 这里我有疑问:** 为什么这里的 `up.release()` 就不会报错? 在书上 418 页有个例子:

        ```cpp
        p2.release();  // 错误: p2 不会释放内存, 而且我们丢失了指针
        ```

    +   另一方面, 指向数组的 `unique_ptr` 提供的操作与 (417 页) 中使用的那些操作有一些不同. 当一个 `unique_ptr` 指向一个数组时, 我们不能使用点和箭头成员运算符. 毕竟 `unique_ptr` 指向的是一个数组而不是单个对象时, 因此这些运算符是无意义的. 另一方面, 当一个 `unique_ptr` 指向一个数组时, 我们可以使用下标运算符来访问数组中的元素.

+   allocator 类

    +   new 有一些灵活性上的局限, 其中一方面表现在它将内存分配和对象构造组合在了一起:

        ```cpp
        string *const p = new string[n]; // 构造 n 个空 string
        ```

        +   这个例子中, 初始化执行的是默认初始化. 但是我们分配单个对象时, 通常希望将内存分配和对象初始化组合在一起. 特别是, 那些没有默认构造函数的类就不能动态分配数组了.

    +   allocator 类定义在 `#include <memory>` 头文件中, 它帮助我们将内存分配和对象构造分离开来.

    +   allocator 分配的内存是未构造的, 我们需要在此内存中构造对象. 为了使用 `allocate` 返回的内存, 我们必须使用 `construct` 构造对象. 使用未构造的内存, 其行为是未定义的.

    +   当我们用完对象后, 必须对每个构造的元素调用 destroy 来销毁它们. 函数 destroy 接受一个指针, 对指向的对象执行析构函数.

        ```cpp
        allocator<string> alloc;  // 可以分配 string 的 allocator 对象
        auto const p = alloc.allocate(n); // 分配 n 个未初始化的 string

        auto q = p;  // q 指向最后构造的元素之后的位置
        alloc.construct(q++);  // *q 为空字符串
        alloc.construct(q++, 10, 'c'); // *q 为 cccccccccc
        alloc.construct(q++, "hi"); // *q 为 hi
        // 但此时 q 指向最后构造元素之后的位置, 即 hi 后一个位置

        // 使用 destroy 来销毁每个构造的元素
        while (q != p)
          alloc.destory(--q);  // 释放我们真正构造的 string
        // 最后一步循环我们 destroy 了第一个构造的元素, 
        // 随后 q 将与 p 相等, 循环结束.

        // 调用 deallocate 来似乎释放内存
        alloc.deallocate(p, n);
        ```

    +   拷贝和填充未初始化内存的算法

        ```cpp
        // 这些函数在给定目的位置创建元素 而不是由系统分配内存给它们
        uninitialized_copy(b, e, b2)
        uninitialized_copy_n(b, n, b2)
        uninitialized_fill(b, e, t)
        uninitialized_fill_n(b, n, t)
        ```



### 习题

+   习题 12.23: 编写一段程序, 连接两个字符串字面常量, 将结果保存在一个动态分配的 char 数组中, 重写这个程序, 连接两个标准库 string 对象.

    ```cpp
    #include <iostream>
    #include <cstring>

    using namespace std;

    int main(int argc, const char **argv) {
      const char *c1 = "Hello ";
      const char *c2 = "World";
      char *r = new char[strlen(c1) + strlen(c2) + 1];
      strcpy(r, c1); // 拷贝第一个字符串常量
      strcat(r, c2); // 连接第二个字符串常量
      cout << r << endl;
      
      string s1 = "hello ";
      string s2 = "world";
      strcpy(r, (s1 + s2).c_str()); // 使用 c_str() 获取 (s1+s2) 的内存地址
      cout << r << endl;
      
      // 释放动态数组
      delete [] r;
      return 0;
    }
    ```

+   习题 12.24: 编写一个程序, 从标准输入读取一个字符串, 存入一个动态分配的字符数组中, 描述你的程序如何处理变长输入. 测试你的程序, 输入一个超出你分配的数组长度的字符串.

    ```cpp
    // 我们处理变长输入的方法是: 根据动态分配的字符数组的大小确定字符串长度
    // 阈值, 当读取的字符数超出阈值, 停止读取, 即采取了截断的方式. 还可以
    // 采取其他处理方式, 如抛出异常等. 另外, 为了能读取空格等空白符,
    // 程序中用到了 get 操作来读取字符
    #include <iostream>
    #include <cstring>
    using namespace std;

    int main(int argc, char **argv) {
      char c;
      
      // 分配  20 个字符的动态数组
      // 因此最多存放 19 个字符
      char *r = new char[20];
      int l = 0;
      while (cin.get(c)) {
        if (isspace(c))  // 空格符, 制表符等空白符
          break;
        r[l++] = c; // 存入动态数组
        if (l == 20) {
          cout << "capacity has reached" << endl;
          break;
        }
      }
      r[l] = 0;
      cout << r << endl;
      
      // 释放动态数组
      delete [] r;
    }
    ```

+   习题 12.26: 用 allocator 重写第 427 页中的程序

    ```cpp
    // 427 页中的程序是 
    string *const string = new string[n];
    string s;
    string *q = p;
    while (cin >> s && q != p + n)
      *q++ = s;
    const size_t size = q - p;  // 记住我们读取了多少个 string, 注意
                            // 前面有 q++, 所以这里是 q - p

    // 使用数组后
    delete[] p; // 释放数组
    ```

    下面使用 allocator 进行重写:

    ```cpp
    #include <iostream>
    #include <string>
    #include <memory> 

    using namespace std;

    int main(int argc, const char *argv[]) {
        allocator<string> alloc;
        const size_t n = 10;
        string s;
        auto const p = alloc.allocate(n);
        auto q = p;
        while (cin >> s && q != p + n)
            alloc.construct(q++, s); // 用 s 初始化 *q
      	const size_t size = q - p; // 记住读取了多少个 string

      	// 使用数组
      	for (size_t i = 0; i < size; i++) 
          cout << p[i] << " " << endl;
      
      	// 使用完毕后, 释放已构造的 string
        while (q != p) {
            alloc.destroy(--q); // 刚开始 q 指向的是最后一个元素后一个位置
        alloc.deallocate(p, n); // 释放内存.
            
        return 0;
    }
    ```

    ​

## 2017 年 8 月 7 日

### 第 12 章 动态内存

+   到目前为止, 我们编写的程序中所使用的对象都有着严格定义的生存期. 全局对象在程序启动时分配, 在程序结束时销毁. 对于局部自动对象, 当我们进入其定义所在的程序块时被创建, 在离开块时销毁; 局部 static 对象在第一次使用前分配, 在程序结束时销毁.

    +   除了自动 (auto) 和 static 对象外, C++ 还支持动态分配对象. 动态分配的对象的生存期与它们在哪里创建是无关的, 只有当显式地被释放时, 这些对象才会销毁.
    +   动态对象的正确释放被证明是编程中极其容易出错的地方.

+   **静态内存**用来保存局部 static 对象, 类 static 数据成员以及定义在任何函数之外的变量. **栈内存** 用来保存定义在函数内的非 static 对象. 分配在静态内存或栈内存的对象由编译器自动创建和销毁. 对于栈对象, 仅在其定义的程序块运行时才存在; static 对象在使用之前分配, 在程序结束时销毁.

+   除了静态内存和栈内存, 每个程序还拥有一个内存池. 这部分内存被称作**自由空间**(free store)或者 **堆** (heap). 程序用堆来存储动态分配(dynamiclly allocate)的对象 -- 即那些在程序运行时分配的对象. 动态对象的生存期由程序来控制, 也就是说, 当动态对象不再使用时, 我们的代码必须显式地销毁它们.

+   new: 在动态内存中为对象分配空间并返回一个指向该对象的指针, 我们可以选择对对象进行初始化;

+   delete: 接受一个动态对象的指针, 销毁该对象, 并释放与之相关联的内存.

+   新标准库提供了两种**智能指针**类型来管理动态对象, 行为类似于常规指针, 但是它负责自动释放所指向的对象. (智能指针也是模板)

    +   下面这三种类型都定义在 `#include <memory>` 头文件中
    +   `shared_ptr`: 允许多个指针指向同一个对象
    +   `unique_ptr`: 则 "独占" 所指向的对象
    +   `weak_ptr`: 伴随类, 它是一种弱引用, 指向 `share_ptr` 所管理的对象.

+   `shared_ptr` 类

    ```cpp
    // 创建智能指针
    shared_ptr<string> p1;
    shared_ptr<list<int>> p2;
    ```

    +   默认初始化的智能指针中保存着一个空指针, 解引用一个智能指针返回它所指的对象, 如果在条件判断中使用智能指针, 效果就是判断它是否为空:

        ```cpp
        // 如果 p1 不为空, 检查它是否指向一个空 string
        if (p1 && p1->empty())
          *p1 = "hi"; // 如果 p1 指向一个空 string, 解引用 p1, 赋值
        ```

+   `shared_ptr` 和 `unique_ptr` 都支持的操作:

    ```cpp
    shared_ptr<T> sp  // 空智能指针, 可以指向类型为 T 的对象
    unique_ptr<T> up
    p    // 将 p 用作一个条件判断, 若 p 指向一个对象, 则为 true
    *p   // 解引用 *p, 获得它所指的对象
    p->mem  // 等价于 (*p).mem
    p.get()  // 返回 p 中保存的指针, 要小心使用, 若智能指针释放了其对象, 返回的指针所指向的对象也消失了
    swap(p, q)  // 交换 p 和 q 中的指针
    p.swap(q)
    ```

+   `shared_ptr` 独有的操作

    ```cpp
    make_shared<T>(args) // 返回一个 shared_ptr, 指向一个动态分配的类型为 T 的对象, 
                         // 使用 args 初始化此对象.
    shared_ptr<T> p(q)   // p 是 shared_ptr q 的拷贝; 此操作会递增 q 中的计数器, 
                         // q 中的指针必须能转换为 T*
    p = q    // p 和 q 都是 shared_ptr, 所保存的指针必须能相互转换. 此操作会递减 p 
             // 中的引用计数, 递增 q 中的引用计数; 若 p 的引用计数变为 0, 
             // 则将其管理的原内存释放.
    p.unique()  // 若 p.use_count() 为 1, 返回 true; 否则返回 false
    p.use_count()  // 返回与 p 共享对象的智能指针数量; 可能很慢, 主要用于调试.
    ```

+   `make_shared` 函数

    +   最安全的分配和使用动态内存的方法是调用一个名为 `make_shared` 的标准库函数, 次函数在动态内存中分配一个对象并初始化它, 返回指向此对象的 `shared_ptr`. `make_shared` 用其参数来构造给定类型的对象.

+   `shared_ptr` 的拷贝和赋值

    +   当进行拷贝或赋值操作时, 每个 `shared_ptr` 都会记录有多少个其他 `shared_ptr` 指向相同的对象. 每个 `share_ptr` 都有一个引用计数(reference count). 无论何时我们拷贝一个 `share_ptr`, 计数器都会递增. 一旦一个 `shared_ptr` 的计数器变为 0, 它就会自动释放自己所管理的对象.

        ```cpp
        auto r = make_shared<int>(42);  // r 指向的 int 只有一个引用者
        r = q; // 给 r 赋值, 令它指向另一个地址, 递增 q 指向的对象的引用计数
               // 递减 r 原来指向的对象的引用计数, r 原来指向的对象已没有引用者, 会自动释放
        ```

    +   写了个简单的程序感受一下:

        ```cpp
        #include <iostream>
        #include <string>
        #include <vector>
        #include <memory>

        using namespace std;

        int main(int argc, const char *argv[]) {

            auto r = make_shared<vector<int>>();
            auto b = make_shared<int>(42);

            for (int i = 0; i < 5; ++i)
                r->push_back(i);

            for (const auto &i : *r)
                cout << i << " ";
            cout << endl;
            return 0;
        }

        // 注意到两个问题:
        // 1. *r 表示的是 vector<int>, 另外在初始化 r 时, 使用
        // make_shared<vector<int>>({42}) 会报错
        // 2. 这里定义了 b, 但是编译时编译器没有提示 b 定义了但没使用..
        ```

+   `shared_ptr` 自动销毁所管理的对象...

    +   当指向一个对象的最后一个 `shared_ptr` 被销毁时, `shared_ptr` 类会自动销毁此对象. 它是通过另一个特殊的成员函数 -- 析构函数 (destructor) 完成销毁工作的.
    +   如果你将 `shared_ptr` 存放于一个容器中, 而后不再需要全部元素, 而只使用其中一部分, 要记得用 erase 删除不在需要的那些元素.

+   **使用动态生存期的资源的类**: 程序使用动态内存出于以下三种原因之一:

    1.  程序不知道自己需要使用多少对象
    2.  程序不知道所需对象的准确类型
    3.  程序需要在多个对象间共享数据

    +   容器类是出于第一种原因而使用动态内存的典型例子.
    +   第二种原因的例子将在 15 章看到.
    +   第三种原因的例子:

    ```cpp
    // 到目前为止, 我们接触到的类中, 分配的资源都与对应对象生存期一致
    // 比如, 每个 vector 都 "拥有" 自己的元素, 当我们拷贝一个 vector 时,
    // 原 vector 和副本 vector 中的元素是相互分离的
    vector<string> v1; 
    { // 新作用域
      vector<string> v2 = {"a", "b", "c"};
      v1 = v2; // 从 v2 中拷贝元素到 v1 中
    } // v2 被销毁, 其中的元素也被销毁
      // 而 v1 中有三个元素, 是原来 v2 中元素的拷贝
    ```

    但是某些类分配的资源具有与原对象相独立的生存期. 一般而言, 如果两个对象共享底层的数据, 当某个对象被销毁时, 我们不能单方面地销毁底层数据:

    ```cpp
    // 假定我们希望定义一个 Blob 类, 保存一组元素, 与容器不同, 我们希望
    // Blob 对象的不同拷贝之间共享相同的元素.
    Blob<string> b1;
    {
      Blob<string> b2 = {"a", "b", "c"};
      b1 = b2;
    } // b2 被销毁了, 但是 b2 中的元素不能销毁
     // b1 指向最初由 b2 创建的元素
    ```

    +   **使用动态内存的一个常见原因是允许多个对象共享相同的状态**

+   定义 StrBlob 类: 使用动态内存来管理 `vector<string>`.

    ```cpp
    #include <iostream>
    #include <string>
    #include <vector>
    #include <memory>  // 提供智能指针
    #include <initializer_list>

    using namespace std;

    class StrBlob {
    public:
        typedef vector<string>::size_type size_type;
        StrBlob();
        StrBlob(initializer_list<string> il);
        size_type size()  const { return data->size();  }
        bool empty() const { return data->empty(); }
        void push_back(const string &s) { data->push_back(s); }
        void pop_back();
        string& front();
        string& back();
    private:
        shared_ptr<vector<string>> data;
        void check(size_type i, const string &msg) const;
    };

    StrBlob::StrBlob() : data(make_shared<vector<string>>()) {}
    StrBlob::StrBlob(initializer_list<string> il) :
        data(make_shared<vector<string>>(il)) {}

    void StrBlob::check(size_type i, const string &msg) const  {
        if (i >= data->size())
            throw out_of_range(msg);
    }

    string& StrBlob::front() {
        check(0, "front on empty StrBlob");
        return data->front();
    }

    string& StrBlob::back() {
        check(0, "back on empty StrBlob");
        return data->back();
    }

    void StrBlob::pop_back() {
        check(0, "pop_back on empty StrBlob");
        data->pop_back();
    }

    int main(int argc, const char *argv[]) {

        StrBlob b1;
        {
            StrBlob b2 = {"a", "b", "c"};
            b1 = b2;
            b2.push_back("about");
            cout << b2.size() << endl;
        }
        cout << b1.size() << endl;
        cout << b1.front() << " " << b1.back() << endl;

        return 0;
    }

    // 运行结果是
    4
    4
    a about
    ```

    +   注意 `StrBlob` 类使用默认版本的拷贝, 赋值和销毁成员函数来对此类型的对象进行这些操作. 我们的 StrBlob 类只有一个数据成员, 它是 `shared_ptr` 类型, 因此, 当我们拷贝, 赋值或销毁一个 StrBlob 对象时, 它的 `shared_ptr` 成员会被拷贝, 赋值或者销毁.

+   直接管理内存

    +   `new` 分配内存, `delete` 释放 `new` 分配的内存.

    +   使用 new 动态分配和初始化对象

        +   在自由空间分配的内存是无名的, 因此 new 无法为其分配的对象命名, 而是返回一个指向对象的指针:

            ```cpp
            int *pi = new int; // pi 指向一个动态分配的, 未初始化的无名对象
            // 此处 new 表达式在自由空间构造一个 int 型对象, 并返回
            // 指向该对象的指针, 动态分配的对象是默认初始化的, 这意味着内置类型
            // 或组合类型的对象是未定义的, 而类类型对象将用默认构造函数进行初始化

            string *ps = new string;
            int *pi = new int; // 指向一个未初始化的 int

            // 直接初始化
            int *pi = new int(1024);
            string *ps = new string(10, '9'); // *ps 为 "9999999999"
            vector<int> *pv = new vector<int>{1, 2, 4};

            // 值初始化, 只需在类型名后面加上一对空括号即可
            string *ps1 = new string();
            int *pi = new int;  // 默认初始化, 值是未定义的.
            int *pi = new int();  // 值初始化为 0
            ```

    +   动态分配的 const 对象

        ```cpp
        const int *pi = new const int(1024);
        // 分配并默认初始化一个 const 的空 string
        const string *ps = new const string;
        ```

        +   由于分配的对象是 const 的, new 返回的指针是一个指向 const 的指针.

    +   内存耗尽

        +   一旦一个程序用光了它所有可用的内存, new 表达式就会失败. 默认情况下, 如果 new 不能分配所要求的内存空间, 它会抛出一个 `bad_alloc` 的异常.

        +   我们可以改变使用 new 的方式来阻止它抛出异常:

            ```cpp
            int *p1 = new int; // 如果分配失败, new 抛出 std::bad_alloc
            int *p2 = new (nothrow) int; // 如果分配失败, new 返回一个空指针
            ```

            +   这种形式的 new 称为定位 new (placement new). 如果这种形式的 new 不能分配所需的内存, 它会返回一个空指针.

        +   `bad_alloc` 和 `nothrow` 都定义在 `#include <new>` 中.

        +   为了防止内存耗尽, 应使用 `delete` 将动态内存归还给系统. delete 接受一个指针, 指向我们想要释放的对象.

    +   指针值和 delete 

        +   传递给 delete 的指针必须是指向动态分配的内存, 或者是一个空指针; 释放一块并非 new 分配的内存, 或者将相同的指针值释放多次, 其行为是未定义的.
        +   **通常情况下, 编译器不能分辨一个指针指向的是静态的还是动态分配的对象; 类似的, 编译器也不能分辨一个指针所指向的内存是否已经被释放了.** 对于这些 delete 表达式, 大多数编译器会编译通过, 尽管它们是错误的.
        +   对于 const 动态对象, 它本身是可以销毁的, 只要用 delete 指向它的指针即可.

    +   返回指向动态内存的指针 (而不是智能指针) 的函数给其调用者增加了一个额外负担 -- 调用者必须记得释放内存.

        ```cpp
        // factory 返回一个指针, 指向一个动态分配的对象
        Foo* factory(T arg) {
          return new Foo(arg); // 调用者负责释放此内存
        }

        void use_factory(T arg) {
          Foo *p = factory(arg);
          // 使用 p 但不 delete 它
        } // p 离开了它的作用域, 但它所指向的内存没有被释放!
        ```

    +   **最好是使用智能指针**, 可以避免以下使用 new 和 delete 管理动态内存常见的错误:

        +   忘记 delete 内存: 忘记释放动态内存会导致人们常说的 "内存泄露" 问题. 因为这种内存永远不可能被归还给自由空间. 查找内存泄露错误是非常困难的, 因为通常应用程序运行很长时间后, 真正耗尽内存时, 才能检测到这种错误.
        +   使用已经释放掉的对象: 通过在释放内存后将指针置为空, 有时可以检测出这种错误
        +   同一块内存释放两次: 当两个指针指向相同的动态分配对象时, 可能发生这种错误. 如果对其中一个指针进行了 delete 操作, 对象的内存就被归还给自由空间了. 如果我们随后又 delete 第二个指针, 自由空间就可能被破坏.

    +   delete 之后重置指针, 避免空悬指针 (dangling pointer), 可以在 delete 之后将 nullptr 赋值给指针.

+   `shared_ptr` 和 new 结合使用

    +   前面说过, 如果我们不初始化一个智能指针, 它就会被初始化为一个空指针. 我们还可以用 new 返回的指针来初始化智能指针: (注意必须使用直接初始化)

        ```cpp
        shared_ptr<int> p1(new int(42)); // 直接初始化
        shared_ptr<int> p1 = new int(42); // 错误: 接受指针参数的智能指针构造函数是 explicit 
        ```

    +   默认情况下, 一个用来初始化智能指针的普通指针必须指向动态内存, 因为智能指针默认使用 delete 释放它所关联的对象. 我们可以将智能指针绑定到一个指向其他类型的资源的指针上, 但是这样做, 必须提供自己的操作来替代 delete.

    +   定义和改变 `shared_ptr` 的其他方法: (412 页)

        ```cpp
        shared_ptr<T> p(q) 
        shared_ptr<T> p(u) 
        shared_ptr<T> p(q, d) 
        shared_ptr<T> p(p2, d) 
        p.reset()
        p.reset(q)
        p.reset(q, d)
        ```

    +   不要混合使用普通指针和智能指针

    +   也不要使用 get 初始化另一个智能指针或为智能指针赋值

        智能指针类型定义了一个名为 `get` 的函数, 它返回一个内置指针, 指向智能指针管理的对象. 此函数是为了这样一种情况而设计的: 我们需要向不能使用智能指针的代码传递一个内置指针. 使用 get 返回的指针的代码不能 delete 此指针. 虽然编译器不会给出错误信息, 但将另一个智能指针也绑定到 get 返回的指针上是错误的:

        ```cpp
        shared_ptr<int> p(new int(42)); // 引用计数为 1
        int *q = p.get(); // 正确, 但使用 q 时要主要不要让它管理的指针被释放
        {
          // 未定义: 此时有两个独立的 shared_ptr 指向相同的内存
          shared_ptr<int>(q);
        }// 程序块结束, q 被销毁, 它指向的内存被释放
        int foo = *p; // 未定义: p 指向的内存已经被释放了
        ```

        在本例中, p 和 q 指向相同的内存. 由于它们是相互独立创建的, 因此各自的引用计数都是 1. 当 q 所在的程序块结束时, q 被销毁, 这会导致 q 指向的内存被释放. 从而 p 变成一个空悬指针, 意味着当我们试图使用 p 时, 将会发生未定义的行为. 而且, 当 p 被销毁时, 这块内存会被第二次 delete.

        +   **因此, get 是用来将指针的访问权限传递给代码, 你只有在确定代码不会 delete 指针的情况下, 才能使用 get. 特别是, 永远不要用 get 初始化另一个智能指针或者为另一个智能指针赋值.**

    +   其他 `shared_ptr` 操作: 我们可以用 reset 来将一个新的指针赋予给一个 `shared_ptr`:

        ```cpp
        p = new int(1024); // 错误: 不能将一个指针赋予 shared_ptr
        p.reset(new int(1024)); // 正确: p 指向一个新的对象
        ```

        ​

### 习题

+   习题 12.6: 编写函数, 返回一个动态分配的 int 的 vector, 将此 vector 传递给另一个函数, 这个函数读取标准输入, 将读入的值保存在 vector 的元素中. 再将 vector 传递给另一个函数, 打印读入的值. 记得在恰当的时刻 delete vector.

    ```cpp
    #include <iostream>
    #include <string>
    #include <vector>
    #include <memory>
    #include <initializer_list>

    using namespace std;

    vector<int>* returnVec() {   // 返回动态分配对象
        return new vector<int>;
    }

    /* 参考答案给出的
    vector<int>* returnVec() {   // 返回动态分配对象
        return new (nothrow) vector<int>;
    }

    之后在 main 函数中还有条件判断
    vector<int> *p = returnVec();
    if (!p) {
      cout << "out of memory!" << endl;
      return -1;
    }

    */

    istream& read(istream &is, vector<int> *vec) { // 从标准输入中读取数据到 vec 中
        int item;
        while (is >> item)
            vec->push_back(item);
        return is;
    }

    void print(const vector<int> *vec) {  // 打印出 vec 中的值
        cout << "Now, print your elements" << endl;
        for (const auto &i : *vec)
            cout << i << " ";
        cout << endl;
    }

    int main(int argc, const char *argv[]) {
        vector<int> *p = returnVec();
        read(cin, p);
        print(p);

        delete p;  // delete 动态分配对象
      	p = nullptr;
        return 0;
    }

    // g++ -Wall -std=c++0x -o main main.cpp
    // ./main
    // 输入: 12 21 22<Enter>
    // <Enter> 按完之后在按 Ctrl + D
    // 得到输出
    ```

+   习题 12.7: 重做上一题, 这次使用 `shared_ptr` 而不是内置指针.

    ```cpp
    #include <iostream>
    #include <string>
    #include <vector>
    #include <memory>
    #include <initializer_list>

    using namespace std;

    shared_ptr<vector<int>> returnVec() {

        return make_shared<vector<int>>();
    }

    istream& read(istream &is, shared_ptr<vector<int>> vec) {
        int item;
        while (is >> item)
            vec->push_back(item);
        return is;
    }

    void print(const shared_ptr<vector<int>> vec) {
        cout << "Now, print your elements" << endl;
        for (const auto &i : *vec)
            cout << i << " ";
        cout << endl;
    }

    int main(int argc, const char *argv[]) {
        shared_ptr<vector<int>> p = returnVec(); // 可以使用 auto
        read(cin, p);
        print(p);

        return 0;
    }
    ```


+   习题 12.10: 下面的代码调用了第 413 页中定义的 process 函数, 解释此调用是否正确. 如果不正确, 应该如何修改?

    ```cpp
    // 413 页定义的 process 函数是
    void process(shared_ptr<int> ptr) {
      // 使用 ptr
    } // ptr 离开作用域, 被销毁

    // 此题中的代码
    shared_ptr<int> p(new int(42));
    process(shared_ptr<int>(p));
    ```

    +   此调用是正确的,  利用 p 创建了一个临时的 `shared_ptr` 赋予 process 的参数 ptr, p 和 ptr 都指向相同的 int 对象, 引用计数被正确的置为 2, process 函数执行完毕后, ptr 被销毁, 引用计数减 1, 这是正确的, 只有 p 指向它.

+   习题 12.11: 如果我们像下面这样调用 process, 会发生什么?

    ```cpp
    process(shared_ptr<int>(p.get()));
    ```

    +   此调用是错误的. `p.get()` 获得一个普通指针, 指向 p 所共享的 int 对象. 利用此指针创建一个 `shared_ptr`, 而不是利用 p 创建一个 `shared_ptr`, 将不会形成正确的动态对象共享. 编译器会认为 p 和 ptr 是使用两个地址 (虽然它们相等) 创建的两个不相干的 `shared_ptr`, 而非共享同一个动态对象. 这样, 两者的引用创建的两个不相干的 `shared_ptr`, 而非共享同一个动态对象. 这样, 两者的引用计数均为 1. 当 process 执行完毕后, ptr 的引用计数减为 0, 所管理的内存地址被释放, 而此内存就是 p 所管理的. p 成为一个管理空悬指针的 `shared_ptr`.

+   习题 12.12: p 和 sp 定义如下, 对于接下来的对 process 的每个调用, 如果合法, 解释它做了什么, 如果不合法, 解释错误的原因.

    ```cpp
    auto p = new int();
    auto sp = make_shared<int>();
    (a) process(sp);
    (b) process(new int());
    (c) process(p);
    (d) process(shared_ptr<int>(p));
    ```

    +   (a) 合法, sp 是一个共享指针, 指向一个 int 对象, 对 process 的调用会拷贝 sp, 传递给 process 的参数 ptr, 两者都指向相同的 int 对象
    +   (b) **合法**, new 创建了一个 int 对象, 指向它的指针被用来创建了一个 `shared_ptr`, 传递给 process 的参数 ptr. ((**我明白了, 这其实是调用了拷贝构造函数...!** 后面的话基本可以不看了.)**对于这里合法的问题, 我有一点思路, 我现在想不起来为什么可以这样传递参数, 只能暂时认为给函数传递实参实际上是对形参进行初始化的过程, 由于 `shared_ptr<int>(new int())` 是合法的, 而 `shared_ptr<int>(p)` 是不合法的. 回忆起某些函数 `func(const int &)`, 可以传递非 const 实参; 但是也有这样一种可能, 首先非 const 实参转换为 const 对象, 然后...**) 
    +   (c) 不合法. 不能将 `int*` 转换为 `shared_ptr<int>`.
    +   (d) **不合法, 但是程序是错误的**, p 是一个指向 int 对象的普通指针, 被用来创建一个临时的 `shared_ptr`, 传递给 process 的参数 ptr, 引用计数为 1. 当 process 执行完毕, ptr 被销毁, 引用计数变为 0. int 对象被销毁, p 变为空悬指针.

+   习题 12.13: 如果指向下面的代码, 会发生什么?

    ```cpp
    auto sp = make_shared<int>();
    auto p = sp.get();
    delete p;
    ```

    +   get 获取了 sp 指向的 int 对象的地址, 第三行用 delete 释放掉了这个地址, 这意味着 sp 的引用计数仍为 1, 但其指向的 int 对象已经释放掉了. sp 称为了类似空悬指针的 `shared_ptr`.

    ​

### 第 9 章 顺序容器 (sequential container)

+   一个容器就是一些特定类型对象的集合. 顺序容器为程序员提供了控制元素存储和访问顺序的能力. 

+   下面是标准库中的顺序容器, 所有的顺序容器都提供了快速顺序访问元素的能力, 但是这些容器在一下方面都有不同的性能折中:

    +   向容器中添加或从容器中删除元素的代价
    +   非顺序访问容器中的元素的代价

    ```cpp
    vector   // 可变大小数组, 支持快速随机访问, 在尾部之外的位置插入元素或删除元素可能很慢
    deque  // 双端队列, 支持快速随机访问, 在头尾位置插入/删除速度很快
    list   // 双向链表, 只支持双向顺序访问, 在 list 中任何位置进行插入/删除操作速度都很快
    forward_list // 单向链表, 只支持单向顺序访问, 在链表任何位置进行插入/删除操作速度都很快
    array // 固定大小数组, 支持快随随机访问, 不能添加或删除元素
    string // 与 vector 相似, 但专门用于保存字符. 随机访问快, 在尾部插入/删除速度快
    ```

    +   `forward_list` 和 `array` 是 C++11 标准增加的类型. 与内置数组相比, array 是一种更安全, 更容易使用的数组类型.

+   容器库概览

    +   一般来说, 容器库都定义在一个头文件总, 文件名和类型名相同. 比如 `#include <deque>`, `#include <list>`, 依次类推. 容器均定义为模板类, 我们必须提供额外的信息来生成特定的容器类型.

+   容器操作

    ```cpp
    // 类型别名
    iterator        // 容器的迭代器类型
    const_iterator  // 只可以读取元素的迭代器类型
    size_type       // 无符号整数类型, 足够保存此种容器类型最大可能容器的大小
    difference_type // 带符号整数类型, 足够保存两个迭代器之间的距离
    value_type      // 元素类型
    reference       // 元素的左值类型, 与 value_type& 含义相同
    const_reference // 元素的 const 左值类型, 

    // 构造函数
    C c;   // 默认构造函数, 构造空容器
    C c1(c2); // 构造 c2 拷贝 c1
    C c(b, e); // 构造 c, 将迭代器 b 和 e 指定范围内的元素拷贝到 c (array 不支持)
    C c{a, b, c...}; // 列表初始化 c

    // 赋值与 swap
    c1 = c2   // 将 c1 中的元素替换为 c2 中的元素
    c1 = {a, b, c...} // 将 c1 中的元素替换为列表中的元素 (不适用于 array)
    a.swap(b)  // 交换 a 和 b 的元素
    swap(a, b) // 与 a.swap(b) 等价

    // 大小
    c.size()  // c 中的元素的数目 (不支持 forward_list)
    c.max_size() // c 可保存的最大元素数目
    c.empty() // 若 c 中存储了元素, 返回 false, 否则返回 true

    // 添加/删除元素 (不支持 array)
    // 注: 在不同的容器中, 这些操作的接口都不同
    c.insert(args)  // 将 args 中的元素拷贝进 c
    c.emplace(inits) // 使用 inits 构造 c 中的一个元素
    c.erase(args) // 删除 args 指定的元素
    c.clear() // 删除 c 中的所有元素, 返回 void

    // 关系运算符
    ==, !=       // 所有容器都支持相等(不等) 运算符
    <, <=, >, >=  // 关系运算符, (无序容器不支持)

    // 获取迭代器
    c.begin(), c.end()  // 返回指向 c 的首元素和尾元素之后位置的迭代器
    c.cbegin(), c.cend() // 返回 const_iterator

    // 关联容器的额外成员(不支持 forward_list)
    reverse_iterator   // 按逆序寻址元素的迭代器
    const_reverse_iterator // 不能修改元素的逆序迭代器
    c.rbegin(), c.rend() // 返回指向 c 的尾元素和首元素之前位置的迭代器
    c.crbegin(), c.crend() // 返回 const_reverse_iterator
    ```

+   迭代器

    +   一个迭代器范围 (iterator range) 由一对迭代器表示, 两个迭代器分别指向同一个容器中的元素或者尾元素之后的位置 (one past the last element).
    +   左闭合区间 (left-inclusive interval), 其标准的数学描述为 `[begin, end)`.

+   **使用左闭合范围蕴含的编程假定**: 标准库使用左闭合范围是因为这种范围有 3 种方便的性质, 假定 begin 和 end 构成一个合法的迭代器范围, 则:

    +   如果 begin 和 end 相等, 则范围为空
    +   如果 begin 与 end 不等, 则范围至少包含一个元素, 且 begin 指向该范围中的第一个元素
    +   我们可以对 begin 递增若干次, 使得 `begin == end`

+   容器类型成员

    +   迭代器类型, 还包括反向迭代器, 此时 `++` 表示指向上一个元素
    +   类型别名: **通过类型别名, 我们可以在不了解容器中元素类型的情况下使用它.** 如果需要元素的类型, 可以使用容器的 `value_type`; 如果需要元素类型的一个引用, 可以使用 `reference` 或 `const_reference`. 这些元素相关的类型别名在泛型编程中非常有用.
    +   begin 和 end 成员



### 习题

+   习题 9.6: 下面程序有何错误, 应该如何修改它?

    ```cpp
    list<int> lst1;
    list<int>::iterator iter1 = lst1.begin(), iter2 = lst1.end();
    while (iter1 < iter2) /* ... */
    ```

    +   与 vector 和 deque 不同, list 的迭代器不支持 `<` 运算, 只支持递增, 递减, `==` 与 `!=` 运算.
    +   原因在于这几种数据结构实现上的不同, vector 和 deque 将元素在内存中连续保存, 而 list 则是将元素以链表的方式存储, 因此前者可以方便地实现迭代器的大小比较 (类似指针的大小比较) 来体现元素的前后关系. 而在 list 中, 两个指针的大小关系与它们指向的元素的前后关系并不一定是吻合的, 因此 `<` 运算将会非常困难和低效.

### 8.3 string 流

+    `#include <sstream>` 头文件中定义了 3 个类型来支持内存 IO, 这些类型可以向 string 写入数据, 从 string 读取数据, 就像 string 是一个 IO 流一样.

+   当我们的某些工作是对整行文本进行处理, 而另外一些工作是处理行内的单个单词时, 通常可以使用 `istringstream`: (书上 288 页写了个读取电话本的例子, 下面是我自己写的)

    ```cpp
    // 这是程序是在前一天的笔记中读取文本的程序进行修改的
    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <string>
    #include <vector>

    using namespace std;

    int main(int argc, const char *argv[]) {
        vector<string> svec;
        string line, str;
        if (argc > 1) {
            ifstream in(argv[1]);
            while (getline(in, line)) {  // 从文本中读取每一行
                istringstream record(line); // 使用 istringstream 来处理一行文本中的各个单词
                while (record >> str) {  // 将每一行中的各个单词分别存入 str 中
                    svec.push_back(str);
                }
            }
        } else {
            cout << "You forgot file name!" << endl;
        }

        for (auto &s : svec)
            cout << s <<  "...." << endl;

        return 0;
    }

    // g++ -Wall -std=c++0x -o main main.cpp 
    // ./main main.cpp
    ```

    +   `sstream strm(s);` strm 是一个 stream 对象, 保存 string s 的一个拷贝, 此构造函数是 explicit 的.
    +   `strm.str()` 返回 strm 所保存的 string 的拷贝
    +   `strm.str(s)` 将 string s 拷贝到 strm 中, 返回 void.

+   使用 ostringstream

    +   在我们逐步构造输出, 希望最后一起打印时, ostringstream 是很有用的.

    +   结合下面的习题 8.11 来学习 ostringstream:

        ```cpp
        // 这里给出的书上 289 页的例子
        for (const auto &entry : people) {
          // formatted, badNums 分别用于记录正确的电话号码(已经格式化的)
          // 以及错误的电话号码
          ostringstream formatted, badNums; // 每个循环时创建流对象
          for (const auto &nums : entry.phones) { // 对每个数
            if (!valid(nums)) {
              badNums << " " << nums;  // 将数的字符串形式存入 badNums
        	} else 
              formatted << " " << format(nums);
          }
          if (badNums.str().empty()) // .str() 返回的是一个字符串
            os << entry.name << " "  // 打印名字
            	<< formatted.str() << endl;  // 和格式化的数
          else // 否则, 打印名字和错误的数字的错误信息
            cerr << "input error: " << entry.name
            		<< " invalid number(s) " << badNums.str() << endl;
        }
        ```

        +   在此程序中我们假定已有两个函数 valid 和 format. 对于字符串流, 我们使用标准输出运算符 (<<) 向这些对象写入数据, 但这些 "写入" 操作实际上转换为 string 操作, 分别向 formatted 和 badNums 中的 string 对象添加字符.

        +   习题 8.14: 我们为什么将 entry 和 nums 定义为 `const auto&`?

            +   使用 const 表明在循环中不会改变这些项的值, 使用 auto 是请求编译器依据 vector 元素类型来推断出 entry 和 nums 的类型, 既简化了代码又避免了出错; 使用引用的原因是, people 和 phones 的元素分别是结构对象和字符串对象, 使用引用可以避免对象拷贝.

            ​

### 习题

+   习题 8.11: 本节的程序在外层 while 循环中定义了 istringstream 对象, 如果 record 对象定义在循环之外, 你需要对程序进行怎样的修改?

    +   **主要注意一点, 重复使用字符串流的时候, 每次都要调用 clear 方法来将流复位**.

    ```cpp
    // 原程序
    struct PersonInfo {
      string name;
      vector<string> phones;
    };
    string line, word;
    vector<PersonInfo> people;
    while(cin >> line) {
      PersonInfo info;
      istringstream record(line);
      record >> info.name;  // 读取名字
      while (record >> word) // 读取电话号码
        phones.push_back(word);
      people.push_back(info);  // 将此记录追加到 people 末尾
    }

    // 本题修改
    struct PersonInfo {
      string name;
      vector<string> phones;
    };
    string line, word;
    vector<PersonInfo> people;
    istringstream record; // 如果 istringstream 对象被移到了循环外面
    while(cin >> line) {
      PersonInfo info;
      record.clear(); // 重复使用字符串流的时候, 每次都要调用 clear
      record.str(line); // 通过 str 方法读取 line
      record >> info.name; 
      while (record >> word)
        phones.push_back(word);
      people.push_back(info); 
    }
    ```


+   习题 8.12: 为什么我们没有在 PersonInfo 中使用类内初始化?

    +   由于每个人的电话号码数量不固定, 因此更好的办法不是通过类内初始化指定人名和所有电话号码, 而是在缺省初始化之后, 在程序中设置人名并逐个添加电话号码.



##  2017 年 8 月 6 日

### 8.1 IO 类

+   目前使用过的 IO 类型和对象都是操作 char 类型的, 默认情况下, 这些对象都是关联到用户的控制台窗口的. 但应用程序常常需要读写命名文件, 而且可能还需要读写宽字符支持的语言. 为了支持这些不同之类的 IO 处理操作, 标准库还定义了其他的 IO 类型. 下面是三个独立的头文件

    +   `#include <iostream>` 定义了用于读写流的基本类型
    +   `#include <fstream>` 定义了读写文件的类型
    +   `#include <sstream>` 定义了读写内存 string 对象的类型.

+   继承机制: 我们通常可以将一个派生类对象当做基类对象来使用. 比如 `ifstream` 和 `istringstream` 都继承自 `istream`, 我们是怎样使用 `cin` 的, 就可以怎样使用这些对象.

+   **IO 对象无拷贝或赋值, 由于不能拷贝 IO 对象, 因此我们也不能将形参或返回类型设置为流类型. 进行 IO 操作的函数通常以引用方式传递和返回流. 读写一个 IO 对象会改变其状态, 因此传递和返回的引用不能是 const 的.**

+   条件状态: (具体的一些状态查书, 280 页)

    +   一个流一旦发生错误, 其上后续的 IO 操作都会失败. 只有当一个流处于无错状态时, 我们才可以从它读取数据, 向它写入数据. **由于流可能处于错误状态, 因此代码通常应该在使用流之前检查它是否处于良好状态.** 确定一个流对象的状态最简单的方法是将它当做一个条件来使用:

        ```cpp
        while (cin >> word)
          // ok, 读操作成功
        ```

        while 循环检查 `>>` 表达式返回的流的状态. 如果输入操作成功, 流保持有效状态, 则条件为真.

    +   查询流的状态:

        +   将流作为条件使用, 只能知道它是否有效, 有时我们需要知道流为什么失败.
        +   IO 库定义了一个与机器无关的 iostate 类型.

    +   管理条件状态

        +   流对象的 rdstate 成员返回一个 iostate 值.

+   管理输出缓冲

    +   每个输出流都管理一个缓冲区, 用来保存程序读写的数据.

    +   刷新输出缓冲区

        +   `endl` : 完成换行并刷新缓冲区的工作
        +   `flush`: 刷新缓冲区但不输出任何额外的字符
        +   `ends`: 向缓冲区插入一个空字符, 然后刷新缓冲区

    +   `unitbuf` 操作符

        +   如果想在每次输出操作后都刷新缓冲区, 可以使用 `unitbuf` 操作符. 它告诉流在接下来的每次写操作之后都进行一次 flush 操作, 而 `nounitbuf` 操作符则重置流, 使其恢复使用正常的系统管理的缓冲区刷新机制:

            ```cpp
            cout << unitbuf;  // 所有输出操作后都会立即刷新缓冲区
            // 任何输出都会立即刷新, 无缓冲
            cout << nounitbuf; // 回到正常的缓冲方式
            ```

        +   警告: 如果程序崩溃, 输出缓冲区不会刷新

+   关联输入和输出流

    +   标准库已经将 cin 和 cout 关联到一起, `cin.tie(&cout)`
    +   当一个输入流被关联到一个输出流时, 任何试图从输入流读取数据的操作都会先刷新关联的输出流.
    +   每个流同时最多关联到一个流, 但多个流可以同时关联到同一个 `ostream`

+   文件输入输出

    +   `#include <fstream>` 头文件定义了三个类型支持文件 IO:

        +   `ifstream` 从一个给定文件读取数据
        +   `ofstream` 向一个给定文件写入数据
        +   `fstream` 可以读写给定文件

    +   使用文件流对象: 

        +   每个文件流对象都定义了一个 open 成员函数.

        +   创建文件流对象时, 我们可以提供文件名 (可选); 如果提供了一个文件名, open 会自动调用:

            ```cpp
            ifstream in(ifile);  // 构造一个 ifstream 并打开给定文件, 此处, ifile 为 string
            ofstream out;  // 输出文件流, 但未关联到任何文件
            ```

    +   用 fstream 代替 `iostream&`

        +   这就要注意继承的关系, 本书给出的书店销售记录的例子中, 曾写过 `read` 和 `print` 函数, 分别接受 `istream&` 和 `ostream&` 对象, 此时也可以将 `ifstream` 对象和 `ofstream` 对象分别传递给这些参数.

            ```cpp
            ifstream input(argv[1]);
            ofstream output(argv[2]); // 输入输出文件名通过 main 程序读入

            print(output, total); // 回忆: total 为 Sales_data 对象
            read(input, total); // read(istream &is, const Sales_data &item);
            // 虽然 read 接受的是 istream & 类型的参数, 但由于继承关系, 
            // 也能接受 ifstream 对象.
            ```

    +   成员函数 open 和 close

        +   因为调用 open 可以会失败, 进行 open 是否成功的检测通常是个好习惯:

            ```cpp
            ifstream in;
            in.open(ifile);
            if (in.is_open()) // 检测 open 是否成功
            ```

        +   为了将文件流关联到另外一个文件, 必须首先关闭已经关联的文件. 一旦文件成功关闭, 我们可以打开新的文件.

    +   自动构造和析构

        +   如下程序, main 函数接受一个要处理的文件列表

            ```cpp
            for (auto p = argv + 1; p != argv + argc; ++p) {
              ifstream input(*p);  // 创建输出流并打开文件
              if (input) {  // 如果文件打开成功, 处理该文件
                process(input);
              } else {
                cerr << "couldn't open: " + string(*p);
              }
            } // 每个循环 input 都会离开作用域, 因此会被销毁
            ```

            +   注意, 由于在每次迭代中都构造一个新的名为 input 的 ifstream 对象, 它是循环的局部变量, 在每次迭代过程中都要创建和销毁一次. 当一个 fstream 对象离开其作用域时, 与之关联的文件会自动关闭. **当一个 fstream 对象被销毁时, close 会自动调用.**

+   文件模式

    +   每个文件流类型都定义了一个默认的文件模式. 与 ifstream 关联的文件默认以 in 模式打开; 与 ofstream 关联的文件默认以 out 模式打开; 与 fstream 关联的文件默认以 in 和 out 模式打开.

    +   以 out 模式打开文件会丢弃已有的数据: 默认情况下, 当我们打开一个 ofstream 时, 文件的内容会被丢弃. 阻止一个 ofstream 清空给定文件内容的方法是同时指定 app 模式:

        ```cpp
        // 在这几条语句中, file1 都被截断
        ofstream out("file1"); // 隐含以输出模式打开文件并截断文件
        ofstream out2("file1", ofstream::out); // 隐含地截断文件
        ofstream out3("file1", ofstream::out | ofstream::trunc);

        // 为了保留文件的内容, 我们必须显式指定 app 模式
        ofstream app("file2", ofstream::app); // 隐含为输出模式
        ofstream app2("file2", ofstream::out | ofstream::app);
        ```

    +   通常情况下, out 模式意味着同时使用 trunc 模式.

### 习题

+   习题 8.4: 编写函数, 以读模式打开一个文件, 将其内容读入到一个 string 的 vector 中, 将每一行作为一个独立的元素存于 vector 中.

+   习题 8.5: 重写上面的程序, 将每个单词作为一个独立的元素进行存储

    ```cpp
    #include <iostream>
    #include <fstream>
    #include <string>
    #include <vector>

    using namespace std;

    int main(int argc, const char *argv[]) {
        vector<string> svec;
        string str;
        if (argc > 1) {       // 要求输入文件名
            ifstream in(argv[1]);  // 打开文件
            if (in.is_open()) {  // 判断是否成功打开
                //while (in >> str)     // 将 "每个单词" 作为独立的元素存储
                while (getline(in, str))  // 将 "每行" 作为一个独立的元素存储
                    svec.push_back(str);
            } else {
                cerr << "Couldn't open file: " + string(argv[1]) << endl;
            }
        } else {
            cout << "You forgot file name!" << endl;
        }

        for (auto &s : svec)
            cout << s <<  "...." << endl;  // 输出每个元素, (后面带些 ....)

        return 0;
    }
    ```

    ​

## 2017 年 8 月 5 日

### 第 8 章 IO 库

+   我想还是按照顺序来看吧, 老老实实地看掉这些内容, 跳着看其实有很多内容理解起来很吃力. 特别是后面的内容, 大段的文字描述, 如果有一定的实践经验, 后面那些叙述才能有比较深刻的体会...
+   现在基础看完了, 应该搞点事情了. 不要昏昏沉沉地看书...

### 第 13 章 拷贝控制

+   当定义一个类时, 我们显式地或隐式地指定在此类型的对象拷贝, 移动, 赋值和销毁时做什么. 一个类通过定义五种特殊的成员函数来控制这些操作, 包括:

    +   拷贝构造函数 (copy constructor)
    +   拷贝赋值运算符 (copy-assignment operator)
    +   移动构造函数 (move constructor)
    +   移动赋值运算符 (move-assignment operator)
    +   析构函数 (destructor)

+   **拷贝和移动构造函数定义了当用同类型的另一个对象初始化本对象时做什么, 拷贝和移动赋值运算符定义了讲一个对象赋予同类型的另一个对象时做什么. 析构函数定义了当此类型对象销毁时做什么. 我们称这些操作为拷贝控制操作.**

+   拷贝构造函数

    +   如果一个构造函数的第一个参数是自身类类型的引用, 且任何额外参数都有默认值,(指如果有其他的函数参数的话, 必须要有默认值) 则此构造函数是拷贝构造函数.

        ```cpp
        class Foo{
        public:
          Foo();  // 默认构造函数
          Foo(const Foo&);  //拷贝构造函数
        };
        ```

        +   **拷贝构造函数的第一个参数必须是一个引用类型**, 否则(如果不是引用类型的话)会引发无限循环, 因为为了调用拷贝构造函数, 我么必须拷贝它的实参, 但我们又要调用拷贝构造函数...
        +   拷贝构造函数在几种情况下都会被隐式的使用, 通常不应该是 explicit 的.

    +   合成拷贝构造函数: 编译器生成的, 和默认构造函数不同的是, 即使我们定义了其他的构造函数, 编译器也会为我们合成一个拷贝构造函数. 一般情况下, 合成拷贝构造函数会将其参数的成员逐个拷贝到正在创建的对象中. 编译器从给定的对象中依次将每个非 static 成员拷贝到正在创建的对象中.

    +   **每个成员的类型决定了它如何拷贝: 对类类型的成员, 会使用其拷贝构造函数来拷贝; 内置类型的成员则直接拷贝. 虽然我们不能直接拷贝一个数组, 但合成拷贝构造函数会逐元素地拷贝一个数组类型的成员. 如果数组元素是类类型, 则使用元素的拷贝构造函数来进行拷贝.**

    +   拷贝初始化与直接初始化

        +   一定要看博客 [C++的一大误区--深入解释直接初始化和拷贝初始化的区别](http://blog.csdn.net/ljianhui/article/details/9245661)
        +   它们之间的差异是, **当使用直接初始化时, 我们实际上是要求编译器使用普通的函数匹配来选择与我们提供的参数最匹配的构造函数.** 当我们使用拷贝构造函数时, 我们要求编译器将右侧对象拷贝到正在创建的对象中. 如果需要的话, 还需要进行类型转换. 
        +   **拷贝初始化通常是依靠拷贝构造函数或移动构造函数来完成的.**
        +   拷贝初始化不仅在我们使用 `=` 定义变量时会发生, 在下列情况下也会发生:
            +   将一个对象作为实参传递给一个非引用类型的形参
            +   从一个返回类型为非引用类型的函数返回一个对象
            +   用花括号列表初始化一个数组中的元素或一个聚合类中的成员

    +   参数和返回值

        +   **在函数调用过程中, 具有非引用类型的参数要进行拷贝初始化**. 类似的, 当一个函数具有非引用的返回类型时, 返回值会被用来初始化调用方的结果. 
        +   拷贝构造函数被用来初始化非引用类类型参数, 这一特性解释了为什么拷贝构造函数自己的参数必须是引用类型. 如果其参数不是引用类型, 则调用永远不会成功 -- 为了调用拷贝构造函数, 我们必须调用它的实参, 但为了拷贝实参, 我们有需要调用拷贝构造函数, 如此无限循环.

    +   拷贝初始化的限制

        ```cpp
        vector<int> v1(10);  // 正确: 直接初始化
        vector<int> v2 = 10; // 错误: 接受大小参数的构造函数是 explicit 的
        ```

        +   注意 explicit 
        +   此处的问题可以参考书上 265 页: explicit 构造函数只能用于直接初始化

    +   编译器可以绕过拷贝构造函数

        ```cpp
        string null_book = "9-999-9999";  // 拷贝初始化
        // 可以改写为
        string null_book("9-999-9999"); // 编译器略过了拷贝构造函数
        ```

        +   注意, string 类中必然定义了将 `const char*` 转换为 string

        ​

### 7.5 构造函数再探

+   当我们定义变量时, 习惯立即对其进行初始化, 而非先定义, 再赋值;

    ```cpp
    string foo = "hello";  // 定义并初始化
    string bar;    // 默认初始化为 string 对象
    bar = = "hello";   // 为 bar 赋一个新值
    ```

    如果没有在构造函数的初始值列表中显式的初始化成员, 则该成员将在构造函数体之前执行默认初始化. 如:

    ```cpp
    // Sales_data 的构造函数的一种写法, 虽然合法但是比较草率:
    // 没有使用构造函数的初始值
    Sales_data::Sales_data(const string &s, unsigned cnt, double price){
      bookNo = s;
      units_sold = cnt;
      revenue = cnt * price;
    }
    ```

    它和使用构造函数初始值列表进行初始化的区别是: 这个版本的构造函数是对数据进行赋值操作. 这一区别到底会有什么深层次的影响, 完全依赖于数据成员的类型.

+   构造函数的初始值有时必不可少

    +   比如, 如果成员是 const 或者引用的话, 必须将其进行初始化. 类似的, 当成员属于某种类型, 且该类型没有定义默认构造函数时, 也必须将这个成员初始化. 比如:

        ```cpp
        class ConstRef{
        public:
          ConstRef(int ii);
        private:
          int i;
          const int ci;  // 注意: ci 的常量性质是在构造函数初始化完成后才获得的
          int &ri;
        };

        // 错误: ci 和 ri 必须被初始化
        ConstRef::ConstRef(int ii){
          // 下面是赋值而不是初始化
           i = ii;   // 正确
          ci = ii;  // 错误: 不能给 const 赋值
          ri =  i;   // 错误: ri 没被初始化
        }
        ```

        +   随着构造函数体一开始执行, 初始化就完成了. 因此我们初始化 const 或者引用的唯一机会就是通过构造函数的初始值. 因此正确的方法是:

            ```cpp
            // 正确: 显式的初始化引用和 const 成员
            ConstRef::ConstRef(int ii): i(ii), ci(ii), ri(i) {}
            ```

        +   总之, 如果成员是 const, 引用个, 或者属于某种未提供默认构造函数的类类型, 我们必须通过构造函数初始值列表为这些成员提供初值.

        +   建议: **养成使用构造函数初始值的习惯**

+   成员初始化的顺序

    +   成员初始化的顺序与它们在类定义中出现的顺序一致: 第一个成员先被从初始化, 然后第二个, 以此类推. **构造函数初始值列表中初始值的前后位置关系不会影响实际的初始化顺序.** 但是最好令构造函数和初始值的顺序与成员声明的顺序保持一致. 而且如果可能的话, 尽量避免使用某些成员初始化其他成员.

+   **默认实参和构造函数**

    +   **如果一个构造函数为所有的参数都提供了默认实参, 则实际上也定义了默认构造函数**

        ```cpp
        // 
        class Sales_data {
        public:
          // 定义默认构造函数, 令其与只接受一个 string 实参的构造函数功能相同
          Sales_data(std::string s = " "): bookNo(s) {}
        private:
          std::string bookNo;
          unsigned units_sold = 0;
          double price = 0.0;
        };
        ```

        +   上面的构造函数如果没有给定实参, 或者给定了一个 string 实参, 两个版本的类创建了相同的对象. 因为我们不提供实参也能调用上述的构造函数, 所以该构造函数实际上为我们的类提供了默认构造函数.

    +   注意出现二义性错误的问题, 查看习题 7.38 和 39

+   委托构造函数

    +   C++11 新标准扩展了构造函数初始值的功能, 使得我们可以定义委托构造函数 (delegating constructor).  一个委托构造函数使用它所属类的其他构造函数执行它自己的初始化过程, 或者说它把它自己的一些 (或者全部) 职责委托给了其他构造函数.

    +   我们使用委托构造函数重写 `Sales_data` 类:

        ```cpp
        class Sales_data {
        public:
          // 非委托构造函数使用对应的实参初始化成员
          Sales_data(std::string s, unsigned cnt, double price): 
          			bookNo(s), units_sold(cnt), revenue(cnt * price) {}
          // 其余构造函数全部委托给另一个构造函数, 下面都是委托构造函数
          Sales_data(): Sales_data("", 0, 0) {}
          Sales_data(std::string s): Sales_data(s, 0, 0) {}
          Sales_data(std::istream &is): Sales_data() { read(is, *this); }
        private:
          std::string bookNo;
          unsigned units_sold = 0;
          double revenue = 0.0;
        };
        ```

        +   上面的例子中, 一个委托构造函数也有一个成员初始值列表和一个函数体. 在委托构造函数内, 成员初始值列表只有一个唯一的入口, 就是类名本身. 和其他成员初始值一样, 类名后面紧跟圆括号括起来的参数列表, 参数列表必须与类中另一个构造函数匹配.
        +   当一个构造函数委托给另一个构造函数时, 受委托的构造函数的初始值列表和函数体被依次执行. 在 `Sales_data` 类中, 受委托的构造函数恰好是空的, 假如函数体包含有代码的话, 将先执行这些代码, 然后控制权才会交还给委托者的函数体.

+   默认构造函数的作用

    +   这一节还需要再研究, 说明了在何种情况下对象会被默认初始化或者值初始化. 
    +   得到的经验是, 如果定义了其他构造函数, 那么最好也提供一个默认构造函数.

+   使用默认构造函数

    ```cpp
    // 注意比较下面声明的不同
    Sales_data obj;  // 声明一个使用默认构造函数初始化的对象 
    Sales_data obj(); // 这个是定义了一个函数, 接受空形参列表, 返回 Sales_data 类型...
    ```

    +   请注意声明使用默认构造函数初始化对象的时候, 对象名字后面不要加上圆括号.

+   隐式的类类型转换

    +   如果构造函数只接受一个实参, 则它实际上定义了转换为此类类型的隐式转换机制, 有时我们把这种构造函数称为**转换构造函数**.

        ```cpp
        string null_book = "9-999-99999-9";
        // 构造一个临时的 Sales_data 对象
        // 该对象 units_sold 和 revenue 为 0, bookNo 等于 null_book
        item.combine(null_book);  // 注意这里 item 是 Sales_data 类型, 而 null_book 是 string 类型

        // 但是下面不行: 因为只允许一步类类型转换
        item.combine("9-999-99999-9");

        // 使用 istream 的构造函数创建一个对象传递给 combine
        item.combine(cin);
        ```

        +   在 `Sales_data` 类中, 接受 string 的构造函数和接受 istream 的构造函数分别定义了从这两种类型向 `Sales_data` 隐式转换的规则. 也就是说, 在需要使用 `Sales_data` 的地方, 我们可以使用 string 或这 istream 作为替代.
        +   上面 `combine` 调用合法的理由是, 编译器用给定的 string 自动创建了一个 `Sales_data` 对象, 新生成的这个 `Sales_data` 对象被传递给 combine, 因为 combine 的参数是一个常量引用, 所以我们可以给该参数传递一个临时量.
        +   还有就是要注意编译器**只允许一步类类型转换**. 上面的第二个例子中, 需要两步转化. 首先 "9-999-99999-9" 转换为 string, 然后 string 转换为 `Sales_data`.

+   **抑制构造函数定义的隐式转换**

    +   在要求隐式转换的程序上下文中, 我们可以通过将构造函数声明为 **explicit** 加以阻止:

        ```cpp
        class Sales_data {
        public:
          Sales_data() = default;
          Sales_data(std::string s, unsigned cnt, double price): 
          			bookNo(s), units_sold(cnt), revenue(cnt * price) {}
          explicit Sales_data(const std::string &s): bookNo(s) {}
          explicit Sales_data(std::istream &is);
        };

        // 此时, 之前的两种用法都无法通过编译
        item.combine(null_book);   // 错误: string 构造函数是 explicit 的
        item.combine(cin); // 错误: istream 构造函数是 explicit 的

        // 错误: explicit 关键字只允许出现在类内构造函数的声明处
        explicit Sales_data::Sales_data(istream &is){
          read(is, *this);
        }
        ```

        +   此时没有任何构造函数能够用于隐式创建 `Sales_data` 对象!

        +   **关键字 explicit 只对一个实参的构造函数有效, 需要多个实参的构造函数不能用于执行隐式转换, 所以无须将这些构造函数指定为 explicit 的.**

        +   **只能在类内声明构造函数时使用 explicit 关键字, 在类外部定义时不应重复**.

        +   **explicit 构造函数只能以直接初始化的形式使用** (发生隐式转换的一种情况是当我们执行拷贝形式的初始化时(使用 =), 此时我们只能使用直接初始化来使用 explicit 构造函数)

            ```cpp
            Sales_data item1(null_book);  // 正确: 直接初始化
            // 错误: 不能直接将 explicit 构造函数用于拷贝形式的初始化过程
            Sales_data item2 = null_book;
            ```

        +   尽管编译器不会将 explicit 的构造函数用于隐式转换过程, 但是我们可以使用强制转换来使用 explicit 构造函数

            ```cpp
            // 正确
            item.combine(Sales_data(null_book));
            // 正确: static_cast 可以使用 explicit 构造函数
            item.combine(static_cast<Sales_data>(cin));
            ```

+   **注意习题 7.49**: [**非常量引用不能绑定临时对象**](http://www.cnblogs.com/BensonLaur/p/5234555.html) 

    +   这是因为以C++的语义来说，如果一个程序员只想传递参数给函数，而不希望函数修改传入的参数时，那么，或者使用值传递，或者采用常量型引用。考虑到大对象复制时产生的开销，一般使用常量型引用 `const &`。如果函数的参数是某个类型的一个非常量的引用，那就相当于告诉编译器，程序员希望得到函数对参数的修改结果。
        临时变量是由编译器生成的，C++语言规范没规定编译器生成临时变量的规则，程序员无法得知由编译器生成的临时变量的名字，程序员无法访问那个临时变量。这意味着，以引用的方式传递一个临时变量做为函数参数，如果函数内部对此临时变量做了修改，那么函数返回后，程序员无法获得函数对临时变量的修改。函数对临时变量所做出的所有更改，都将丢失。
        一方面，在函数申明中，使用非常量型的引用告诉编译器你需要得到函数对某个对象的修改结果，可是你自己又不给变量起名字，直接丢弃了函数的修改结果，编译器只能说：“大哥，你这是干啥呢，告诉我把结果给你，等我把结果给你了，你又直接给扔了，你这不是在玩我吗？”

        同时，C++的标准为了防止给常量或临时变量（只有瞬间的生命周期）赋值(易产生bug)，只许使用 const 引用之。

+   聚合类

    +   感觉和 struct 很像....

+   字面值常量类

    +   constexpr 函数的参数和返回值必须是字面值类型.
    +   余略...

+   类的静态成员

    +   有的时候类需要它的一些成员与类本身直接相关, 而不是与类的各个对象保持关联. 

    +   在成员声明之前加上 `static` 关键字. 类的静态成员存在于任何对象之外, 对象中不包含任何与静态数据成员有关的数据.

    +   静态成员函数也不与任何对象绑定在一起, 它们不包含 `this` 指针. 作为结果, 静态成员函数不能声明成 const 的, 而且我们也不能在 static 函数体内使用 this 指针.

    +   我们可以**使用作用域运算符直接访问类的静态成员**

        ```cpp
        double r;
        r = Account::rate(); // 使用作用域运算符访问静态成员
        ```

        +   **虽然静态成员不属于类的某个对象, 但是我们仍然可以使用类的对象, 引用或者指针来访问静态成员.**
        +   **成员函数不用通过作用域运算符就能直接使用静态成员.**

    +   在类外部定义静态成员时不能重复使用 static 关键字, 该关键字只出现自类内部的声明中. 因为静态数据成员不属于类的任何一个对象, 所以它们并不是在创建类的对象时被定义的. 这意味着它们不是由类的构造函数初始化的. 而且一般来说, 我们不能在类的内部初始化静态成员, 相反的, **我们必须在类的外部定义和初始化每个静态成员**. 类似于全局变量, 静态数据成员定义在任何函数之外, 因此一旦它被定义, 就将一直存在于程序的整个生命周期中.

+   静态成员的类内初始化

    +   牵扯到 constexpr, 这里看得不太懂.
    +   即使一个常量静态数据成员在类内部被初始化了, 通常情况下也应该在类外部定义一下该成员.

+   静态成员能用于某些场景, 而普通成员不能:

    +   静态数据成员的类型可以就是它所属的类类型, 而非静态数据成员则受到限制, 只能声明成它所属类的指针或引用.
    +   静态成员和普通成员的另一个区别是我们可以使用静态成员作为默认实参. 非静态数据成员不能作为默认实参, 因为它的值本身属于对象的一部分, 这么做的结果是无法真正提供一个对象以便从中获取成员的值, 最终将引发错误.

    ​

### 习题

+   习题 7.38: 有些情况下, 我们希望提供 cin 作为接受 `istream&` 参数的构造函数的默认实参, 请声明这样的构造函数:

    ```cpp
    Sales_data::Sales_data(std::istream &is = std::cin) { is >> *this; }
    ```

    +   **此时该函数具有了默认构造函数的功能, 因此我们原来声明的默认构造函数 `Sales_data()=default;` 应该去掉, 否则会出现二义性错误.**

+   习题 7.39: 如果接受 string 的构造函数和接受 `istream&` 的构造函数都能使用默认实参, 这种行为合法吗? 如果不, 为什么?

    +   不合法
    +   由于它们都使用了默认实参, 那么当使用默认构造函数初始化对象的时候, 就不知道要调用哪个默认构造函数了. (参考答案提到: 出现二义性错误).

+   习题 7.49: 对于 combine 函数的三种不同声明, 当我们调用 `i.combine(s)` 时分别发生什么情况? 其中 `i` 是一个 `Sales_data`, 而 `s` 是一个 string 对象.

    ```cpp
    (a) Sales_data &combine(Sales_data);
    (b) Sales_data &combine(Sales_data&);
    (c) Sales_data &combine(const Sales_data&) const;
    ```

    +   (a) 正常编译 (此时是值传递)
    +   (b) 无法正常编译: 因为 combine 函数的参数是一个非常量引用, 而 s 是一个 string 对象, 编译器用 s 创建一个 `Sales_data` 临时对象, 但是这个新生成的临时对象无法传递给 combine 所需的非常量引用. 如果声明为 `Sales_data &combine(const Sales_data&);` 才行.
    +   (c) 不能正常编译, 此时 combine 被声明为常量成员函数, 无法修改数据成员的值.

+   习题 7.50: vector 将其单参数的构造函数定义成 explicit 的, 而 string 则不是, 你觉得原因何在?

    +   接受一个单参数的 `const char*` 的 string 构造函数不是 explicit 的
    +   接受一个容量参数的 vector 构造函数是 explicit 的.

    答:

    出题思路: **从参数类型到类类型的自动转换是否有意义依赖于程序员的看法, 如果这种转换是自然而然的, 则不应该把它定义成 explicit 的, 如果二者的语义距离较远, 则为了避免不必要的转换, 应该指定对应的构造函数是 explicit 的.**

    +   string 接受的单参数是 `const char*` 类型, 如果我们得到一个常量字符指针(字符数组), 则把它看做 string 对象那个是自然而然的过程, 编译器自动把参数类型转换成类类型也非常符合逻辑, 因此我们无须指定 explicit.
    +   与 string 相反, vector 接受的单参数是 int 类型, 这个参数的原意是指定 vector 的容量, 如果我们在本来需要 vector 的地方提供一个 int 值并且希望这个 int 值自动转换为 vector, 则这个过程显得比较牵强, 因此将其设置为 explicit 显得更加合理.



## 2017 年 8 月 4 日

### 第 7 章 类

+   类的基本思想是数据抽象 (data abstraction) 和封装 (encapsulation)

    +   数据抽象: 依赖于接口 (interface) 和实现 (implementation) 分离
    +   封装实现了类的接口和实现分离, 封装后的类隐藏了它的实现细节.

+   成员函数的声明必须放在类的内部, 它的定义既可以在类的内部也可以在类的外部. 

    +   **定义在类内部的函数是隐式的 inline 函数.**

+   引入 this

    +   成员函数通过一个名为 `this` 的额外的隐式参数来访问调用它的那个对象. 当我们调用一个成员函数时, 用请求该函数的对象地址初始化 `this`, 例如:

        ```cpp
        total.isbn()
        ```

        则编译器负责把 total 的地址传递给 isbn 的隐式形参 `this`, 可以等价地认为编译器将该调用重写成如下的形式:

        ```cpp
        // 伪代码, 用于说明调用成员函数的实际执行过程
        Sales_data::isbn(&total)  // 调用 Sales_data 的 isbn 成员时传入了 total 的地址
        ```

        +   **在成员函数内部, 我们可以直接使用调用该函数的对象的成员, 而无须通过成员运算符来做到这一点, 因为 this 所指的正是这个对象. 任何对类成员的直接访问都被看成 this 的隐式引用.**
        +   由于 `this` 是隐式定义的, 任何自定义名为 this 的参数或变量的行为都是非法的.
        +   `this` 是一个常量指针, 我们不允许改变 this 中保存的地址. (顶层 const)

+   引入 const 成员函数

    isbn 函数定义是

    ```cpp
    std::string isbn() const { return bookNo; }
    ```

    +   这里的 const 的作用是修改隐式 this 指针的类型.

    +   默认情况下, this 的类型是指向类类型非常量版本的常量指针 (`Sales_data* const this`), **尽管 this 是隐式的, 但它仍要遵循初始化规则, 意味着 (在默认情况下) 我们不能把 this 绑定到一个常量对象上**. 这一情况也就使我们不能在一个常量对象上调用普通的成员函数.

        ```cpp
        // 伪代码, 我随意勾勒出来的
        // 默认情况下, isbn
        std::string isbn(Sales_data* const this) { return this->bookNo; }

        // 默认情况下, 常量对象无法调用 isbn, 因此我们需要
        std::string isbn(const Sales_data* const this) { return this->bookNo; }

        // 由于在 isbn 内不会改变 this 所指的对象, 所以将 isbn 声明为
        // 指向常量的指针有助于提高函数的灵活性.
        // 然而 this 是隐式的并且不会出现在参数列表中, 所以 C++ 允许把
        // const 关键字放在成员函数的参数列表后面.
        ```

        +   **由于在 isbn 内不会改变 this 所指的对象, 所以将 isbn 声明为指向常量的指针有助于提高函数的灵活性.然而 this 是隐式的并且不会出现在参数列表中, 所以 C++ 允许把 const 关键字放在成员函数的参数列表后面.** 
        +   像这样使用了 const 的成员函数被称为**常量成员函数**.
        +   **常量对象, 以及常量对象的引用和指针都只能调用常量成员函数**

+   在类外部定义成员函数:

    +   **必须包含它所属的类名**
    +   `Sales_data::avg_price` 使用了作用域运算符, 说明该函数是被声明在类 `Sales_data` 的作用域内的. 一旦编译器看到这个函数名, 就能理解剩余的代码是位于类的作用域内的.

+   定义一个返回 this 对象的函数

    +   比如 combine 函数的设计初衷是类似与 `+=`, 由于内置的赋值运算符把它的左侧运算对象当成左值返回, 因此为了与内置的运算符行为一直, combine **必须返回引用类型**. 因为此时左侧运算对象是一个 `Sales_data` 的对象, 所以返回类型应该是 `Sales_data&`. 

        ```cpp
        Sales_data& Sales_data::combine(const Sales_data &rhs){
          units_sold += rhs.units_sold; // 把 rhs 的成员加到 this 对象的成员上
          revenue += rhs.revenue;
          return *this;  // 返回调用该函数的对象
        }
        ```

+   定义类相关的非成员函数

    +   类的作者经常需要定义一些辅助函数, 比如 `add`, `print`, `read` 等, 尽管这些函数定义的操作从概念上来说属于类的接口的组成部分, 但它们实际上并不属于类本身.

    +   一般来说, 如果非成员函数是类接口的组成部分, 则这些函数的声明应该与类在同一个头文件中.

    +   定义 read 和 print 函数

        ```cpp
        istream &read(istream &is, Sales_data &item){
          double price = 0;
          is >> item.bookNo >> item.units_sold >> price;
          item.revenue = price * item.units_sold;
          return is;
        }

        ostream &print(ostream &os, const Sales_data &item){
          os << item.isbn() << " " << item.units_sold << " "
            << item.revenue << " " << item.avg_price();
          return os;
        }
        ```

        关于上面的函数有两点非常重要:

        +   首先它们接受一个各自 IO 类型的引用作为其参数, 这是因为 IO 类属于不能被拷贝的类型, 因此我们只能通过引用来传递它们. 而且因为读取和写入操作会改变流的内容, 所以两个函数接受的都是普通引用, 而非对常量的引用.
        +   第二点, print 函数不负责换行, 一般来说, 执行输出任务的函数应该尽量减少对格式的控制, 这样可以确保由用户代码来决定是否换行.

+   构造函数

    +   构造函数的任务是初始化类的数据成员, 无论何时只要类的对象被创建, 就会执行构造函数.
    +   **不同于其他成员函数, 构造函数不能被声明成 const 的! 当我们创建类的一个 const 对象时, 直到构造函数完成初始化过程, 对象才真正取得其 "常量" 属性**. 因此构造函数在 const 对象的构造过程中可以向其写值.
    +   默认构造函数 (default constructor) 无须任何实参.
        +   如果我们的类没有显示地定义构造函数, 那么编译器就会为我们隐式地定义一个默认构造函数, 称为合成的默认构造函数.
        +   **注意: 只有当类没有声明任何构造函数时, 编译器才会自动地生成默认构造函数**. 也就是说, 一旦我们定义了一些其他的构造函数, 那么除非我们再定义一个默认的构造函数, 否则类将没有默认构造函数.
        +   如果类包含有内置类型或者复合类型(比如数组和指针)的成员, 只有当这些成员全都被赋予了类内的初始值, 这个类才适合于使用合成的默认构造函数.
        +   ... 一堆的原因, 总之就是说明最好使用自己定义的构造函数...

+   **`= default` 的含义**

    ```cpp
    Sales_data() = default;
    ```

    +   这是一个默认构造函数 (不接受任何实参), 我们希望这个函数的作用完全等同于合成默认构造函数 (编译器帮忙生成的), 在 C++11 新标准中, 如果我们需要默认的行为, 可以通过在参数列表后面写上 `= default` 来要求编译器生成构造函数. 
    +   其中 `= default` 即可以和声明一起出现在类的内部, 也可以作为定义出现在类的外部. 和其他函数一样, 如果 `= default` 在类的内部, 则默认构造函数是内联的; 如果它在类的外部, 则成员默认情况下不是内联的.

+   **构造函数初始值列表**

    +   首先看另外两个构造函数:

        ```cpp
        Sales_data(const std::string &s): bookNo(s) {}
        Sales_data(const std::string &s, unsigned n, double p): 
        	bookNo(s), units_sold(n), revenue(p*n) {}
        ```

    +   冒号和以及冒号和花括号之间的代码, 我们称之为构造函数初始值列表. 它负责为新创建的对象的一个或几个数据成员赋初值. 构造函数初始值是成员名字的一个列表, 每个名字后面紧跟括号括起来的成员初始值. 不同的成员的初始化通过逗号分隔开来.

+   **在类的外部定义构造函数cuowu**

    ```cpp
    Sales_data::Sales_data(std::istream &is){
      read(is, *this); // read 的作用是从 is 中读取一条交易信息然后存入 this 对象中
    }
    ```

    +   尽管该构造函数的初始值列表是空的, 但是由于执行了构造函数体, 所以对象的成员仍然能被初始化.

+   拷贝, 赋值和析构

    +   **除了定义类的对象如何初始化之外, 类还需要控制拷贝, 赋值和销毁对象时发生的行为**
    +   尽管编译器能替我们合成拷贝, 赋值和销毁的操作, 但是必须要清楚的一点是, 对于某些类来说合成的版本无法正常工作.

+   访问控制与封装

    +   使用访问说明符加强类的封装性.
    +   类没有封装指的是用户可以直达对象的内部并且控制它的具体实现细节.
    +   class 和 struct 的唯一区别是默认的访问权限不一样.
    +   出于统一编程风格的考虑, 当我们希望定义的类的所有成员是 public 的时候, 使用 struct; 反之, 使用 class.

+   友元

    +   既然 `Sales_data` 的数据成员是 private 的, 那 `read`, `print` 等函数就无法正常编译了, 尽管这几个函数是类的接口的一部分, 但是它们并不是类的成员.
    +   类可以允许其他类或者函数访问它的非公有成员, 方法是令其他类或者函数成为它的友元.
    +   如果类想把一个函数作为它的友元, 只需要增加一条以 `friend` 关键字开始的函数声明语句即可.
    +   **友元声明只能出现在类定义的内部, 但是在类内出现的具体位置不限, 友元不是类的成员也不受它所在区域访问控制级别的约束.**
    +   另外还要注意: **友元的声明仅仅指定了访问的权限, 而非一个通常意义上的函数声明. 如果我们希望类的用户能够调用某个友元函数, 那么我们就必须在友元声明之外再专门对函数进行一次声明.**

+   定义一个类型成员

    +   除了定义数据和函数成员之外, 类还可以自定义某种类型在类中的别名. 由类定义的类型名字和其他成员一样存在访问权限, 可以是 public 或在 private 中的一种. 如书上 243 页的例子:

        ```cpp
        class Screen{
         public:
          // 也开使用类型别名: using pos = std::string::size_type;
          typedef std::string::size_type pos;
         private:
          ...
        };
        ```

        注意用来定义类型的成员必须先定义后使用, 这一点与普通成员有所区别 (比如前面的 bookNo 成员, 放在 isbn 函数定义的下方也没关系.) 因此, 类型成员通常出现在类开始的地方.

+   令成员作为内联函数

    +   **定义**在类内部的成员函数是自动 inline 的.
    +   我们可以在类的内部把 inline 作为声明的一部分显式地声明成员函数
    +   同样地, 也可以在类的外部用 inline 关键字修饰函数的定义. 最好只在类外部定义的地方说明 inline, 这样可以使类更容易理解.

+   可以重载成员函数, 只要在参数的数量或类型上有所区别即可.

+   **可变数据成员**: (mutable data member)

    +   有时(但并不频繁)我们会希望能修改类的某个数据成员, **即使是在一个 const 成员函数内. 可以通过在变量的声明中加入 mutable 关键字做到这一点.**

    +   一个可变数据成员永远不会是 const, 即使它是 const 对象的成员. 因此, 一个 const 成员函数可以改变一个可变数据成员的值. 比如可以为 Screen 添加一个 `access_ctr` 的可变成员, 通过它我们可以追踪每个 Screen 的成员函数被调用了多少次:

        ```cpp
        class Screen{
         
        public:
          void some_member() const;  // 即使在一个 const 对象内也能被修改
        private:
          mutable size_t access_ctr;
        };

        void Screen::some_member() const {
          ++access_ctr; // 保存一个计数值, 用于记录成员函数被调用的次数
        };
        ```

        总之, 切记 const 对象也可以改变某些数据成员, 只要使用了 mutable 关键字.

+   从 const 函数返回 `*this`

    +   对于 const 成员函数, `this` 将指向一个 const 的指针而 `*this` 是 const 对象. 返回类型应该是 `const Screen&`. 也即是说: **如果一个 const 成员函数以引用的形式返回 *this, 那么它的返回类型将是常量引用.**

+   基于 const 的重载

    +   通过区分成员函数是否是 const 的, 我们可以对其进行重载. 因为非常量的函数对于常量对象是不可用的, 所以我们只能在一个常量对象上调用 const 成员函数. 另一方面, 虽然可以在非常量对象上调用常量版本或非常量版本, 但显然此时非常量版本是一个更好的匹配.

    +   书上 246 页 返回 `*this` 的成员函数这一节的例子可以详细地看一下, 写一个 display 函数让非常量对象和常量对象都能使用:

        ```cpp
        class Screen{
        public:
          // 根据对象是否为 const 重载 display 函数
          Screen &display(std::ostream &os) 
          			{ do_display(os); return *this; }
          const Screen &display(std::ostream &os) const
            		{ do_display(os); return *this; }
        private:
          // 该函数负责显示 Screen 的内容
          void do_display(std::ostream &os) const { os << contents; }
        };
        ```

        +   上面的代码可以通过对象是否是 const 的决定调用哪个版本.
        +   在 248 页书中给出了关于这个例子的建议: **对于公共代码使用私有功能函数**, 从上面代码可以看出, 这里单独的定义了私有的 `do_display` 函数, 原因如下:
            +   一个基本的愿望是避免在多处使用同样的代码
            +   我们预期随着类的规模发展, `display` 函数有可能变得更为复杂, 此时, 把相应的操作写在一处而非两处的作用就比较明显了.
            +   我们很可能在开发过程中给 `do_display` 函数添加某些调试信息, 而这些信息将在代码的最终产品中去掉. 显然, 只在 `do_display` 一处添加或删除这些信息要更容易一些.
            +   这个额外的函数调用不会增加任何开销, 因为我们在类内部定义了 `do_display`, 所以它隐式地被声明成内联函数. 这样的话, 调用 `do_display` 就不会带来任何额外的运行时开销.
        +   在实践中, 设计良好的 C++ 代码常常包含大量类似与 `do_display` 的小函数, 通过调用这些函数, 可以完成一组其他函数的 "实际" 工作.
        +   我的理解是, 此时私有功能相对于 public 来说, 不就是接口了吗.... 而 public 相对与我们用户来说就是接口... 所以道理是一样的...

+   类类型

    +   类的声明

        ```cpp
        class Screen; // 类的声明
        ```

        +   这种声明有时被称为前向声明, 它向程序中引入了名字 Screen, 此时我们已知 Screen 是一个类类型, 但是不清楚它到底包含哪些成员. (此时 Screen 在定义之前被称为不完全类型)

        +   对于一个类来说, 在我们创建它的对象之前, 该类必须被定义过, 而不能仅仅被声明, 否则, 编译器就无法了解这样的对象需要多少存储空间. **所以一个类的成员类型不能是该类自己** (我们必须完成定义然后编译器才能知道存储该数据成员需要多少存储空间). 然而, 一旦一个类的名字出现后, 它就被认为是声明过的了 (但尚未定义), **因此类允许包含指向它自身类型的引用或指针**.

            ```cpp
            class Link_screen{
              Screen window;
              Link_screen *next;
              Link_screen *prev;
            };
            ```

+   友元再探

    +   除了可以把普通的非成员函数声明成友元, 类还可以将其他的类, 其他的类的成员函数定义成友元, 此外友元函数也可以定义在类的内部, 这样的函数是隐式内联的.

    +   友元类

        ```cpp
        class Screen{
          // Window_mgr 的成员可以访问 Screen 类的私有成员
          friend class Window_mgr;
        };
        ```

    +   令成员函数作为友元

        ```cpp
        class Screen{
          // Window_mgr::clear 必须在 Screen 类之前被声明
          friend void Window_mgr::clear(ScreenIndex);
        };
        ```

        +   当一个成员函数声明为友元时, 我们必须明确指出该成员函数属于哪个类.

    +   要想令某个成员函数作为友元, 我们必须仔细组织程序的结构以满足声明和定义的彼此依赖关系. 在这个例子中, 我们必须按照如下方式设计程序:

        +   首先定义 `Window_mgr` 类, 其中声明 clear 函数, **但是不能定义它**: 在 clear 使用 Screen 的成员之前必须先声明 Screen.
        +   接下来定义 Screen, 包括对 clear 的友元声明;
        +   最后定义 clear, 此时它才可以使用 Screen 的成员.

    +   重载函数和友元: 重载函数是不同的函数, 声明友元时要对重载函数进行分别声明.

    +   **友元声明和作用域**

        +   **类和非成员函数的声明不是必须在它们的友元声明之前.**

            +   也就是上面的那个 `Window_mgr` 的例子, 顺序不一定非要那样, 即使先定义 Screen, 并在 Screen 中声明 clear 是友元也是可以的. 原因看下面.

        +   当一个名字第一次出现在一个友元声明中时, 我们隐式假定该名字在当前作用域中是可见的. 然而, 友元本身不一定真的声明在当前作用域中. **甚至就算在类的内部定义该函数, 我们也必类外部提供相应的声明, 从而使得函数可见.** 换句话说, 即使我们仅仅是用声明友元的类的成员调用该友元函数, 它也必须是被声明过的 (详见如下例子):

            ```cpp
            struct X{
              friend void f() { /* 友元函数可以定义在类的内部 */ }
              X() { f(); }  // 错误: f 还没有被声明
              void g();
              void h();
            };
            void X::g() { return f(); }  // 错误: f 还没有被声明
            void f();  // 声明那个定义在 X 中的函数
            void X::h() { return f(); } // 正确: 现在 f 的声明在作用域中了.
            ```

            +   **关于这段代码最重要的是理解友元声明的作用是影响访问权限, 它本身并非普通意义上的声明.** (比如上面代码中的 `void f();`)

    +   总结: 

        +   首先, 声明和定义是有很大区别的
        +   其次, 友元的声明和一般意义上的声明不同, 友元的声明只是会影响对类的访问权限, 并非一般意义上的声明.
        +   另外, 友元函数可以定义在将该函数声明友元的类中, 但是在该函数没有在类外部进行声明之前, 该类的成员函数是无法访问该友元函数的...

+   类的作用域

    +   在类的作用域之外, 类类型成员需要使用作用域运算符进行访问, 比如

        ```cpp
        // pos 在 Screen 中定义为: typedef std::string::size_type pos;
        Screen::pos ht = 24;
        ```

    +   作用域和定义在类外部的成员

        +   一个类就是一个作用域, 一旦遇到类名, 定义的剩余部分就在类的作用域之内了, 这里的剩余部分包括参数列表和函数体. 比如 `Window_mgr` 类定义的一种类型:

            ```cpp
            // ScreenIndex 在 Window_mgr 类中定义为:
            // using ScreenIndex = std::vector<Screen>::size_type;
            // screens 在 Window_mgr 中定义为:
            // std::vector<Screen> screens{Screen(24, 80, ' ')};

            void Window_mgr::clear(ScreenIndex i){
              Screen &s = screens[i];
              s.contents = string(s.height * s.width, ' ');
            }
            ```

            +   一方面, 编译器在处理参数列表之前就已经明确了我们当前正位于 `Window_mgr` 类的作用域中, 所以就不必再专门说明 ScreenIndex 是在 `Window_mgr` 中定义的类. (screens 也是同样的...)

            +   另一方面, **函数的返回类型通常是出现在函数名之前. 因此当成员函数定义在类的外部时, 返回类型中使用的名字都位于类的作用域之外. 这时, 返回类型必须指明它是哪个类的成员.**

                ```cpp
                // 例如我们向 Window_mgr 中添加一个 addScreen 函数, 
                // 它的返回类型是 ScreenIndex, 即新增加的 Screen 的编号
                class Window_mgr{
                public:
                  // 向窗口添加一个 Screen, 返回它的编号
                  ScreenIndex addScreen(const Screen&);
                }

                // 首先处理返回类型, 之后我们才进入 Window_mgr 作用域
                Window_mgr::ScreenIndex
                Window_mgr::addScreen(const Screen &s){
                  screens.push_back(s);
                  return screens.size() - 1;
                }
                ```

                +   因为返回类型出现在类名之前, 所以事实上它是位于 `Window_mgr` 类的作用域之外的. 在这种情况下, 要想使用 ScreenIndex 作为返回类型, 我们必须明确指出哪个类型定义了它.

+   名字查找与类的作用域

    +   类的定义分为两步:

        +   首先, 编译类的成员
        +   直到类全部可见后才编译函数体

    +   **编译器处理完类中的全部声明后才会处理成员函数的定义**

        +   因为成员函数体直到整个类可见后才会被处理, 所以它能使用类中定义的任何名字.

    +   用于类成员声明的名字查找

        +   这种两阶段的处理方式只适用于成员函数中使用的名字. 声明中使用的名字, 包括返回类型或者参数列表中使用的名字, 都必须在使用前确保可见. 如果某个成员的声明使用类中尚未出现的名字, 则编译器将会在定义该类的作用域中继续查找. 例如:

            ```cpp
            typedef double Money;
            string bal;
            class Account{
            public:
              Money balance() { return bal; }
            private:
              Money bal;
            };
            ```

            +   当编译器看到 balance 函数声明语句时, 它将会在 Account 类的范围内寻找对 Money 的声明. 编译器只考虑 Account 中在使用 Money 前出现的声明, 因为没有找到匹配的成员, 所以编译器会接着到 Account 的外层作用域中查找. 在这个例子中, 编译器会找到 Money 的 `typedef` 语句, 该类型被用作 balance 函数的返回类型以及数据成员 bal 的类型. 另一方面, balance 函数体在整个类可见后才被处理, 因此, 该函数的 return 语句返回名为 bal 的成员, 而非外层作用域的 string 对象.

    +   **类型名要特殊对待** 

        +   一般来说, 内层作用域可以重新定义外层作用域中的名字, 即使该名字已经在内层作用域中使用过. **然而在类中, 如果成员使用了外层作用域中的某个名字, 而该名字代表一种类型, 则类不能在之后重新定义改名字.**

            ```cpp
            typedef double Money;
            class Account{
            public:
              Money balance() { return bal; }  // 使用外层作用域的 Money
            private:
              typedef double Money;   // 错误: 不能重新定义 Money, 即使和外层作用域一致
              Money bal;
            };
            ```

    +   成员定义中普通块作用域的名字查找

        +   一般来说, 不建议使用其他成员的名字作为某个成员函数的参数, 比如:

            ```cpp
            // 下面的代码不是好代码
            int height;
            class Screen{
            public:
              typedef std::string::size_type pos;
              void dummy_fcn(pos height){
                cursor = width * height;  // 这个 height 参数到底是函数的参数还是类的成员?
              }                          // 答案是函数的参数!
            private:
              pos cursor = 0;
              pos height = 0, width = 0;
            };

            // 建议写成 void Screen::dummy_fcn(pos ht){ cursor = width * height; }
            ```

            +   当编译器处理 `dummy_fcn` 函数中的乘法表达式时, 它首先在函数作用域内查找表达式中用到的名字, 函数的参数位于函数作用域内, 因此 `dummy_fcn` 函数体用到的名字 `height` 指的是函数的参数.

            +   此例中的 height 隐藏了同名的成员, 如果想要绕开上面的查找规则, 应该将代码变为:

                ```cpp
                // 不建议的写法: 成员函数中的参数不应该隐藏同名的成员
                void Screen::dummy_fcn(pos height){
                  cursor = width * this->height;  // 成员 height
                  // 另外一种表示该成员的方式
                  cursor = width * Screen::height; // 成员 height
                }
                ```

            +   **尽管类的成员被隐藏了, 但我们仍然可以通过加上类的名字或显式地使用 this 指针来强制访问成员.**

    +   类作用域之后, 在外围的作用域中查找

        +   在上面的例子中, 如果我们要用到外层作用域中的名字, 那么**可以显式地通过作用域运算符来进行请求**:

            ```cpp
            // 不建议的写法: 不要隐藏外层作用域中可能被用到的名字
            void Screen::dummy_fcn(pos height){ // 此处的参数 height 将外层的 height 给隐藏了
              cursor = width * ::height;  // 哪个 height? 是那个全局的
            }
            ```

            +   尽管外层的对象被隐藏了, 但我们仍然可以用作用域运算符来访问它.
            +   **注意外层作用域的写法**

    +   在文件中名字的出现处对其进行解析

        +   当成员定义在类的外部时, 名字查找的第三步不仅要考虑类定义之前的全局作用域中的声明, 还要考虑在成员函数定义之前的全局作用域中的声明.

            ```cpp
            int height;
            class Screen{
            public:
              typedef std::string::size_type pos;
              void setHeight(pos);
              pos height = 0; // 隐藏了外层作用域中的 height
            };
            Screen::pos verify(Screen::pos);
            void Screen::setHeight(pos var) {
              // var: 参数
              // height: 类的成员
              // vierify: 全局函数
              height = verify(var);
            }
            ```

            +   要注意, 全局函数 verify 的声明在 Screen 类的定义之前是不可见的. 然而, 名字查找的第三步包括了成员函数之前的全局作用域. 在此例中, verify 的声明位于 setHeight 的定义之前, 因此可以被正常使用.

    ​

### 习题

+   习题 7.10: 在下面这条 if 语句中, 条件部分的作用是什么?

    ```cpp
    if (read(read(cin, data1), data2))
    ```

    +   参考答案: 该条件检验读入 data1 和 data2 的过程是否正常.
    +   确认 data1 和 data2 都被正常的读入了数据.

+   习题 7.35: 解释下面代码的含义, 说明其中的 Type 和 initVal 分别使用了哪个定义, 如果代码存在错误, 尝试修改它.

    ```cpp
    typedef string Type;
    Type initVal();  // 用到的是 string
    class Exercise {
    public:
      typedef double Type;
      Type setVal(Type); // 用到的是 double
      Type initVal(); // 用到的是 double
    private:
      int val;
    };

    // 返回类型用到的是 string
    Type Exercise::setVal(Type parm) { //函数参数用到的是 double
      val = parm + initVal(); // initVal 用到的是类中定义的 initVal
      return val;
    }

    // 修改, 在类内声明的同名函数形参类型是 double, 返回值类型也是 double,
    // 二者无法匹配. 修改的措施是在定义 setVal 函数时使用作用域运算符强制
    // 指定函数的返回值类型.
    Exercise::Type Exercise::setVal(Type parm) {
      val = parm + initVal();
      return val;
    }
    ```



## 2017 年 8 月 3 日

### 6.2.6 含有可变形参的函数

+   含有可变形参的函数 : 为了编写能处理不同数量实参的函数, C++11 提供了两种主要的方法:

    +   如果所有实参类型相同, 可以传递一个名为 `initializer_list` 的标准库类型;
    +   如果实参的类型不同, 可以编写一个特殊的函数, 也就是所谓的可变参数模板;
    +   C++ 还有一种特殊的形参类型(即省略符), 可以用它传递可变数量的实参. (一般只用于与 C 函数交互的接口程序)

+   `initializer_list` 形参

    +   它是一种标准库类型, 和 vector 一样, 也是模板类型, 用于表示某种特定类型的值的数组, 定义在 `#include <initializer_list>` 中, 但和 vector 不一样的是, `initializer_list` 中的元素都是常量值.

    +   `initializer_list` 提供的操作

        ```cpp
        initializer_list<T> lst;  // 默认初始化, T 类型元素的空列表
        initializer_list<T> lst{a, b, c...}; // lst 元素数量和初始值列表一样多, 列表中的元素都是 const
        lst2(lst);  // 拷贝或赋值一个一个initializer_list 对象, 不会拷贝列表中的元素, 
        lst2 = lst; // 拷贝后, 原始列表和副本共享元素
        lst.size(); // 列表中的元素数量
        lst.begin(); // 返回指向 lst 的首元素的指针
        lst.end();  // 返回指向 lst 中尾元素下一位置的指针
        ```

    +   如果想要向 `initializer_list` 形参中传递一个值的序列, 则必须把序列放在一对花括号内:

        ```cpp
        void error_msg(initializer_list<string> lst){
          for (auto beg = lst.begin(); beg != lst.end(); ++beg)
            cout << *beg << " ";
          cout << endl;
        }

        // expected 和 actual 是 string 对象
        // 注意序列放在一对花括号内
        if (expected != actual)
          error_msg({"functionX", expected, actual});
        else
          error_msg({"functionX", "okay"});
        ```

+   返回类型和 return 语句

    +   return 语句终止当前正在执行的函数, 并将控制权返回到调用函数的地方.

    +   **不要返回局部对象的引用或指针**; 因为函数完成后, 它所占用的存储空间也随之被释放掉. 因此, 函数终止意味着局部变量的引用将指向不再有效的内存区域. **要想确保函数值安全, 我们不妨提问: 引用所引的是在函数之前已经存在的哪个对象?**

    +   引用返回左值: 返回类型决定函数是否返回左值 (甚至可以把函数调用放在赋值语句的左侧...)

    +   **列表初始化返回值**

        +   C++11 新标准规定, 函数可以返回花括号包围的值的列表:

            ```cpp
            vector<string> process(){
              return {"A", "B"};
            }
            ```

    +   主函数 main 的返回值

        +   我们允许 main 函数没有 return 语句直接结束. 如果控制到达了 main 函数的结尾处而且没有 return 语句, 编译器将隐式地插入一条返回 0 的 return 语句.

        +   main 函数的返回值可以看作是状态指示器, `#include <cstdlib>` 头文件定义了两个预处理变量, 分别表示成功与失败: `EXIT_SUCCESS` 与 `EXIT_FAILURE`.

            因为它们是预处理变量, 所以不能在前面加上 `std::` (也就是不定义在 std 名字空间中)

+   **返回数组指针**

    +   首先要明白什么是数组指针. 比如举个例子:

        ```cpp
        int arr[3] = {1, 2, 3};

        int (*p)[3] = &arr;  // arr 也是一个对象, 指针, 指向数组首元素的地址, p 是指向数组的指针.
        cout << "arr : " << arr  << endl;
        cout << "&arr: " << &arr << endl;
        cout << "*arr: " << *arr << endl;
        cout << "&p  : " << &p   << endl;
        cout << "*p  : " << *p   << endl;
        cout << "**p : " << **p  << endl;

        // 输出结果:
        arr : 0x7fffa3fcbeb0
        &arr: 0x7fffa3fcbeb0
        *arr: 1
        &p  : 0x7fffa3fcbea8
        *p  : 0x7fffa3fcbeb0
        **p : 1
        ```

        +   从输出结果可以看出, arr 是一个指针, 指向数组首元素的地址, p 是一个新的指针, 指向 arr, 所以 `*p` 中的结果就是 arr 的地址, (只不过我不是很明白为何 `arr` 和 `&arr` 的结果一样)
        +   对于 arr 和 `&arr` 一样的问题, 查看回答: [Address of array](https://stackoverflow.com/questions/8412694/address-of-array) 回答是:

        >   When `t` is used on its own in the expression, an array-to-pointer conversion takes place, this produces a pointer to the first element of the array.
        >
        >   When `t` is used as the argument of the `&` operator, no such conversion takes place. The `&` then explicitly takes the address of `t` (the array). `&t` is a pointer to the array as a whole.
        >
        >   The first element of the array is at the same position in memory as the start of the whole array, and so these two pointers have the same value.

        也就是说, 当直接使用 arr 时, 会发生 array-to-pointer 隐式转换, 这时 arr 指向数组的第一个元素;

        但是当取地址符作用于 `arr` 时, 上述转换不会发生, 这时 `&arr` 指向的是整个数组, (整个数组的地址和数组第一个元素的地址是相同的!) 这时候需要联想一下多维数组... 因为我发现, 如果输出 `*(&arr)` 的结果, 仍然是 `0x7fffa3fcbeb0`, 也就是数组首元素的地址, 毕竟 `&arr` 指向的是整个数组而不是某个具体的元素, 把它想象成 `{{1, 2, 3}}`, 会不会好一些... 这样 `**(&arr)` 就可以检索到 1 这个元素.

    +   这样的话, 上面的 `int (*p)[3] = &arr;` 也就能理解了, 因为 `p` 是数组指针, 所以使用 `&arr` 初始化而不是 `arr`. 如果使用的是 `int (*p)[3] = arr;`, 那么会出现如下错误:

        ```cpp
        error: cannot convert ‘int*’ to ‘int (*)[3]’ in initialization
        ```

    +   从语法上来说, 想要定义一个返回数组的指针或引用的函数比较繁琐, 但可以使用类型别名简化任务:

        ```cpp
        typedef int arrT[10];  // arrT 是一个类型别名, 他表示类型是含有 10 个整数的数组
        using arrT = int[10]; // arrT 的等价声明
        arrT* func(int i); // func 返回一个指向含有 10 个整数的数组的指针
        ```

    +   首先注意如下声明:

        ```cpp
        int arr[10];  // arr 是一个含有 10 个整数的数组
        int *p1[10];  // p1 是一个含有 10 个指针的数组
        int (*p2)[10] = &arr; // p2 是一个指针, 指向含有 10 个整数的数组
        ```

        和这些声明一样, 如果我们想定义一个返回数组指针的函数, 则数组的维度必须跟在函数名字后面. 然而, 函数形参列表也跟在函数名字后面且形参列表应该先于数组的维度. 返回数组指针的函数形如:

        ```cpp
        Type (*function(parameter_list))[dimension]
        ```

        类似于其他数组的声明, `(*function(parameter_list))` 两端的括号必须存在, 就像我们定义 `p2` 时两端必须有括号一样. 如果没有这对括号, 函数返回的就是指针的数组.

        举个具体的例子, 下面这个 func 函数的声明没有使用类型别名:

        ```cpp
        int (*func(int i))[10];
        ```

        可以按照以下顺序来逐层理解该声明的含义:

        +   `func(int i)` 表示调用 `func` 函数需要一个 int 类型的实参
        +   `(*func(int i))` 意味着我们可以对函数调用的结果执行解引用操作
        +   `(*func(int))[10]` 表示解引用 func 的调用将得到一个大小是 10 的数组.
        +   `int (*func(int))[10]` 表示数组中的元素是 int 类型.

    +   使用尾置返回类型 (trailing return type)

        +   C++11 提出的声明方法, 尾置返回类型跟在形参列表后面并以一个 `->` 符号开头. 为了表示函数真正的返回类型跟在形参列表之后, 我们在本该出现返回类型的地方放置一个 `auto`.

            ```cpp
            // func 返回的是一个指针, 该指针指向含有 10 个整数的数组.
            auto func(int i) -> int (*)[10];
            ```

    +   使用 decltype

        +   如果我们知道函数返回的指针将指向哪个数组, 就可以使用 decltype.

            ```cpp
            int odd[] = {1, 3, 5, 7, 9};
            int even[] = {2, 4, 6, 8, 10};
            decltype(odd) *arrPtr(int i){
              return (i % 2) ? &odd : &even; // 返回一个指向数组的指针
            }
            ```

            +   由于 odd 的类型是数组类型, 所以 decltype(odd) 推断的类型就是个数组, 由于 decltype 并不负责将数组类型转换为对应的指针, 所以在 arrPtr 的返回类型处还需加上一个 `*` 符号.

    ​

+   函数重载: 在同一个作用域中, 几个函数名字相同但是形参列表不同, 称为重载函数 (overloaded)\

    +   main 函数不能重载

    +   对于重载函数来说, 它们应该在形参数量或者形参类型上有所不同.

    +   判断两个形参的类型是否相异: 有时候两个形参列表看起来不一样, 但实际上是相同的.

    +   重载和 const 形参

        +   顶层 const 不影响传入函数的对象, 一个拥有顶层 const 的形参无法和另外一个没有顶层 const 的形参区分开来:

            ```cpp
            Record lookup(Phone);
            Record lookup(const Phone);  // 重复声明

            Record lookup(Phone*);
            Record lookup(Phone* const); // 重复声明
            ```

        +   另一方面, 如果形参是某种类型的指针或引用, 则通过区分其指向的是常量对象还是非常量对象可以实现函数重载, 此时的 const 是底层的.

        ```cpp
        // 对于接受引用或指针的函数来说, 对象是常量还是非常量对应的形参是不同的
        // 下面定义了 4 个独立的重载函数
        Record lookup(Account&);   // 作用于 Account 的引用
        Record lookup(const Account&); // 新函数, 作用于 Account 的常量引用

        Record lookup(Account*); // 作用于 Account 的指针
        Record lookup(const Account*); // 新函数, 作用于指向常量的指针
        ```

        +   注意我们只能把 const 对象传递给 const 形参
        +   另外, 因为非常量可以转换为 const, 所以上面四个函数都能作用于非常量对象或者非常量对象的指针. 不过, 后面会介绍, 当我们传递一个非常量对象或者指向非常量对象的指针时, 编译器会优先选用非常量版本的函数.

    +   `const_cast` 和重载

    +   重载与作用域: 最好不要将函数声明置于局部作用域.

+   特殊用途语言特性

    +   与函数相关的 3 种语言特性:

        +   默认实参
        +   内联函数
        +   constexpr 函数

    +   默认实参

        +   **注意, 一旦某个形参被赋予了默认值, 它后面所有的形参都必须有默认值.**

        +   在设计含有默认实参的函数时, 其中一项任务是合理设置形参的顺序, 尽量让不怎么使用的默认值的形参出现在前面, 而让那些经常使用的默认值的形参出现在后面. 因为传递参数的时候必须按照顺序来.

            比如

            ```cpp
            typedef string::size_type sz;
            string screen(sz ht = 24, sz wid = 80, char background = ''); // 都提供了默认参数

            // 但是
            string window = screen(, , '?'); // 错误, 必须提供 ht 和 wid 的值.
            ```

    +   内联函数和 constexpr 函数

        +   调用函数一般比求等价的表达式的值要慢一些. 在大多数的机器上, 一次函数调用其实包含着一系列的工作: 调用前要先保存寄存器, 并在返回时恢复; 可能需要拷贝实参; 程序转向一个新的位置继续执行.
        +   内联函数可以避免函数调用的开销. 在函数返回类型前面加上关键字 inline, 这样就可以将函数声明为内联函数.

    +   constexpr 函数是指能用于常量表达式的函数.

        +   约定: 
            1.  函数的返回类型和所有形参的类型都得是字面值类型.
            2.  函数体中必须有且只有一条 return 语句.
                +   constexpr 函数体内也可以包含其他语句, 只要这些语句在运行时不执行任何操作就行, 比如空语句, 类型别名, 以及 using 声明.

        ```cpp
        constexpr int new_sz() { return 43; }
        constexpr int foo = new_sz(); // 正确, foo 是一个常量表达式
        ```

        +   我们允许 constexpr 函数的返回值并非一个常量:

            ```cpp
            // 如果 cnt 是常量表达式, 则 scale(cnt) 也是常量表达式
            constexpr size_t scale(size_t cnt) { return new_sz() * cnt; }
            ```

            +   注意当 scale 的实参是常量表达式时, 他的返回值也是常量表达式, 反之则不然.

    +   内联函数和 constexpr 函数通常是定义在头文件中.

        +   因为对于内联函数和 constexpr 函数来说, 它的多个定义必须完全一致.

        +   习题 6.46: 对于 189 页的 isShorter 函数, 能否定义为 constexpr 函数?

            ```cpp
            // 比较两个 string 对象的长度
            bool isShorter(const string &s1, const string &s2){
              return s1.size() < s2.size();
            }
            ```

            +   答案是不能, 因为 isShorter 函数不符合 constexpr 函数的要求, 它虽然只有一条 return 语句, 但是返回结果调用了标准库 string 类的 size() 函数和 `<` 比较符, 无法构成常量表达式, 因此不能改写成 constexpr 函数.

+   调试帮助

    +   assert 预处理宏

        ```cpp
        assert(expr);
        ```

        +   首先对 expr 进行求值, 如果表达式为假 (即 0), assert 输出信息并终止程序的执行. 为真的话什么也不做. 
        +   assert 宏定义在 `#include <cassert>` 头文件中. (预处理名字由预处理器而非编译器管理.)

    +   NDEBUG

        +   详情看书
        +   变量 `__func__` 输出当前调试的函数的名字, 编译器为每个函数都定义了 `__func__`, 它是 const char 的一个静态数组, 用于存放函数的名字.
        +   预处理器还定义了另外 4 个对于程序调试很有用的名字:
            +   `__FILE__` 存放文件名的字符串字面值
            +   `__LINE__` 存放当前行号的整型字面值
            +   `__TIME__` 存放文件编译时间的字符串字面值
            +   `__DATE__` 存放文件编译日期的字符串字面值

+   函数指针

    +   函数指针指向的是函数而非对象. 和其他指针一样, 函数指针指向某种特定的类型, **函数的类型由它的返回类型和形参类型共同决定, 与函数名无关**. 例如:

        ```cpp
        bool lengthCompare(const string&, const string&);
        ```

        该函数的类型是 `bool(const string&, const string&)`. 要想声明一个可以指向该函数的指针, 只需要用指针替换函数名即可:

        ```cpp
        // pf 指向一个函数, 该函数的参数是两个 const string 的引用, 返回值是 bool 类型
        bool (*pf)(const string&, const string&); // 未初始化
        ```

        +   从我们声明的名字开始观察, pf 前面有个 `*`, 因此 pf 是指针; 右侧是形参列表, 表示 pf 指向的是函数; 再观察左侧, 发现函数的返回类型是 bool 值, 因此 pf 就是一个指向函数的指针, 其中该函数的参数是两个 const string 的引用, 返回值是 bool 类型.

    +   使用函数指针

        +   把函数名作为一个值使用时, 该函数自动转换为指针.

            ```cpp
            pf = lengthCompare;   // pf 指向 函数
            pf = &lengthCompare;  // 等价的赋值语句: & 是可选的
            ```

        +   直接使用指向函数的指针调用该函数, 无需提前解引用指针:

            ```cpp
            bool b1 = pf("Hello", "Good");  // 调用 lengthCompare 函数
            bool b2 = (*pf)("Hello", "Good"); // 一个等价的调用
            bool b3 = lengthCompare("Hello", "Good"); // 另一个等价的调用
            ```

        +   指向不同函数类型的指针间不存在转换规则. 但是可以为函数指针赋一个 nullptr 或者 0;

    +   函数指针形参

        +   和数组类似, 虽然不能定义函数类型的形参, 但是形参可以是指向函数的指针, 此时形参看起来是函数类型, 但实际上却是当成了指针在使用:

            ```cpp
            // 第三个形参是函数类型, 它会自动转换成指向函数的指针
            void useBigger(const string& s1, const string& s2, bool pf(const string&, const string&));

            // 等价的声明
            void useBigger(const string& s1, const string& s2, bool (*pf)(const string&, const string&));
            ```

            +   可以把函数直接作为实参使用, 此时它会自动转换为指针.

        +   上面的方法比较冗繁, 使用类型别名或者 decltype 可以简化:

            ```cpp
            // Func 和 Func2 是函数类型
            typedef bool Func(const string&, const string&);
            typedef decltype(lengthCompare) Func2; // 等价的类型
            // FuncP 和 FuncP2 是指向函数的指针
            typedef bool (*FuncP)(const string&, const string&);
            typedef decltype(lengthCompare)* FuncP2;
            ```

            +   我认为这里可以将函数类型和数组类型进行联系比较, 发现两者的共同之处. 数组类型是由元素类型以及数组的维度组合成的. 而函数类型是由返回值类型以及形参列表组成的.

    +   返回指向函数的指针

        +   和数组类似, 虽然不能返回一个函数, 但是能返回指向函数类型的指针. 最简单的办法是使用类型别名:

            ```cpp
            using F = int(int*, int); // F 是函数类型, 不是指针
            using PF = int(*)(int*, int);  // PF 是指针类型
            ```

        +   当然我们也可以使用下面的形式直接声明 f1:

            ```cpp
            int (*f1(int))(int*, int);
            ```

            +   按照从内向外的顺序阅读这条声明语句, 我们看到 f1 有形参列表, 所以 f1 是个函数, f1 前面有 `*`, 所以 f1 返回一个指针; 由于指针的类型本身也包含形参列表, 因此指针指向函数, 该函数的返回类型为 int.

        +   还可以使用尾置返回类型:

            ```cpp
            auto f1(int) -> int (*)(int*, int);
            ```

        +   将 auto 和 decltype 用于函数指针类型

            +   如果知道返回的函数是哪一个, 就能使用 decltype简化书写函数指针返回类型的过程.
            +   **不过需要注意的是, 我们需牢记当我们将 decltype 作用于某个函数时, 它返回的是函数类型而非指针类型.** 因此我们需要显式的加上 `*` 表明我们需要返回指针, 而非函数本身.

### 习题

+   习题 6.27: 编写一个函数, 参数是 `initializer_list<T>` 类型的对象, 函数的功能是计算列表中所有元素的和;

    ```cpp
    #include <iostream>
    #include <initializer_list>

    using namespace std;

    template<typename T>
    T sum(initializer_list<T> lst){
        T sum = 0;
        for (const auto &elem : lst)
            sum += elem;
        return sum;
    }

    int main(int argc, char **argv){

        cout << sum({1, 2, 3, 5}) << endl;
    }
    ```

+   习题 6.28: 在 `error_msg` 函数第二个版本中包含的 ErrCode 类型的参数, 其中循环内的 elem 是什么类型?

    ```cpp
    void error_msg(ErrCode e, initializer_list<string> il){
      /*....*/
      for (const auto &elem : il)
        /*....*/
    }
    ```

    +   依我看, elem 是 `const string&` 类型

+   习题 6.29: 在范围 for 循环中使用 `initializer_list` 对象时, 应该将循环控制变量声明为引用类型吗? 为什么?

    +   **引用类型的优势主要是可以直接操作所引用的对象以及避免拷贝较为复杂的类类型对象和容器对象, 因为 `initializer_list` 对象的元素永远是常量值, 所以我们不可能通过设定引用类型来更改循环控制变量的内容. 只有当 `initializer_list` 对象的元素类型是类类型或容器类型 (比如 string) 时, 才有必要把范围 for 循环中的循环控制变量设置为引用类型.**

+   习题 6.31: 什么情况下返回的引用有效? 什么情况下返回常量的引用有效?

    +   如果引用所引的是函数开始之前就已经存在的对象, 则返回该引用是有效的; 如果引用所引的是函数的局部变量, 则随着函数结束局部变量也失效了, 此时返回的引用无效;
    +   当不希望返回的对象被修改时, 返回对常量的引用.

+   **习题 6.33**: 编写一个递归函数, 输出 vector 对象的内容

    +   函数的递归分为直接递归和间接递归, 编写递归函数的关键是确定递归规律和递归终止条件.

    +   我给出的答案:

        ```cpp
        #include <iostream>
        #include <vector>

        // 注意这里必须使用引用, 不用 const 也不会报错, 但是最好加上,
        // 毕竟 vector 只读. 如果不使用引用, 函数外的 vector 就会拷贝
        // 给 vec, 这样的话, if 中的 it 就永远不会等于内层中的 output 
        // 中的 vec.end()
        void output(const vector<int>& vec, vector<int>::iterator it){
            if (it != vec.end()){
                cout << *it << " ";
                output(vec, ++it);
            }
        }
        ```


        int main(int argc, char **argv){
            output(a, a.begin());
            cout << endl;
          	return 0;
        }
        ```

+   习题 6.34: 如果 factorial 函数的停止条件如下所示, 将会发生什么情况?

    ```cpp
    if (val != 0)
    ```

    +   先给出 factorial 函数

        ```cpp
        int factorial(int val){
          if (val > 1)
            return factorial(val - 1) * val;
          return 1;
        }
        ```

    +   如果条件改成上面那个, 则理论上用户可以传入 factorial 函数的参数可以是负数.

+   习题 6.35: 在调用 factorial 函数时, 为什么我们传入的值是 val - 1 而非 --val?

    +   **如果改成这样, 则会出现我们不期望看到的情况, 即变量的递减操作与读取变量值的操作共存于同一条表达式中, 这时可能产生未定义的值.**

+   习题 6.36: 编写一个函数声明, 使其返回数组的引用并且该数组包含 10 个 string 对象, 不要使用尾置返回类型, decltype 或者类型别名.

    ```cpp
    string (&func())[10];

    // 使用类型别名
    typedef string arrT[10];
    using arrT = string[10];
    arrT& func();

    // 使用 decltype 略

    // 使用尾置返回类型
    auto func() -> string (&)[10];
    ```

+   习题 6.38: 修改 arrPtr 函数, 使其返回数组的引用

    ```cpp
    // arrPtr 函数
    int odd[] = {1, 3, 5, 7, 9};
    int even[] = {2, 4, 6, 8, 10};
    decltype(odd) *arrPtr(int i){
      return (i % 2) ? &odd : &even; // 返回一个指向数组的指针
    }

    // 修改为返回数组的引用
    int odd[] = {1, 3, 5, 7, 9};
    int even[] = {2, 4, 6, 8, 10};
    decltype(odd) &arrPtr(int i){
      return (i % 2) ? odd : even; // 返回一个数组的引用
    }
    ```

+   习题 6.48: 说明下面这个循环的含义, 它对 assert 的使用合理吗?

    ```cpp
    string s;
    while (cin >> s && s != sought) { } // 空函数体
    assert(cin);
    ```

    +   首先循环的含义是从输入流中读取字符串, 直到找到匹配的字符串 sought
    +   不合理.

+   习题 6.54: 编写函数的声明, 令其接受两个 int 形参并且返回类型也是 int; 然后声明一个 vector 对象, 令其元素是指向该函数的指针.

    ```cpp
    int func(int&, int&);

    // 使用类型别名
    typedef int (*PF)(int&, int&);
    using PF = int (*)(int&, int&);
    vector<PF> vec;
    vec.push_back(func);

    // 使用 decltype
    vector<decltype(func)*> vec;  // 注意使用 decltype 时要加上 *, decltype 不会转换为指针

    // 直接声明
    vector<int (*)(int&, int&)> vec;
    ```

    ​

## 2017 年 8 月 2 日

### 函数

+   函数基础

    +   函数定义包括以下部分: 返回类型, 函数名字, 由 0 个或多个形参组成的列表以及函数体.
    +   我们通过调用运算符来执行函数, 调用运算符的形式是一对圆括号, 它作用于一个表达式, 该表达式是函数或者指向函数的指针; 圆括号之内是一个用逗号隔开的实参列表, 我们用实参来初始化函数的形参. 调用表达式的类型就是函数的返回类型.
    +   实参和形参: 实参是形参的初始值. 函数体必须放在一对花括号内.

+   局部对象

    +   在 C++ 中, 名字有作用域, 对象有生命周期, 理解这两个概念非常重要.

        +   名字的作用域是程序文本的一部分, 名字在其中可见
        +   对象的生命周期是程序执行过程中该对象存在的一段时间.

    +   函数体是一个语句块, 块构成一个新的作用域. 形参和函数体内部构成的变量统称为局部变量.

    +   自动对象: 我们把只存在于块执行期间的对象称为自动对象, 当块执行结束后, 块中创建的自动对象的值就变成了未定义的. 

        +   形参是一种自动对象, 函数开始时为形参申请存储空间, 因为形参定义在函数体作用域之内, 所以一旦函数终止, 形参也就销毁了. 我们用传递给函数的实参初始化形参对应的自动对象.
        +   对于局部变量对应的自动对象来说, 分为两种情况:
            +   变量本身含有初始值, 则用该初始值初始化
            +   变量本身不含初始值, 执行默认初始化

    +   局部静态对象: 局部静态对象在程序执行路径第一次经过对象定义语句时初始化, 并且直到程序终止时才被销毁, 在此期间使对象所在函数结束执行也不会对它有影响.

        +   在某些时候, 有必要令局部变量的生命周期贯穿函数调用以及之后的时间. 可以将局部变量定义成 static 类型从而获得这样的对象. 下面这个例子中的函数可以统计自己被调用了多少次.

            ```cpp
            size_t count_calls(){
              static size_t ctr = 0;
              return ++ctr;
            }

            int main(){
              for (size_t i = 0; i != 10; ++i)
                cout << count_calls() << endl;
              return 0;
            }
            ```

            +   如果局部静态变量没有显式的初始值, 它将执行值初始化, 内置类型的局部静态变量初始化为 0;

+   参数传递

    +   指针形参: 当执行指针拷贝操作时, **拷贝的是指针的值**. 拷贝之后, 两个指针是不同的指针, 因为指针使我们可以间接的访问它所指的对象, 所以通过指针可以修改它所指对象的值.

        ```cpp
        void reset(int *ip){
          *ip = 0;  // 改变指针 ip 所指对象的值
          ip = 0;   // 只改变了 ip 的局部拷贝, 实参未被改变
        }
        ```

        +   **注意这个函数中, 虽然令 `ip=0;`, 但是这实际上只是改变了函数中的局部变量, 实参 ip 没有被改变.**

+   使用引用避免拷贝: 拷贝大的类类型对象或者容器对象比较低效, 甚至有的类类型 (包括 IO 类型在内) 根本不支持拷贝操作. 当某种类型不支持拷贝操作时, 函数只能通过引用形参访问该类型的对象.

    +   如果函数无须改变引用形参的值, 最好将其声明为常量引用.

+   const 形参和实参

    +   如下程序

        ```cpp
        const int ci = 42;
        int i = ci;  //正确: 当拷贝 ci 时, 忽略了它的顶层 const
        ```

        因此, 和初始化过程一样, 当用实参初始化形参时会忽略掉顶层 const. 因此函数 `void fcn(const int i){....}` 既可以传入 const int 也可以传入 int. 但是

        ```cpp
        void fcn(const int i) {/* fcn 能够读取 i, 但是不能向 i 写值 */}
        void fcn(int i) {/*...*/} // 错误: 重复定义了 fcn(int)
        ```

        +   注意因为顶层 const 被忽略掉了, 所以在上面的代码中传入两个 fcn 函数的参数可以完全一样. 因此第二个 fcn 是错误的. (注意不要跟带引用的形参弄混了...)

    +   尽量使用常量引用

        +   **把函数不会改变的形参定义成普通的引用是一种比较常见的错误, 这么做带给函数的调用者一种误导, 即函数可以修改它的实参的值. 此外, 使用引用而非常量引用也会极大的限制函数所能接受的实参类型, 比如不能将 const 对象, 字面值或者需要类型转换的对象传递给普通的引用形参.**

+   数组形参

    +   数组的两个特性对我们定义和使用作用在数组上的函数有影响:

        +   不允许拷贝数组: 所以我们无法以值传递的方式使用数组参数
        +   使用数组时通常会将其转换成指针: 当我们为函数传递一个数组时, 实际上传递的是指向数组首元素的指针.

    +   我们可以把形参写成类似数组的形式:

        ```cpp
        // 尽管形式不同, 但这三个 print 函数是等价的
        // 每个函数都有一个 const int* 类型的形参
        void print(const int*);
        void print(const int[]);
        void print(const int[10]); // 这里的维度表示我们期望数组含有多少元素, 实际不一定
        ```

        +   **注意虽然表现形式不同, 但上面三个函数是等价的, 每个函数的唯一形参都是 `const int*`**. 如果我们传给 `print` 函数一个数组, 则实参自动转换为指向数组首元素的指针, 数组的大小对函数的调用没有影响.

    +   因为数组是以指针的形式传递给函数的, 所以一开始函数并不知道数组的确切尺寸, 调用者应该为此提供一些额外的信息. 管理指针形参有 3 中常用的技术:

        +   使用标记指定数组的长度: 比如 C 风格字符串

        +   使用标准库规范: 传递指向数组首元素和尾后元素的指针

            ```cpp
            void print(const int *beg, const int *end){
              while (beg != end)
                cout << *beg++ << endl;
            }
            ```

        +   显示传递一个表示数组大小的形参:

            ```cpp
            // const int ia[] 等价于 const int*
            // size 表示数组的大小, 将它显示地传给函数用于控制对 ia 元素的访问
            void print(const int ia[], size_t size){
              for (size_t i = 0; i != size; ++i)
                cout << ia[i] << endl;
            } 

            // 例子
            int j[] = {1, 2};
            print(j, end(j) - begin(j));
            ```

    +   数组引用形参

        ```cpp
        // 正确: 形参是数组的引用, 维度是类型的一部分
        void print(int (&arr)[10]){
          for (auto elem : arr)
            cout << elem << endl;
        }
        ```

        +   **注意上面这种写法只能处理大小为 10 的数组, 否则会报错**, 到 578 页会介绍如果写传递任意大小的数组的函数.

+   `main`: 处理命令行选项

    +   命令行选项通过两个可选的形参传递给 main 函数:

        ```cpp
        int main(int argc, char *argv[]){ ... }
        ```

        +   第二个形参 `argv` 是一个数组, 它的元素是指向 C 风格字符串的指针
        +   第一个形参 `argc` 表示数组中字符串的数量

    +   因为第二个形参是数组, 所以 main 函数还可以定义成这样:

        ```cpp
        int main(int argc, char **argv){ ... }
        ```

        其中 argv 指向 `char *`.

    +   当实参传递给 main 函数后, argv 的第一个元素指向程序的名字或者一个空字符串, 接下来的元素依次传递命令行提供的实参. 最后一个指针之后的元素值保证为 0. 比如

        ```cpp
        // 若 ./main -d -o ofile data, 则
        argv[0] = "./main"; // 或者 argv[0] 也可以指向一个空字符串
        argv[1] = "-d";
        argv[2] = "-o";
        argv[3] = "ofile";
        argv[4] = "data";
        argv[5] = 0;
        ```

        此时 argc 应该为 5; 注意不是 6. 还需注意用户的参数从 `argv[1]` 开始而不是从 0 开始.



### 第五章 语句

+   C++ 中大多数语句都以分号结束, 一个表达式, 比如 `ival + 5`, 末尾加上分号就变成了表达式语句. 表达式语句的作用是执行表达式并丢弃掉求值的结果.

+   最简单的语句是空语句:

    ```cpp
    ; // 空语句
    ```

+   复合语句页称为块 (block)

+   语句的作用域

    +   可以在 `if`, `switch`, `while` 和 `for` 语句的控制结构内定义变量, 定义在控制结构当中的变量只在相应语句内部可见, 一旦语句结束, 变量也就超出其作用范围了:

        ```cpp
        while (int i = get_num()) // 每次迭代时创建并初始化 i
          cout << i << endl;
        i = 0; // 错误: 在循环外部无法访问 i
        ```

+   条件语句

+   try 语句块和异常处理

    +   异常处理机制为程序中的异常检测和异常处理这两部分的协作提供支持, 在 C++ 中, 异常处理包括:
        +   throw 表达式: 异常检测部分使用 throw 表达式来表示它遇到了无法处理的问题
        +   try 语句块: 异常处理部分使用 try 语句块处理异常. 
        +   异常类: 用于在 throw 表达式和相关的 catch 子句之间传递异常的具体信息.
    +   每个标准库异常类都定义了名为 `what` 的成员函数. 这些函数没有参数.

### 第四章 表达式

+   表达式由一个或多个运算对象 (operand) 组成, 对表达式求值将得到一个结果 (result). 字面值和变量是最简单的表达式, 其结果就是字面值和变量的值. 把一个运算符和一个或多个运算对象组合起来可以生成较复杂的表达式.

+   基本概念: 

    +   一元, 二元, 三元运算符, 函数调用也是一种特殊的运算符, 它对运算对象的数量没有限制.
    +   对含有多个运算符的复杂表达式来说, 想要理解它的含义首先要理解运算符的优先级, 结合律, 以及运算对象的求值顺序. 比如: `5 + 10 * 20/2;`, 对于 `*`, 它是一个二元运算符, 它的运算对象有 4 中可能: 10 和 20, 10 和 20/2, 15 和 20, 15 和 20/2.
    +   运算对象的类型转换..
    +   重载运算符.
    +   左值和右值: 当一个对象被用作右值的时候, 用到的是对象的值 (内容); 当对象被用作左值的时候, 用到的是对象的身份 (在内存中的位置).
        +   赋值运算符需要一个非常量的左值作为其左侧运算对象, 得到的结果也仍然是一个左值.
        +   取地址符作用于一个左值运算对象, 返回一个指向该对象的指针, 这个指针是一个右值.
        +   内置解引用运算符, 下标运算符, 迭代器解引用运算符, string 和 vector 的下标运算符的求值结果都是左值.
        +   内置类型和迭代器的 `++` 和 `--` 作用于左值运算对象, 其前置版本所得的结果也是左值.
    +   **使用关键字 decltype 的时候, 左值和右值也有所不同, 如果表达式的求值结果是左值, decltype 作用于该表达式 (不是变量) 得到一个引用类型.** 比如, 若 p 的类型是 `int*`, 因为解引用运算符生成左值, 所以 `decltype(*p)` 的结果是 `int&`, 另一方面, 因为取地址符生成右值, 所以 `decltype(&p)` 的结果是 `int**`, 也就是说结果是一个指向整型指针的指针.

+   优先级和结合律: **它们决定表达式中每个运算符对应的运算对象来自表达式的哪一部分**.

    +   高优先级运算符的运算对象要比低优先级运算符的运算对象更为紧密地组合在一起, 如果优先级相同, 则其组合规则由结合律确定.
    +   左结合律意味着当优先级相同时按照从左向右的顺序进行组合.

+   求值顺序: **优先级规定了运算对象的组合方式, 但是没有说明运算对象按照什么顺序求值.** 大多数情况下不会明确指定求值的顺序. 但比如:

    ```cpp
    int i = 0;
    cout << i << " " << ++i << endl; // 未定义
    ```

    +   程序是未定义的, 因为可能得到 `1, 1`, 也可能得到 `0, 1`.
    +   有 4 中运算符明确规定了运算对象的求值顺序: `&&`, `||`, `?:`(条件运算符), `,` (逗号运算符)

+   根据取余运算的定义, 如果 m 和 n 是整数且 n 非 0, 则表达式 `(m/n)*n + m%n` 的结果与 m 相等.

    ```cpp
    21 % 6;   // 结果为 3
    -21 % -8; // 结果为 -5
    21 % -5;  // 结果为 1
    -21 / -8; // 结果为 2
    21 / -5;  // 结果为 -4
    ```

    +   如果 `m%n` 不等于 0, 则它的符号和 m 相同
    +   `(-m)/n` 和 `m/(-n)` 都等于 `-(m/n)`, 
    +   `m%(-n)` 等于 `m%n`, `(-m)%n` 等于 `-(m%n)`

+   如果想要测试一个算术对象或指针对象的真值, 最直接的方法就是将其作为 if 语句的条件:

    ```cpp
    if (val) {...} // 如果 val 是任意的非 0 值, 条件为真
    if (!val) {...} // 如果 val 是 0, 条件为真
    ```

+   赋值运算满足右结合律

    ```cpp
    int ival, jval;
    ival = jval = 0;
    ```

    +   因为赋值运算符满足右结合律, 所以靠右的赋值运算 `jval = 0` 作为靠左的赋值运算符的右侧运算对象. 又因为赋值运算返回的是其左侧运算对象, 所以靠右的赋值运算的结果 (即 jval) 被赋给了 ival.

+   赋值运算优先级较低

    +   赋值语句经常出现在条件当中, 因为赋值运算的优先级相对较低, 所以通常需要给赋值部分加上括号使其符合我们的愿意. 

    +   下面这个循环说明了把赋值语句放在条件当中有什么用处, 他的目的是反复调用一个函数直到返回期望的值为止:

        ```cpp
        // 这是一种形式烦琐, 容易出错的写法
        int i = get_value();
        while (i != 42){
          // 其他处理 ....
          i = get_value();    // 获得新的值
        }
        ```

        可以将上述代码更简洁的表达出来:

        ```cpp
        int i;
        // 更好的写法, 条件部分表达得更加清晰
        while ((i = get_value()) != 42){
          // 其他处理 ......
        }
        ```

        +   注意要加上括号
        +   注意赋值运算符返回左侧运算对象

+   递增和递减运算符

    +   建议: 除非必须, 否则不使用递增递减运算符的后置版本!

    +   如果我们想在一条复合表达式中既将变量加 1 或减 1 又能使用它原来的值, 这时就可以使用递增和递减运算符的后置版本. (下面这个例子输出 vector 对象中的内容直到遇到第一个负值为止)

        ```cpp
        auto pbeg = v.begin();
        while (pbeg != v.end() && *pbeg >= 0)
          cout << *pbeg++ << endl;
        ```

    +   建议: 简洁可以成为一种美德, 比如 `*iter++`, 要习惯这种写法.

    +   **注意**: 运算对象可以按任意顺序求值, 这在一般情况下不会有什么影响, 然而, 如果一条子表达式改变了某个运算对象的值, 另一条子表达式又要使用该值的话, 运算对象的求值顺序就很关键了. 因为递增递减运算符会改变运算对象的值, 所以要提防在复合表达式中错用这两个运算符.

        ```cpp
        // 该循环的行为是未定义的
        while (beg != s.end() && !isspace(*beg))
          *beg = toupper(*beg++);   // 错误, 该赋值语句未定义
        ```

        上面的例子将产生未定义的行为. 问题在于: 赋值运算符左右两端的运算对象都用到了 beg, 并且右侧的运算对象还改变了 beg 的值, 所以该赋值语句是未定义的. 编译器可能按照下面任意一种思路处理该表达式:

        ```cpp
        *beg = toupper(*beg);      // 如果先求左侧的值
        *(beg + 1) = toupper(*beg);  // 如果先求右侧的值
        ```

        也可能采取别的什么方式处理它.

+   成员访问运算符

    +   `ptr->mem` 等价于 `(*ptr).mem`
    +   解引用的优先级低于点运算符.

+   条件运算符

    +   `condition ? expr1 : expre2`
    +   如果 condition 的值为真, 对 `expr1` 求值并返回该值, 否则对 `expr2` 求值并返回该值.
    +   当条件运算符的两个表达式都是左值或者能够转换为同一种左值类型时, 运算的结果是左值; 否则运算的结果为右值.
    +   嵌套条件运算符

+   位运算符

    ```cpp
    ~   // 位求反
    <<  // 左移
    >>  // 右移
    &   // 位与
    ^   // 位异或
    |   // 位或
    ```

+   `sizeof` 运算符

    +   该运算符返回一条表达式或一个类型的名字所占的字节数. `sizeof` 运算符满足有结合律, 其所得的值是一个 `size_t` 类型的常量表达式.

        ```cpp
        sizeof (type)
        sizeof expr  // sizeof 返回的是表达式结果类型的大小
        ```

    +   sizeof 并不实际计算其运算对象的值. 因此, 在 sizeof 的运算对象中解引用一个无效指针仍然是一种安全行为, 因为指针实际上并没有真正被使用, sizeof 不需要真的解引用指针也能知道它所指的对象的类型.

    +   sizeof 的结果依赖其作用的类型:

        +   **对数组执行 sizeof 运算得到的是整个数组所占空间的大小, 等价于对数组中所有的元素各执行一次 sizeof 运算并将所得的结果求和. 注意 sizeof 运算不会把数组转换成指针来处理.**
        +   对 string 或 vector 对象执行 sizeof 运算只返回该类型固定部分的大小, 不会计算对象中的元素占用了多少空间.

    +   因为执行 sizeof 运算能得到整个数组的大小, 所以可以用数组的大小除以单个元素的大小得到数组的长度.

        ```cpp
        constexpr size_t = sizeof(ia)/sizeof(*ia);
        ```

+   逗号运算符: 含有两个运算对象, 按照从左向右的顺序依次求值, 规定了从左向右的求值顺序; 逗号运算符真正的结果是右侧表达式的值.

+   **类型转化**

    +   隐式转换

        +   算术转换: 要想理解算术转化, 办法之一就是研究大量的例子.
        +   数组转换成指针
        +   指针的转换
            +   常量整数值 0 或者字面值 nullptr 能转换成任意指针类型
            +   指向任意非常量的指针能转换成 `void*`
            +   指向任意对象的指针能转换成 `const void*`
            +   转换成 bool 类型
            +   转换成常量
            +   类类型定义的转换

    +   显示转换

        +   强制类型转换 (cast)

        +   一个命名的强制类型转换具有如下形式:

            ```cpp
            cast_name<type>(expression);
            ```

            +   `type` 是转换的目标类型, 而 `expression` 是要转换的值.
            +   如果 `type` 是引用类型, 则结果是左值.
            +   其中 `cast_name` 是 `static_cast`, `dynamic_cast`, `const_cast` 和 `reinterpret_cast` 中的一种

    +   `static_cast`

        +   任何具有明确定义的类型转换, 只要不包含底层 const, 都可以使用 `static_cast`. 比如:

            ```cpp
            double slope = static_cast<double>(j)/i;  // 强制类型转换, double -> int
            ```

        +   `static_cast` 对于编译器无法自动执行的类型转化也非常有用, 例如我们可以使用 `static_cast` 找回存在于 `void*` 中的指针中的值:

            ```cpp
            void* p = &d; // 任何非常量对象的地址都能存入 void*
            double *dp = static_cast<double*>(p); // 将 void* 转换回初始的指针类型
            ```

    +   `const_cast`

        +   我靠....
        +   看不懂

    +   `reinterpret_cast`

        +   ..... pass

### 习题

+   习题 4.9: 解释下面的 if 语句中条件部分的判断过程

    ```cpp
    const char *cp = "Hello World";
    if (cp && *cp)
    ```

    +   cp 判断指针是否为有效, 由于 cp 不是空指针, 所以根据 `&&` 运算符, 还需判断 `*cp` 的值; `*cp` 判断 cp 所指向的对象是否为空, 由于 `*cp` 的内容为 `'H'`, 所以条件判断为真.

+   习题 4.11: 书写一条表达式用于测试 4 个值 a, b, c, d 的关系, 确保 a 大于 b, b 大于 c, c 大于 d.

    ```cpp
    a > b && b > c && c > d
    ```

+   习题 4.12: 假设 i, j 和 k 是 3 个整数, 说明表达式 `i != j < k` 的含义;

    +   由于 `<` 的优先级大于 `!=`, 所以 `j < k` 先先执行, 因为表达式的含义是判断 `i` 是否不等于 `j < k` 这个 bool 值.

+   习题 4.13: 下述语句中, i 和 d 分别是多少

    ```cpp
    int i; double d;
    (a) d = i = 3.5;   (b) i = d = 3.5;
    ```

    +   (a) d = 3; i = 3; 
    +   (b) d = 3.5; i = 3;

+   习题 4.14: 执行下述 if 语句之后将发生什么情况?

    ```cpp
    if (42 = i) // ...
    if (i = 42) // ...
    ```

    +   报错, 42 是右值, 不能被赋值
    +   if 中的条件判断结果为 true

+   习题 4.15: 下面的赋值是非法的, 为什么, 应该如何修改?

    ```cpp
    double dval; int ival; int *pi;
    dval = ival = pi = 0;
    ```

    +   因为 `pi` 是指针类型, 无法与 `int` 类型进行转化.

+   习题 6.1: 实参和形参的区别是什么?

    +   形参出现在函数定义的地方, 形参列表可以包含 0 个, 1 个或多个形参, 多个形参之间用逗号分割, 形参规定了一个函数所接收数据的类型和数量;
    +   实参出现在函数调用的地方, 实参的数量与形参一样多. 实参的主要作用是初始化形参, 并且这种初始化过程是一一对应的. 实参的类型必须与对应的形参类型匹配.

+   习题 6.13: 假设 T 是某种类型的名字, 说明一下两个函数声明的区别: 一个是 `void f(T)`, 另一个是 `void f(T&)`. 

    +   前者采用的是传值方式, 也就是说, 实参的值被拷贝至形参, 形参和实参是两个相互独立的变量, 在函数 f 内部对形参所做的任何改动都不会影响实参的值.
    +   后者的形参采用的是传引用的方式, 此时形参是对应的实参的别名, 形参绑定到初始化它的对象. 如果我们改变了形参的值, 也就是改变了对应实参的值.



## 2017 年 8 月 1 日

### 3.2 节 字符串

+   访问 string 对象的两种方式: 

    +   使用下标
        +   下标运算符 `[]` 接收的是 `string::size_type` 类型的值
    +   使用迭代器

+   标准库类型 vector:

    +   `#include <vector>`

    +   注意 vector 是一个类模板, 类模板本身不是类或函数, 编译器根据模板创建类或函数的过程称为实例化 (instantiation)

    +   初始化 vector: 

        +   列表初始化 : `vector<int> vec = {1, 2, 3};`

        +   创建指定数量的元素 : `vector<int> v1(10, -1);`

        +   值初始化 : 通常情况下, 可以只提供 vector 对象容纳的元素数量而略去初始值, 此时库会创建一个值初始化的 (value-initialized) 元素初值, 并把它赋给容器中的所有元素.

            ```cpp
            vector<int> ivec(10);  // 10 个元素, 每个都初始化为 0
            ```

    +   **这个问题注意:** 在某些情况下, 初始化的真实含义依赖于传递初始值时用的花括号还是圆括号. 如果用的是圆括号, 可以说提供的值是用来构造 (construct) vector 对象的; 如果用的是花括号, 可以表述成我们想列表初始化该 vector 对象. 也就是说, **初始化过程会尽可能把花括号内的值当成是元素初始值的列表来处理, 只有无法执行列表初始化时才会考虑其他初始化方式.**

        ```cpp
        vector<string> v5{"hi"};  // 列表初始化
        vector<string> v6("hi");   // 错误, 不能使用字符串字面值构建 vector 对象
        vector<string> v7{10};    // v7 有 10 个默认初始化的元素
        vector<string> v8{10, "hi"};  // v8 有 10 个值为 "hi" 的元素
        ```

        由于不能用 int 来初始化 string 对象, 所以 v7 和 v8 提供的值不能作为元素的初始值, 确认无法执行列表初始化后, 编译器会尝试用默认值初始化 vector 对象.

    +   vector 支持的操作:

        ```cpp
        v.empty()
        v.size()        // 返回的类型是由 vector 定义的 size_type 类型, 比如 vector<int>::size_type
        v.push_back(t)
        v[n]
        v1 = v2
        v1 = {1, 2, 3}
        v1 == v2        // v1 等于 v2 当且仅当它们的元素数量相同且对应位置的元素值都相同
        v1 != v2
        <, <=, >, >=   // 以字典顺序进行比较
        ```

    +   思考: 统计各个分数段各有多少个成绩, 比如 0 ~ 100 分中, 0 ~ 9, 10 ~ 19, ..., 90 ~ 99, 100 等 11 个分数段中各有多少个成绩.

        ```cpp
        vector<unsigned> scores(11, 0); // 11 个分数段, 全部初始化为 0
        unsigned grade;
        while (cin >> grade){    // 读取成绩
          if (grade <= 100)      // 只处理有效成绩
            ++scores[grade/10];  // 将对应分数段的计数值加 1
        }
        ```

    +   **注意: 不能在范围 for 循环中向 vector 对象添加元素.**

+   迭代器介绍

    +   有迭代器的类型同时拥有返回迭代器的成员, 其中 `begin()` 返回指向第一个元素的迭代器, `end()` 成员负责返回指向容器尾元素的下一位置的迭代器. 特殊情况下, 如果容器元素为空, 则 begin 和 end 返回的是同一个迭代器. `auto b = v.begin(), e = v.end();` 一般来说, 我们不清楚 (不在意) 迭代器准确的类型到底是什么.

    +   迭代器运算符

        ```cpp
        *iter           // 返回 iter 所指的元素的引用
        iter->mem       // 等价于 (*iter).mem
        ++iter          // 令 iter 指向容器中的下一个元素
        --iter
        iter1 == iter2  // 如果两个迭代器指示的是同一个元素或者   
        iter1 != iter2  // 它们是同一个容器的尾后迭代器, 则相等, 否则不等
        ```

        例子:

        ```cpp
        for (auto it = s.begin(); it != s.end() && !isspace(*it); ++it)
          *it = toupper(*it);
        ```

        +   对了, 由于所有的标准库容器的迭代器都定义了 == 和 !=, 但是它们中的大多数都没有定义 < 运算符, 因此只要我们养成使用迭代器和 != 的习惯, 就不用太在意用的到底是哪种容器类型.

    +   迭代器的类型: iterator 以及 const_iterator

        ```cpp
        vector<int>::iterator it; // it 能读写 vector<int> 的元素
        vector<int>::const_iterator it2; // it2 只能读元素, 不能写元素.
        ```

        +   如果对象是常量, begin 和 end 返回 const_iterator; 否则, 返回 iterator.

        +   但如果对象只需读操作而无需写操作的话, 最好使用常量类型 (比如 const_iterator), 为了便于专门得到 const_iterator 类型的返回值, c++11 标准引入了两个新函数, 分别是 cbegin 和 cend; 不论对象是否为常量, 都返回 const_iterator.

            ```cpp
            auto it = v.cbegin();  // it 的类型是 vector<int>::const_iterator;
            ```

    +   结合解引用和成员访问操作: 

        ```cpp
        vector<string> ivec = {"test"};
        auto it = ivec.begin();
        cout << it->size() << endl;

        (*it).size() // 结果为 4, 通过这两种方式访问成员
        it->size()
        ```

        注意 `(*it).size()` 中的圆括号必不可少, `*it.size()` 是错误的, 它试图访问 it 的名为 size() 的成员, 但 it 是迭代器没有 size() 成员. 为了简化上述表达式, C++ 定义了箭头运算符, `it->size()` 就等价于 `(*it).size()`.

    +   任何一种可能改变  vector 对象容量的操作, 比如 `push_back`, 都会使该 vector 对象的迭代器失效.

+   迭代器运算

    ```cpp
    iter + n
    iter - n
    +=, -=
    <, <=, >, >=
    iter1 - iter2  // 两个迭代器相减的结果是它们之间的距离
    ```

    +   假设 it 和 mid 是同一个 vector 对象的两个迭代器, 可以使用下面的代码比较它们所指的位置孰前孰后:

        ```cpp
        if (it < mid)
          // 处理 vi 前半部分的元素
        ```

        所谓的距离指的是右侧的迭代器向前移动多少位置就能够追上左侧的迭代器, 其类型名为 `difference_type` 的带符号整型数.

    +   下面的程序使用迭代器完成了二分搜索:

        ```cpp
        // text 必须是有序的
        // beg 和 end 表示我么搜索的范围
        auto beg = text.begin(), end = text.end();
        auto mid = text.begin() + (end - begin) / 2; // 初始状态下的中间点
        // 当还有元素尚未检查并且我们还没有找到 sought 的时候执行循环
        while (mid != end && *mid != sought){
          if (sought < *mid)
            end = mid;
          else
            beg = mid + 1;
        	
          mid = beg + (end - beg) / 2;   // 新的中间点
        }
        ```

        +   循环部分先检查搜索范围是否为空, 如果 mid 和 end 的当前值相等, 说明已经找遍了所有元素, 此时条件不满足, 循环终止; 当搜索范围不为空时, 可知 mid 指向了某个元素, 检查该元素是否是我们所要搜索的, 如果是, 也终止循环.
        +   当进入到循环体内部后, 程序通过某种规则一定 beg 或者 end 来缩小搜索范围;
        +   当循环过程终止时, mid 或者等于 end, 或者指向要找的元素, 如果 mid 等于 end, 说明 text 中没有我们要找的元素.

+   数组: 数组和 vector 不同的是, 数组的大小确定不变, 不能随意向数组中增加元素. 因为数组的大小固定, 因此对某些特殊的应用来说程序的运行时性能较好, 但是相应的也损失了一些灵活性.

    +   如果不确定元素的确切个数, 请使用 vector.

    +   数组的声明形式形如 `a[d]`, 其中 a 是数组的名字, d 是数组的维度. 维度说明了数组中元素的个数, 由于数组中元素的个数也属于数组类型的一部分, 编译的时候维度应该是已知的, 也就是说, 维度必须是一个常量表达式.

    +   定义数组的时候必须指定数组的类型, 不允许使用 auto 关键字由初始值列表推断类型. **另外, 和 vector 一样, 数组的元素应为对象, 因此不存在引用的数组**.

    +   字符数组的特殊性: 字符数组有一种额外的初始化方式, 我们可以使用字符串字面值对此类数组进行初始化, 当使用这种方式时, 注意字符串字面值的结尾处还有一个空白字符, 这个空白字符也会像字符串的其他字符一样被拷贝到字符数组中去:

        ```cpp
        const char a4[6] = "abcdef";   // 错误: 没有空间可存放空字符;
        ```

    +   不允许拷贝或赋值: 不能将数组的内容拷贝给其他数组作为初始值, 也不能用数组为其他数组赋值:

        ```cpp
        int a[] = {1, 2, 3};
        int a2[] = a;  // 错误, 不允许使用一个数组初始化另一个数组
        a2 = a;        // 错误, 不能把一个数组直接赋值给另一个数组
        ```

        +   Warning: 一些编译器支持数组的赋值, 这就是所谓的编译器扩展, 但一般来说, 尽量避免使用非标准特性, 因为这样的程序很可能在其他编译器上无法工作.

+   **理解复杂的数组声明**

    +   比如可以定义一个存放指针的数组, 又因为数组本身就是对象, 所以允许定义数组的指针以及数组的引用.

        ```cpp
        int *ptrs[10];  // ptrs 是含有 10 个整型指针的数组
        int &refs[10] = /* ? */;  // 错误, 不存在引用的数组;
        int (*Parray)[10] = &arr;   // Parray 指向一个含有 10 个整数的数组
        int (&arrRef)[10] = arr;    // arrRef 引用一个含有 10 个整数的数组
        ```

        +   **默认情况下, 类型修饰符从右向左依次绑定.** 

        +   对于 ptrs 来说, 从右向左理解其含义: 首先知道我们定义的是一个大小为 10 的数组, 它的名字为 ptrs, 然后知道数组中存放的是指向 int 的指针;

        +   但是对于 Parray 来说, 从右向左理解就不太合理了. 因为数组的维度是紧跟着被声明的名字的, 所以就数组而言, 由内向外阅读要比从右向左好多了: 首先圆括号括起来的部分 `*Parray` 因为在 Parry 是个指针, 接下来观察右边, 可以知道 Parray 是个指向大小为 10 的数组的指针, 最后观察左边, 知道数组中的元素是 int. 这样最终的含义就明白无误了, Parray 是一个指针, 它指向一个 int 数组, 数组中包含 10 个元素. 同理 (&arrRef) 表示 arrRef 是一个引用, 它引用的对象是一个大小为 10 的数组, 数组中的元素的类型是 int.

        +   对修饰符的数量并没有特殊的限制:

            ```cpp
            int *(&array)[10] = ptrs; // array 是数组的引用, 该数组包含有 10 个指针
            ```

        +   **想要理解数组声明的含义, 最好的办法是从数组的名字开始按照由内而外的顺序阅读.**

+   访问数组元素

    +   数组下标, 定义类型为 `size_t`, 它是一种机器相关的无符号类型, 在 `#include <cstddef>` 中定义.
    +   当要遍历数组中的所有元素的时候, 最好使用范围 for 语句.

+   指针和数组

    +   数组的一个特性: 在用到数组名字的地方, 编译器会自动将其替换为一个指向数组首元素的指针. 也就是说, 在一些情况下, 数组的操作实际上就是指针的操作. 比如, **当使用数组作为一个 auto 变量的初始值时, 得到的类型就是指针而非数组:**

        ```cpp
        int ia[] = {1, 2, 3};
        auto ia2(ia);  // ia2 是一个整型指针, 指向 ia 的第一个元素.
        ```

        +   但是当使用 decltype 关键字时上述转换不会发生, `decltype(ia)` 返回的类型是由 3 个整数构成的数组.

    +   指针也是迭代器: 要获取数组的尾后指针, 首先可以设法获取数组尾元素之后的那个并不存在的元素的地址:

        ```cpp
        int *e = &ia[4];  // 尾后指针也不指向具体的元素, 因此不能对尾后指针执行解引用或递增的操作.
        ```

+   标准库函数 begin 和 end

    +   C++11 标准引入了两个名为 begin 和 end 的函数, 使用数组作为它们的参数, 这两个函数定义在 `#include <iterator>` 头文件中.

        ```cpp
        int ia[] = {1, 2, 3};
        int *beg = begin(ia);
        int *end = end(ia);
        ```

+   指针运算: 给指针加上或减去某整数值, 结果仍是指针.

    +   **注意: 给指针加上一个整数, 得到的新指针仍需指向同一数组的其他元素, 或者指向同一数组的尾元素的下一个位置. 如果计算所得的指针超出了上述范围就将产生错误, 而且这种错误编译器一般发现不了.**

    +   和迭代器一样, 两个指针相减的结果是它们之间的距离, 参与运算的指针必须指向同一个数组当中的元素:

        ```cpp
        auto n = end(arr) - begin(arr);
        ```

        +   相减的结果的类型是一种名为 `ptrdiff_t` 的标准库类型. 和 `size_t` 一样, 它也定义在 `<cstddef>` 头文件中.

        +   只要两个指针指向同一个数组的元素, 或者指向该数组尾元素的下一位置, 就能利用关系运算符对其进行比较.

        +   内置的下标运算符所用给的索引值不是无符号类型 (也就是说可以在下标中使用负值, 见下面的例子), 这一点与 vector 和 string 不一样.

            ```cpp
            int *p = &arr[2];
            int k = p[-2];  // p[-2] 是 arr[0] 表示的那个元素.
            ```

+   **使用数组初始化 vector 对象**:

    +   不允许使用一个数组为另一个内置类型的数组赋初值, 也不允许使用 vector 对象初始化数组. 

    +   相反的, 允许使用数组来初始化 vector 对象, 只需指明拷贝区域的首元素地址和尾后地址即可:

        ```cpp
        int arr[] = {1, 2, 3};
        vector<int> ivec(begin(arr), end(arr));
        ```

+   多维数组: 严格来说, C++ 中没有多维数组, 通常所说的多维数组其实是数组的数组.

    +   多维数组的初始化: 

        ```cpp
        int ia[3][3] = {
          {1, 3, 4},
          {1, 3, 4},
          {9, 0, 1}
        };
        ```

    +   多维数组的下标引用

        +   如果表达式含有的下标运算符和数组的维度一样多, 该表达式的结果将是给定类型的元素; 反之, 如果表达式含有的下标运算符数量比数组的维度小, 则表达式的结果将是给定索引处的一个内层数组:

            ```cpp
            ia[2][2] = 4;
            int (&row)[3] = ia[1]; // 把 row 绑定到 ia 的第二个 3 元素的数组上.
            ```

+   **使用范围 for 语句处理多维数组**

    +   首先说明: **要使用范围 for 语句处理多维数组, 除了最内层的循环外, 其他所有循环的控制变量都应该是引用类型.**

    +   比如:

        ```cpp
        for (auto &row : ia)
          for (auto &col : row)
            col = 0;
        ```

        上面这个例子中, 因为要改变数组元素的值, 所以使用了引用类型来作为循环控制变量, 但其实还有一个深层次的原因促使我们这么做, 如下:

        ```cpp
        for (const auto &row : ia)
          for (auto col : row)
            cout << col << endl;
        ```

        这个循环中并没有任何写操作, 可是我们还是将外层循环的控制变量声明成了引用类型, 这是**为了避免数组被自动转化成指针**. 假设不使用引用类型, 则循环如下形式:

        ```cpp
        for (auto row : ia)
          for (auto col : row)
        ```

        程序将无法通过编译, 这是因为, 注意到 ia 中的元素都是大小为 3 的**数组**, **因为 row 不是引用类型, 所以编译器初始化 row 时会自动将这些数组形式的元素转化成指向该数组内首元素的指针.** 这样得到的 row 类型就是 `int*`,  显然内层的循环就不合法了, 编译器将试图在一个 `int*` 内遍历, 这显然和程序的初衷相去甚远.

+   当程序使用多维数组的名字时, 也会自动将其转化成指向数组首元素的指针. 因为多维数组实际上是数组的数组, 所以由多维数组名转换得来的指针实际上是指向第一个内层数组的指针:

    ```cpp
    int ia[3][4];
    int (*p)[4] = ia;  // p 指向含有 4 个整数的数组
    p = &ia[2];        // p 指向 ia 的尾元素
    ```

    +   注意指针 p 指向一个维度为 4 的数组. 

    +   随着 C++11 新标准的提出, 通过使用 auto 或者 decltype 就能尽可能地避免在数组前面加上一个指针类型:

        ```cpp
        // 输出 ia 中每个元素的值, 每个内层数组各占一行
        // p 指向含有 4 个整数的数组
        for (auto p = ia; p != ia + 3; ++p){
          // q 指向 4 个整数数组的首元素, 也就是说, q 指向一个整数
          for (auto q = *p; q != *p + 4; ++q)
            cout << *q << ' ';
          cout << endl
        }
        ```

        +   外层 for 循环首先声明一个指针 p 并令其指向 ia 的第一个内层数组, 然后一次迭代知道 ia 的全部 3 行都处理完位置.
        +   内层的 for 循环负责输出内层数组所包含的值. 它首先令指针 q 指向 p 当前所在行的第一个元素. `*p` 是一个含有 4 个整数的数组, 像往常一样, 数组名被自动转换为指向该数组首元素的指针. 为了获得内层for 循环的终止条件, 再一次解引用 p 得到指向内层数组首元素的指针, 给它加上 4 就得到了终止条件.
        +   使用标准库提供的 `begin` 和 `end` 函数, 结果更为简洁, 注意内层循环为 `for (auto q = begin(*p); q != end(*p); ++q)`; 

    +   类型别名简化多维数组指针:

        ```cpp
        using int_array = int[4]; 
        // 等价的: typedef int int_array[4];

        for (int_array *p = ia; p != ia + 3; ++p)
          for (int *q = *p; q != *p + 4; ++q)
            cout << *q << ' ';
          cout << endl;
        ```

        ​

### 习题

+   习题 3.6: 使用范围 for 将字符串中的所有字符用 X 代替

    ```cpp
    #include <iostream>
    #include <string>
    using namespace std;

    int main(){
      string str = "adbcd";
      for (auto &s : str)
        s = 'X';
      cout << str << endl;
      return 0;
    }
    ```

+   习题 3.7: 如果将上一题中循环控制变量的类型设为 char 将会发生什么..

    +   就本题而言, 设为 char 不会对运行结果造成影响, 因为 auto 自动推断出字符串 str 的元素类型, 结果同样是 char.

+   习题 3.10: 编写一段程序, 读入一个包含标点符号的字符串, 将标点符号去除后输出字符串的剩余部分;

    ```cpp
    #include  <iostream>
    #include <string>
    using namespace std;

    int main(){
      string str = "ad,b,cd";
      string newstr;
      for (auto &s : str){
        if (!ispunct(s))
          newstr += s;
      }
      cout << newstr << endl;
      return 0;
    }
    ```

+   习题 3.17: 从 cin 中读入一组词并把它们存入一个 vector 对象, 然后设法把所有词都改为大写形式, 输出改变后的结果, 每个词占一行.

    ```cpp
    #include <iostream>
    #include <string>
    #include <cctype>
    #include <vector>

    template<typename T>
    void printArr(const T arr){
        for (auto c : arr)
            cout << c << " ";
        cout << endl;
    }

    int main(){
      vector<string> sen;
      string word;
      while(cin >> word){
        sen.push_back(word);
      }
      for (auto &word : sen)
        for(auto &c : word)
          c = toupper(c);
      printArr(sen);
      return 0;
    } 
    ```

+   习题 3.20: 读入一组整数并把它们存入一个 vector 对象, 将每对相邻整数的和输出出来; 改写你的程序, 这次要求先输出第一个和最后一个元素的和, 接着输出第二个和倒数第二个元素的和.

    ```cpp
    #include <iostream>
    #include <string>
    #include <vector>

    using namespace std;

    template<typename T>
    void printArr(const T arr){
        for (auto c : arr)
            cout << c << " ";
        cout << endl;
    }

    int main(){

        vector<int> ivec, sum1, sum2;
        int i;
        while(cin >> i){
            ivec.push_back(i);
        }
        for (decltype(ivec.size()) i = 0; i != ivec.size() - 1; ++i)
            sum1.push_back(ivec[i] + ivec[i + 1]);
        for (decltype(ivec.size()) i = 0; i != ivec.size() / 2 + 1; i++)
            if (i != ivec.size() - 1 - i)
                sum2.push_back(ivec[i] + ivec[ivec.size() - 1 - i]);
            else
                sum2.push_back(ivec[i]);

        printArr(ivec);
        printArr(sum1);
        printArr(sum2);

    }
    ```

+   习题 3.25: 3.3.3节(第 93 页) 划分分数段的程序是使用下标运算符实现的, 利用迭代器改写该程序.

    ```cpp
    auto it = scores.begin();
    while (cin >> grade)
      if (0 <= grade)
        ++(*(it + grade/10));
    ```

+   **习题 3.26:** 在 100 页的二分搜索程序中, 为什么用的是 `mid = beg + (end - beg)/2;` 而非 `mid = (beg + end)/2;`? 

    +   **C++ 并没有定义两个迭代器的加法运算, 实际上直接把两个迭代器加起来是没有意义的. 与之相反, C++ 定义了迭代器的减法运算, 两个迭代器相减的结果是它们之间的距离, 也就是说, 将运算符右侧的迭代器向前移动多少个元素后可以得到左侧的迭代器** 
    +   另外, C++ 还定义了迭代器与整数的加减法运算, 用于控制迭代器在容器中的左右移动.
    +   在本题中, 因为迭代器的加法不存在, 所以 `mid = (beg + end)/2;` 不合法. `mid = beg + (end - beg)/2;` 的含义是, 先计算 `end - beg` 的值得到容器中的元素的个数, 然后控制迭代器从开始处向右移动二分之一容器的长度, 从而定位到容器正中间的元素.

+   习题 3.29: 相比于 vector 来说, 数组有哪些缺点, 请列举一些:

    +   元素个数确定, (数组大小不变), 不能随意向数组中增加额外的元素, 虽然在某些情况下运行时性能较好, 但是 与 vector 相比就损失了灵活性. 具体说来, 数组的维度在定义的时候已经确定, 如果我们想要更改数组的长度, 只能创建一个更大的新数组, 然后把原数组中的所有元素复制到新数组中, 我们也无法像 vector 那样使用 size 函数直接获取数组的维度, 如果是字符数组, 可以调用 strlen 函数得到字符串的长度; 如果是其他的数组, 只能使用 `sizeof(array)/sizeof(array[0])` 的方式计算数组的维度.

+   习题 3.34: 假定 p1 和 p2 指向同一个数组中的元素, 则下面的程序的功能是什么? 什么情况下该程序非法?

    ```cpp
    p1 += p2 - p1;
    ```

    +   让指针 p1 指向 p2 所指的元素.

    ​



## 2017 年 7 月 30 日

### 2.6 节 自定义数据结构

### 知识

+   **类体右侧的表示结束的花括号后必须写一个分号**, 这是因为类体后面可以紧跟变量名以示对该类型对象的定义, 所以分号必不可少.

+   头文件保护符:

    ```cpp
    #ifndef SALES_DATA_H  // SALES_DATA_H 称为预处理变量
    #define SALES_DATA_H
    /* empty */
    #endif
    ```

    +   整个程序中的预处理变量包括头文件保护符必须唯一, 通常的做法是基于头文件中的类的名字来构建保护符的名字, 同时, 一般预处理变量的名字全部大写.

+   命名空间的 using 声明: 

    +   目前为止, 我们用到的库函数基本上都属于名字空间 std;

    +   有了 using 声明就无需专门的前缀(如 namespace::) 也能使用所需的名字, 形式如下:

        ```cpp
        using namespace::name; // 比如 using std::cin;
        ```

        +   这种方法对于每个名字都需要独立的 using 声明;

    +   **头文件不应包含 using 声明**: 这是因为头文件的内容会拷贝到所有引用它的文件中去, 如果头文件里有某个 using 声明, 那么每个使用该头文件的文件都会有这个声明, 可能会产生名字冲突.

+   标准库类型 string: (string 定义在名字空间 std 中)

+   定义和初始化 string 对象:

    +   string 对象默认初始化为一个空 string
    +   如果提供一个字符字面值, 那么该字面值中除了最后那个空字符外, 其他所有字符都被拷贝到新创建的 string 对象中去.
    +   如果提供一个数字和一个字符, 如 `string a(3, 'c');`, 则 string 对象的内容是给定字符连续重复若干次后得到的序列, 即 `ccc`.

+   直接初始化和拷贝初始化: 如果使用等号初始化一个变量, 实际上执行的是拷贝初始化, 编译器把等号右侧的初始值拷贝到新创建的对象中去; 如果不使用等号, 则执行的是直接初始化. 如果初始化用到的值有多个, 一般来说只能使用直接初始化, 如 `string a(3, 'c');`, 但是 `string b = string(3, 'c');` 用到的是拷贝初始化.

+   string 的操作:

    ```cpp
    os << s    //将 s 写到输出流中, 返回 os
    is >> s    // 从 is 中读取字符串赋给 s, 字符串以空白分隔, 返回 is
    getline(is, s)  // 从 is 中读取一行赋给 s, 返回 is
    s.empty()  // s 为空返回 true, 否则返回 false
    s.size()   // 返回  s 中字符的个数
    s[n]       // 返回 s 中第 n 个字符的引用, n 从 0 计数
    s1 + s2    // 返回 s1 和 s2 连接后的结果, 还有 += 操作
    s1 = s2    // 用 s2 的副本代替 s1 中原来的字符
    s1 == s2   // 如果 s1 和 s2 中所含的字符完全一样, 则它们相等; 
               // string 对象相等性判断对字母的大小写敏感
    <, <=, >, >= // 利用字符在字典中的顺序进行比较, 且对字母的大小写敏感.
    ```

+   读取未知数量的 string 对象:

    ```cpp
    int main(){
      string word;
      while(cin >> word)
        cout << word << endl;
    }
    ```

    +   **while 语句中的条件负责在读取时检测流的情况, 如果流有效, 也就是说没遇到文件结束标记或非法输入, 那么执行 while 语句内部的操作**.
    +   可以使用 `getline` 读取一整行, 注意该函数会返回流参数, 所以也可以使用 `while(getline(cin, s))`. 注意 `getline` 从给定的输入流中读取数据, 直到遇到换行符为止, 此时换行符也被读取进来, 但是并不存储在最后的字符串中.
    +   empty 操作: `if (!line.empty()) cout << line << endl;`, 如果 line 非空, 输出.

+   `string::size_type` 类型

    +   string 类和其他大多数标准库类型都定义了几种配套的类型, 这些配套类型体现了标准库类型与机器无关的特性, 类型 `size_type` 即是其中的一种, 在具体使用的时候, 通过作用域操作符 (`::`) 来表明名字 `size_type` 是在 string 中定义的.
    +   C++11 新标准允许编译器使用 auto 或者 decltype 来推断变量的类型: `auto len = line.size()`.
    +   **由于 size 函数返回的是一个无符号整数, 因此切记, 如果在表达式中混用了带符号数和无符号数将可能产生意想不到的结果.** 假设 n 是一个具有负值的 int, 那么 `s.size() < n` 的判断结果几乎可以肯定是 true, 这是因为负值 n 会自动的转换成一个较大的无符号值.
    +   **Tip: 如果一条表达式中已经有了 size() 函数就不要再使用 int 了, 这样可以避免混用 int 和 unsigned 可能带来的问题.**

+   字面值和 string 对象相加: **当把 string 对象和字符字面值或字符串字面值混在一条语句中使用时, 必须确保每个加法运算符 (+) 两侧的运算对象至少有一个是 string.**

+   切记: 字符串字面值和 string 是不同的类型! (`"hello" + "\n"` 是会报错滴.)

+   建议使用 C++ 版本的 C 标准库头文件: C 语言的头文件形如 `name.h`, C++ 则将这些文件命名为 `cname`, 也就是去掉了 `.h` 后缀, 以及在 `name` 之前加上了 c, 这里的 c 表示这是一个属于 C 语言标准库的头文件. 特别的, 在名为 cname 的头文件中定义的名字从属于命名空间 std, 而定义在 .h 的头文件中的则不然.

+   `cctype` 头文件中定义的函数

    ```cpp
    isalnum(c)  // 当 c 是字母或数字为真
    isalpha(c)  // 当 c 是字母时为真
    iscntrl(c)  // c 为控制字符
    isdigit(c)  // c 为数字
    isgraph(c)  // c 不是空格但可打印时为真
    islower(c)  // c 为小写字母
    isprint(c)  // c 为可打印字符时为真 (即 c 是空格或 c 具有可视形式)
    ispunct(c)  // c 是标点符号(即 c 不是控制字符, 数字, 字母, 可打印空白中的一种)
    isspace(c)  // c 是空白(即 c 是空格, 横向制表符, 纵向制表符, 回车符, 换行符, 进纸符中的一种
    isupper(c)  // c 是大写字母
    isxdigit(c) // c 是十六进制
    tolower(c)  // c 是大写字母则输出小写字母, 否则输出 c
    toupper(c)  // c 是小写字母则输出大写字母, 否则输出 c
    ```

+   range for 语句

    ```cpp
    for (declaration : expression)
      statement
      
    // 其中 expression 是一个对象, 用于表示一个序列; 
    //declaration 部分负责定义一个变量, 该变量将用于访问序列中的基础元素.
      
    // 比如
    string str("Hello World!!!")
    for (auto c : str)
      cout << c << endl;

    // 如果要改变字符串中的字符
    for (auto &c : str)
      c = toupper(c)
    cout << s << endl;  // 将得到 HELLO WORLD!!!
    ```

    ​

### 习题

+   习题 2.39: 编译下面的程序观察其运行结果, 注意, 如果忘记写类定义体后面的分号会发生什么情况? 记录下相关信息, 以后可能会有用:

    ```cpp
    struct Foo {/* empty */} // 注意没有分号
    int main(){
      return 0;
    }
    ```

    +   结果是报错: `error: expected ‘;’ after struct definition`.

+   ​



## 2017 年 7 月 29 日

### 2.5.3 节 (decltype 类型指示符)

### 知识

+   有时希望从表达式的类型推断出要定义的变量的类型, 但是不想用该表达式的值来初始化变量, 为了满足这一要求, C++11 新标准引入 decltype:

    ```cpp
    decltype(f()) sum = x; // sum 的类型就是函数 f 的返回类型 
    ```

    +   注意 decltype 处理顶层 const 和引用的方式和 auto 有些许不同, 如果 decltype 作用于一个变量, 则返回该变量的类型(包括顶层 const 和引用在内):

        ```cpp
        const int ci = 0, &cj = ci;
        decltype(ci) x = 0;  // x 的类型是 const int
        decltype(cj) y = x;  // y 的类型是 const int&, y 绑定到变量 x
        decltype(cj) z;      // 错误: z 是一个引用, 必须初始化
        ```

    +   **如果 decltype 使用的表达式不是一个变量, 则 decltype 返回的表达式结果对应的类型.** (这里真的要注意, 也是下面程序第二点将要指出的, 有些表达式将向 decltype 返回一个引用类型, 一般来说当这种情况发生时, 意味着该表达式的结果对象能作为一条赋值语句的左值.)

        ```cpp
        // decltype 的结果可以是引用类型
        int i = 42, *p = &i, &r = i;
        decltype(r + 0) b; // 正确, 加法的结果是 int, 因此 b 是一个 int
        decltype(*p) c;    // 错误, c 是 int&, 必须初始化
        ```

        +   因为 r 是一个引用, 因此 decltype(r) 的结果是引用类型. 如果想让结果类型是 r 所指的类型, 可以把 r 作为表达式的一部分, 比如 `r + 0`, 显然这个表达式的结果将是一个具体值而非一个引用.

        +   另一方面, 如果表达式的内容是解引用操作, 则 decltype 将得到引用类型. 由于解引用指针可以得到指针所指的对象, 而且还能给这个对象赋值, 因此 `decltype(*p)` 的结果类型**就是 int&, 而非 int**.

        +   decltype 和 auto 的另一处重要区别是, decltype 的结果类型与表达式形式密切相关. 有一种情况需要特别注意, 如下:

            ```cpp
            // decltype 的表达式如果是加上了括号的变量, 结果将是引用
            decltype((i)) d;    // 错误: d 是 int&, 必须初始化
            decltype(i) e;      // 正确: e 是一个未初始化的 int
            ```

            +   如果给变量加上一层或多层括号, 编译器会把它当成一个表达式. 变量是一种可以作为赋值语句左值的特殊表达式, 所以这样的 decltype 就会得到引用类型.


            +   切记: `decltype((variable))` (注意双层括号) 的结果永远是引用, 而 `decltype(variable)` 结果只有当 variable 本身就是一个引用时才是引用 (注意 variable 是解引用指针的情况).

        +   注意下面的习题 2.37: **赋值是会产生引用的一类典型表达式, 引用的类型就是左值的类型.**

            ```cpp
            decltype(a = b) c = a; // 推断出来的 c 的类型是 int&.
            ```
    
            ​

### 习题

+   习题 2.36: 关于下面的代码, 请指出每一个变量的类型以及程序结束时它们各自的值.

    ```cpp
    int a = 3, b = 4;
    decltype(a) c = a;
    decltype((b)) d = a;
    ++c;
    ++d;
    ```

    +   c 的类型是 int, 而 d 的类型是 int&;
    +   程序结束时, a = 4, b = 4, c = 4, d = 4;

+   习题 2.37: **赋值是会产生引用的一类典型表达式, 引用的类型就是左值的类型.** 也就是说, 如果 i 是 int, 则表达式 i = x 的类型是 int&. 根据这一特点, 请指出下面的代码中每一个变量的类型和值.

    ```cpp
    int a = 3, b = 4;
    decltype(a) c = a;
    decltype(a = b) d = a;
    ```

    +   c 的类型是 int, d 的类型是 int&
    +   a = 4, b = 4, c = 3, d = 4;

+   习题 2.38: 说明由 decltype 指定类型和由 auto 指定类型有何区别. 请举出一个例子, decltype 指定的类型与 auto 指定的类型一样; 再举一个例子, decltype 指定的类型与 auto 指定的类型不一样.

    +   思路: **auto 与 decltype 是两种类型推断方式**

    +   参考答案:

        +   区别主要体现在三个方面: 第一,  auto 类型说明符用编译器计算变量的初始值来推断其类型, 而 decltype 虽然也让编译器分析表达式并得到他的类型, 但是不实际计算表达式的值.
        +   第二, 编译器推断出来的 auto 类型有时候和初始值的类型并不完全一样, 编译器会适当地改变结果类型使其更符合初始化规则. 例如, auto 一般会忽略掉顶层 const, 而把底层 const 保留下来. 与之相反, decltype 会保留变量的顶层 const.
        +   第三, 与  auto 不同, decltype 的结果类型与表达式形式密切相关, 如果变量名加上一对括号, 则得到的类型与不加括号时会有不同. 如果 decltype 使用的是一个不加括号的变量, 则得到的结果就是该变量的类型; 如果给变量加上了一层或多层括号, 则编译器将推断得到引用类型.

    +   我的解答: 

        +   区别一: decltype 处理顶层 const 与 auto 类型不同, decltype 可以返回变量的类型 (包括顶层 const 和引用在内), 但是 auto 会忽略顶层 const.
        +   区别二: decltype 的结果类型与表达式形式密切相关, 如果变量名加上一对括号, 则得到的类型与不加括号时会有不同. decltype 声明的如 int 型变量可以不初始化,但是 auto 的对象必须初始化.
        +   如下代码:

        ```cpp
        // decltype 与 auto 相同
        int a = 0;
        decltype(a) b = a; 
        auto c = a;    // 此时 c 与 b 都是 int

        // decltype 与 auto 不同
        const int a = 0;
        decltype(a) b = a;
        auto c = a;   // 此时 c 为 int, 而 b 为 const int
        ```

        ​





## 2017 年 7 月 28 日

### 2.4 节以后

### 知识

+   const 限定符: 因为 const 对象一旦创建之后, 其值就不能再改变, 所以 const 对象必须初始化. 

+   默认状态下, const 对象仅在文件内有效; 如果想在多个文件之间共享 const 对象, 必须在变量的定义之前添加 extern 关键字.

    ```cpp
    //file_1.cc 定义并初始化了一个常量, 该常量能被其他文件访问
    extern const int bufSize = fcn();
    // file_1.h 头文件
    extern const int bufSize;  // 与 file_1.cc 中定义的 bufSize 是同一个
    ```

    `file_1.h` 头文件中的声明也由 extern 做了限定, 其作用是指明 bufSize 并非本文件 (即 `file_1.h` 文件) 所独有, 它的定义将在别处出现.

    为了说明这个意思, 我自己写了这样一个程序, 由 3 个文件构成:

    ```cpp
    // func.hpp
    // 在 func.hpp 文件中只是声明了一个函数 show()
    #ifndef FUNC_HPP
    #define FUNC_HPP

    void show();

    #endif

    // func.cpp
    // 在 func.cpp 文件中定义了 show() 函数, 同时定义了可供外部访问的 bufSize
    // show() 负责输出这个 bufSize 的大小
    #include <iostream>
    #include "func.hpp"

    extern const int bufSize = 512;

    void show(){

        std::cout << "Func.cpp >> bufSize: " << bufSize << std::endl;
    }

    // main.cpp
    // 要让 main.cpp 中能访问到 func.cpp 中定义的 bufSize, 需要在 main.cpp 中使用
    // extern const int bufSize 进行声明.
    #include <iostream>
    #include "func.hpp"

    using namespace std;
    extern const int bufSize;

    int main(){

        show();
        cout << "main.cpp >> bufSize: " << bufSize << endl;
        return 0;
    }

    // 运行程序, 使用
    // g++ -Wall -std=c++0x -o main main.cpp func.cpp
    // 输出如下: 
    Func.cpp >> bufSize: 512
    main.cpp >> bufSize: 512
    ```


+   现在注意 4 个问题:

    1.  如果在 main.cpp 中没有使用 `extern const int bufSize;`, 结果会报错, 说 `bufSize was not declared in this scope`.

    2.  如果对 main.cpp 中的 `extern const int bufSize = 100;` 进行了重新赋值, 结果会报错, 说

        `multiple definition of bufSize`.

    3.  如果将 main.cpp 中改为 `extern int bufSize;` 也就是去掉了 `const`, 程序正常运行...

    4.  如果将 `func.cpp` 中改为 `extern int bufSize = 512;`, 那么虽然程序可以正常运行, 但是编译的时候会出现警告: `warning: ‘bufSize’ initialized and declared ‘extern’`, 我想这应该是编译器想提醒我这个变量有在其他文件中被修改的风险.


+   顶层 const (top-level const) 与底层 const (low-level const): 顶层 const 表示指针本身是一个常量, 底层 const 表示指针所指的对象是一个常量. (更一般的, 顶层 const 可以表示任意对象是常量, 这一点对任何数据类型都适用, 如算术类型, 类, 指针等. 比如 `const int ci = 42;` 是一个顶层 const. 而底层 const 则与指针和引用等复合类型的基本类型部分有关. 比较特殊的是指针类型既可以是顶层 const 也可以是底层 const.)

+   常量表达式: 常量表达式的定义是指值不会改变并且在编译过程就能得到计算结果的表达式.

    +   **注意一个对象 (或者表达式) 是不是常量表达式由它的数据类型和初始值共同决定**, (显然, 字面值属于常量表达式, 用常量表达式初始化的 const 对象也是常量表达式), 例如:

    ```cpp
    const int max_files = 20; // max_files 是常量表达式
    int staff_size = 27; 
    // 尽管 staff_size 的初始值是个字面值常量, 但由于它的数据类型
    // 只是一个普通的 int 而非 const int, 所以不是常量表达式
    const int sz = get_size();
    // 另一方面, 尽管 sz 本身是一个常量, 但它的具体值直到运行时
    // 才能获得, 因此也不是常量表达式.
    ```

    +   C++11 规定, 允许将变量声明为 **constexpr** 类型以便由编译器来验证变量的值是否是一个常量表达式. **声明为 constexpr 的变量一定是一个常量**, 而且必须用常量表达式初始化.
    +   一般来说, 如果你认定变量是一个常量表达式, 那就把它声明为 constexpr 类型.
    +   字面值类型: (对声明 constexpr 时用到的类型必须有所限制, 使得常量表达式的值在编译时就能得到计算) 目前算术类型, 引用和指针都属于字面值类型, 而自定义类 Sales_item, IO 库, string 类型则不属于字面值类型, 也就不能被定义为 constexpr. 另外, 对于指针和引用, 它们的初始值都收到了严格限制, 一个 constexpr 指针的初始值必须是 nullptr 或者 0, 或者是存储于某个固定地址中的对象.

### 2.5 处理类型

+   使用 typedef 定义类型别名:

    ```cpp
    typedef double wages;
    typedef wages base, *p; // base 为 double 的同义词, p 是 double* 的同义词
    ```

+   使用 using 定义类型别名:

    ```cpp
    using SI = Sales_item;
    ```

+   **注意: 如果某个类型的别名指代的是复合类型或常量, 那么把它用到声明语句里就会产生意想不到的后果, 例如下面的声明语句用到了 pstring, 它实际上是类型 char* 的别名**

    ```cpp
    typedef char *pstring;
    const pstring cstr = 0; // cstr 是指向 char 的常量指针
    const pstring *ps;  // ps 是一个指针, 它的对象是指向 char 的常量指针
    ```

    **这里要十分注意:** `const pstring cstr = 0;` 与 `const char *cstr = 0;` 两者是不等价的, 前者是指向 char 的常量指针, 即顶层 const, 相当于 `char *const cstr = 0;`; 而后者是声明了一个指向 `const char` 的指针, 相当于底层 const. 那么:

    ```cpp
    // 当你使用下面的语句时就会报错
    const pstring cstr;  // 原因是由于是顶层 const, 你需要对 cstr 进行初始化.

    // 而下面的语句不需要初始化
    const char *cstr;  // 不会报错.
    ```

    对于最上面 `const pstring *ps;`, 要明白, 首先 ps 是一个指针, 然后 `const pstring` 为 **指向 char 的常量指针**, 所以 `const pstring *ps;` 的意义是 ps 是一个指针, 它的对象是指向 char 的常量指针. (注意常量指针与常量引用是不同的概念, 常量指针是顶层 const, 而常量引用是对 const 的引用, 也就是底层 const.)

    +   总结就是: 对于复合类型的类型别名与 const, 一定要小心, 不能简单地代入到声明或定义中. 

+   auto 类型说明符: auto 能让编译器通过初始值来推算变量的类型. **显然, auto 定义的变量必须有初始化.**

+   对于推断出来的 auto 类型, 有时候和初始值的类型并不完全一样:

    +   情况 1: 当引用作为初始值时, 真正参与初始化的其实是引用对象的值,(毕竟使用引用就是在使用引用的对象) 此时编译器以引用对象的类型作为 auto 的类型, 如下, a 的类型为 int 而不是引用.

        ```cpp
        int i = 0, &r = i;
        auto a = r;  // a 是一个整数
        ```

    +   情况 2: auto 一般会忽略掉顶层 const, 但是底层 const 则会保留下来:

        ```cpp
        const int ci = i, &cr = ci;
        auto b = ci;  // b 是一个整数 (ci 的顶层 const 特性被忽略了)
        auto c = cr;  // c 为一个整数
        auto d = &i;  // d 是一个整型指针
        auto e = &ci; // e 是一个指向整数常量的指针 (对常量对象取地址是一种底层 const)
        ```

        **如果希望推断出的 auto 类型是一个顶层 const, 需要明确指出**:

        ```cpp
        const auto f = ci; // f 此时为 const int
        ```

    +   情况 3: 还可以将引用的类型设为 auto, 原来的初始化规则仍然适用:

        ```cpp
        auto &g = ci;   // g 是一个整型常量引用, 绑定到 ci
        auto &h = 42;   // 错误: 不能为非常量引用绑定字面值
        const auto &j = 42;  // 正确, 可以为常量引用绑定字面值
        ```

        注意, 设置一个类型为 auto 的引用时, 初始值中的顶层常量属性仍然保留, 比如上面的 `auto &g = ci;`; *如果我们给初始值绑定一个引用 (比如上面的 `auto &h = 42;`), 则此时的常量就不是顶层常量了.* (**这句话不是很明白!!!!**)

### 习题

+   习题 2.26: 下面那些语句是合法的?

    ```cpp
    (a) const int buf; // (b)(c)(d) 就不不写了
    ```

    +   (a) 是非法的, const 对象一旦创建后其值就不能被修改, 所以 const 对象必须初始化. 应修改为 `const int buf = 10;`

+   习题 2.27: 下面那些初始化是合法的?

    ```cpp
    (a) int i = -1, &r = 0;
    (b) int *const p2 = &i2;
    (c) const int i = -1, &r = 0;
    (d) const int *const p3 = &i2;
    (e) const int *p1 = &i2;
    (f) const int &const r2;
    (g) const int i2 = i, &r = i;
    ```

    +   *提示 1:* **"对 const 的引用" (reference to const) 简称为 "常量引用"; 引用的类型必须与其所引用对象的类型一致, 但是有两个例外. 第一种例外是在初始化常量引用时允许使用任意表达式作为初始值, 只要该表达式的结果能转化为引用的类型即可. 尤其, 允许为一个常量引用绑定非常量的对象, 字面值, 甚至是一个一般表达式. 和常量引用一样, 指向常量的指针也没有规定其所指的对象必须是一个常量. 所谓指向常量的指针仅仅要求不能通过该指针改变对象的值, 而没有规定那个对象的值不能通过其他途径改变.**
    +   *提示 2:* **指针是对象而引用不是, 因此就像其他对象类型一样, 允许把指针本身设定为常量. 常量指针 (const pointer) 必须初始化, 而且一旦初始化完成, 则它的值 (也就是存放在指针中的那个地址) 就不能在改变了. 指针本身是一个常量并不意味着不能通过指针修改其所指对象的值, 能否这样做完全依赖所指对象的类型.**
    +   (a) 不合法, 因为 `r` 是引用, 需要绑定一个对象; (非常量引用 `r` 不能引用字面值常量 0)
    +   (b) 合法, p2 是指向整型的常量指针, p2 的值永不改变, 即 p2 永远指向变量 i2;
    +   (c) 合法, r 是一个常量引用, 此时 r 可以绑定到字面值常量 0;
    +   (d) 合法, p3 为指向常量的常量指针, p3 的值永不改变, 同时我们不能通过 p3 改变所指对象的值;
    +   (e) 合法, p1 为指向常量的指针, 我们不能通过 p1 改变所指对象的值;
    +   (f) 不合法, 引用本身不是对象, 因此不能让引用为 const; (只有 const 指针, 没有 const 引用);
    +   (g) 合法, i2 是一个 const, r 为一个常量引用.

+   习题 2.28: 找出其中不合法的:

    ```cpp
    (a) int i, *const cp;
    (e) const int *p;
    ```

    +   (a) 不合法, cp 是一个常量指针, 因其值不能被改变, 所以必须初始化
    +   (e) 是合法的, 但是 p 没有指向任何实际的对象 (注意与常量引用区分, 常量引用就需要初始化)

+   习题 2.30: 对于下面这些语句, 请说明对象被声明成了顶层 const 还是底层 const:

    ```cpp
    const int v2 = 0; int v1 = v2;
    int *p1 = &v1, &r1 = v1;
    const int *p2 = &v2, *const p3 = &i, &r2 = v2;
    ```

    +   其中 r2 和 p2 为底层 const, 而 p3 和 v2 是顶层 const;

+   习题 2.31: 假设已有上一个练习中所做的那些声明, 则下面的哪些语句是合法的? 请说明顶层 const 和底层 const 在每个例子中有何体现.

    ```cpp
    r1 = v2;
    p1 = p2; p2 = p1;
    p1 = p3; p2 = p3;
    ```

    +   首先我必须提醒一点, 上面不是初始化, 而是赋值操作....
    +   r1 是普通引用, `r1 = v2;` 的意思是将 v2 的值赋给 r1, 这没什么错误, (此时 r1 的值变化了, 会导致 v1 的值变化), 所以是合法的. 
    +   p1 是普通指针, `p1 = p2;` 使得普通指针指向 pointer to const p2, 这是不合法的. 而 `p2 = p1;` 合法
    +   p1 是普通指针, `p1 = p3;` 使得普通指针指向 p3 (包含底层 const 定义), 这是不合法的; 而 `p2 = p3;` 合法, 因为 p2 和 p3 包含相同的底层 const.

+   习题 2.35: 判断下列定义推断出来的类型是什么, 编程验证:

    ```cpp
    const int i = 42;
    auto j = i; 
    const auto &k = i;
    auto *p = &i;
    const auto j2 = i, &k2 = i;
    ```

    +   `j` 的类型是 int, 因为 auto 推断一般会忽略顶层 const
    +   `k` 的类型为 const int&, 整型常量引用
    +   `p` 的类型为 const int*, 指向整型常量的指针, 因为 auto 会保留底层 const
    +   `j2` 的类型为 const int



## 2017 年 7 月 24 日

+   习题 2.6: 下面的两组定义是否有区别, 如果有, 请叙述之;

    ```cpp
    int month = 9, day = 7;
    int month = 09, day = 07;
    ```

    +   注意第二组定义是错误的, 编译时将报错; 因为用八进制表示数, 数字 9 超过了八进制数能表示的范围.

+   `double a = b = 0.9` 是错误的, 在声明语句中声明多个变量时需要用逗号将变量名隔开, 而不能直接用赋值运算符连接.

+   习题 2.11: 指出下面的语句是声明还是定义:

    ```cpp
    (a) extern int ix = 1024;  // 定义了变量 ix
    (b) int iy;   // 声明并定义了变量 iy;
    (c) extern int iz; // 声明了变量 iz
    ```

    声明与定义的关系是: 声明使得名字为程序所知, 而定义则负责创建与名字关联的实体. 对于 (b), 如果 `iy` 定义在函数体之外, 将会被默认初始化为 0; 但是如果在 main 函数内部定义的话, 将不被初始化! 注意对 string 类型情况却是不同, 不管 string 类型是在函数体内还是函数体外定义的, 默认初始化为空串. 

+   习题 2.19: 说明指针和引用的主要区别.

    +   指针指向内存中的某个对象, 而引用 "绑定到" 内存中的某个对象, 它们都实现了对其他对象的间接访问, 二者的主要区别有两方面:
        +   第一, 指针本身就是一个对象, 允许对指针赋值和拷贝, 而且在指针的生命周期内它可以指向几个不同的对象; 引用不是一个对象, 无法令引用重新绑定到另外一个对象.
        +   第二, 指针无须在定义时赋初值, 和其他内置类型一样, 在块作用域内定义的指针如果没有被初始化, 也将拥有一个不确定的值; 引用则必须在定义时赋初值.

+   习题 2.24: 为什么下面的 p 合法?

    ```cpp
    int i = 42;
    void *p = &i;
    ```

    因为 `void*` 是一种特殊的指针类型, 可用于存放任意对象的地址. (我们对该地址中到底是个什么类型的对象并不了解.)





## 2017 年 7 月 23 日

+   习题 1.14: 对比 for 循环和 while 循环, 两种形式的优缺点各是什么?
    +   在循环次数已知的情况下, for 循环的形式显然更为简洁;
    +   而循环次数无法预知时, 用 while 循环实现更为适合. 用特定条件控制循环是否执行, 循环体中执行的语句可能导致循环判定条件发生变化.

+   习题 1.16: 编写程序, 从 cin 中读取一组数, 输出总和.


```cpp
#include <iostream>
using namespace std;

int main()
{
    int sum = 0, i = 0;
    while(cin >> i){  
        sum += i;
    }
    cout << sum << endl;
}

// 如果用 for 循环的话, 代码如下:
for(; cin >> i; ;) sum += i;
```

注意要运行这段代码, 需要考虑从键盘输入文件结束符, 在 Windows 系统中输入 Ctrl + Z; 在 Unix 和 Mac OS X 中使用 Ctrl + D. 比如:

```bash
# 在 Terminal 下输入如下数字, 最后按下 <Enter>
1 2 3<Enter>
<Ctrl+D>  # 按完 <Enter> 之后按下 Ctrl + D
6         # 得到最后的输出 6
```

此题考验的是不定次数的循环, 以及输入流结束判断.



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

