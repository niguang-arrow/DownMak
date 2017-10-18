# 动态内存分配

## 2017 年 10 月 17 日

### C++ 内存分区

摘抄自牛客网一道题目中的评论, 关于 C++ 内存分区的问题, 大家的见解还有一些细微的差异, 下面这个比较详细:

在C++中，内存分成5个区，他们分别是堆、栈、自由存储区、全局/静态存储区和常量存储区。       

栈，就是那些由编译器在需要的时候分配，在不需要的时候自动清楚的变量的存储区。里面的变量通常是局部变量、函数参数等。      

**堆，就是那些由new分配的内存块**，他们的释放编译器不去管，由我们的应用程序去控制，一般一个 new 就要对应一个delete。如果程序员没有释放掉，那么在程序结束后，操作系统会自动回收。      

自由存储区，就是那些由 malloc 等分配的内存块，他和堆是十分相似的，不过它是用 free 来结束自己的生命的。      

全局/静态存储区，全局变量和静态变量被分配到同一块内存中，在以前的 C语言中，全局变量又分为初始化的和未初始化的（初始化的全局变量和静态变量在一块区域，未初始化的全局变量与静态变量在相邻的另一块区域，同时未被初始化的对象存储区可以通过 `void*` 来访问和操纵，程序结束后由系统自行释放），在C++里面没有这个区分了，他们共同占用同一块内存区。      

常量存储区，这是一块比较特殊的存储区，他们里面存放的是常量，不允许修改（当然，你要通过非正当手段也可以修改，而且方法很多）



### C++ 动态内存与智能指针

+   `shared_ptr` 允许多个指针指向同一个对象;
+   `unique_ptr` 则 "独占" 所指向的对象;
+   `weak_ptr` 伴随类, 它是一种弱引用, 指向 `shared_ptr` 所管理的对象.



### `shared_ptr` 类

+   `shared_ptr` 与 `unique_ptr` 都含有 `p.get()` 方法, 用于返回 p 中保存的指针. (要小心使用, 若智能指针释放了其对象, 返回的指针所指向的对象也消失了.)

+   `shared_ptr` 的操作:

    ```cpp
    make_shared<T>(args);
    shared_ptr<T> p(q); // q 中的指针必须能转换为 T*
    p = q;
    p.get(); // 返回 p 中保存的指针
    p.unique(); // p.use_count() 为 1 返回 true, 否则返回 false
    p.use_count(); // 返回与 p 共享对象的智能指针数量
    ```

+   最安全的分配和使用动态内存的方法是调用 `make_shared` 函数.

+   无论何时我们拷贝一个 `shared_ptr`, 引用计数就会递增. 例如, 当用一个 `shared_ptr` 初始化另一个 `shared_ptr`, 或将它作为参数传递给一个函数, 以及作为函数的返回值, 计数器就会递增.

+   当我们给 `shared_ptr` 赋予一个新值, 或是 `shared_ptr` 被销毁(例如一个局部 `shared_ptr` 离开作用域), 计数器就会递减.

+   `shared_ptr` 自动销毁所管理的对象, 还会自动释放相关联的内存. `shared_ptr` 通过析构函数完成对象的销毁工作, `shared_ptr` 的析构函数会递减它所指向的对象的引用计数, 如果引用计数变为 0, `shared_ptr` 的析构函数就会销毁对象, 并释放它占用的内存.

### 直接管理内存

+   使用 new 动态分配和初始化对象

    在自由空间(或者堆)中分配的内存是无名的, 因此, new 无法为其分配的对象命名, 而是返回一个指向该对象的指针: `int *pi = new int(42);`

+   动态分配 const 对象: new 返回的指针是一个指向 const 的指针.

+   传递给 delete 的指针必须是指向动态分配的内存, 或者是一个空指针. 释放一块并非 new 分配的内存, 或将相同的指针值释放多次, 其行为是未定义的.

+   返回指向动态内存的指针(而不是智能指针)的函数给其调用者增加了一个额外的负担 -- 调用者必须记得释放内存.

+   delete 之后使用 nullptr 重置指针值

### `share_ptr` 和 `new` 的结合使用

+   我们可以使用 new 返回的指针来初始化智能指针. (如果智能指针不初始化, 那么会被编译器初始化为一个空指针). 另外由于接受指针参数的智能指针构造函数是 explicit 的, 因此, 我们不能将一个内置指针隐式转换为一个智能指针,必须使用直接初始化形式来对智能指针进行初始化; 同样的, 一个返回智能指针的函数不能在其返回语句中隐式转换为一个普通指针.

    ```cpp
    shared_ptr<int> p1(new int(42));
    shared_ptr<int> p2 = new int(42); // 这样是不行的!
    ```

+   **程序员非常容易忘记释放资源**, 如果在资源分配和释放之间发生了异常, 程序也会发生资源泄漏.

+   (417页)如果你使用智能指针管理的资源不是 new 分配的内存, 记住传递给它一个删除器! 比如 416 页中的网络库: (这个网络库不具有良好定义的析构函数)

    ```cpp
    struct destination;  // 表示我们正在连接什么
    struct connection;  // 使用连接所需的信息
    connection connect(destination*);  // 打开连接
    void disconnect(connection); // 关闭给定连接
    void f(destination &d /* 其他参数 */) {
      // 获得一个连接, 记住使用完后要关闭它
      connection c = connect(&d);
      // 使用连接
      // 如果我们在 f 退出前忘记调用 disconnect, 就无法关闭 c 了
    }
    ```

    如果 connection 有一个析构函数, 就可以在 f 结束时由析构函数自动关闭连接. 因此, 我们可以使用 `shared_ptr` 来保证 connection 被正确的关闭. 

+   **在默认情况下, `shared_ptr` 假定它们指向的是动态内存.** 因此, **当一个 `shared_ptr` 被销毁时, 它默认对它管理的指针进行 delete 操作.** 为了用 `shared_ptr` 来管理上面的 connection, 我们必须首先定义一个函数来替代 delete. 这个删除器函数必须能够完成对 `shared_ptr` 中保存的指针进行释放的操作.

    ```cpp
    void end_connection(connection *p) { disconnect(*p); }

    void f(destination &d /* 其他参数 */) {
      connection c = connect(&d);
      shared_ptr<connection> p(&c, end_connection);
      // 使用连接
      // 当 f 退出时(即使由于异常而退出), connection 会被正确关闭.
    }
    ```




### `unique_ptr`

+   定义 `unique_ptr` 时, 需要将其绑定到一个 new 返回的指针上. 

+   由于一个 `unique_ptr` 拥有它指向的对象, **因此它不支持普通的拷贝或赋值操作**.

+   可以使用 `release()` 或 `reset()` 将指针的所有权从一个非 const 的 `unique_ptr` 转移到另一个 `unique_ptr`.

    +   `release()` 返回 `unique_ptr` 当前保存的指针并将其(这里的"其"指的是 `unique_ptr`)置为空. 调用 `release` 会切断 `unique_ptr` 和它原来管理的对象间的联系. `release` 返回的指针常用来初始化另一个智能指针或给另一个智能指针赋值.
    +   `reset()` 成员接受一个可选的指针参数, 令 `unique_ptr` 重新指向给定的指针, 比如: `p2.reset(p1.release())`

+   不能拷贝 `unique_ptr` 有一个例外: 我们可以拷贝或赋值一个将要被销毁的 `unique_ptr`. 最常见的例子是从函数返回一个 `unique_ptr`

+   向 `unique_ptr` 传递删除器: (前面 `shared_ptr` 的例子)

    ```cpp
    void f(destination &d /* 其他需要的参数 */) {
      connection c = connect(&d);
      // 当 p 被销毁时, 连接会被关闭
      unique_ptr<connection, decltype(end_connection)*> p(&c, end_connection);
      // 当 f 退出时, connection 会被正确关闭
    }
    ```

    由于 `decltype(end_connection)` 返回一个函数类型, 所以我们必须添加一个 `*` 来指出我们正在使用该类型的一个指针.



### `shared_ptr` 与 `unique_ptr`

+   (599 页)`shared_ptr` 是在运行时绑定删除器的, 而 `unique_ptr` 在编译时绑定删除器. 通过在编译时绑定删除器, `unique_ptr` 避免了间接调用删除器的运行时开销. 通过在运行时绑定删除器, `shared_ptr` 使用户重载删除器更为方便.



### `weak_ptr` 

+   `weak_ptr` 指向由一个 `shared_ptr` 管理的对象, 将一个 `weak_ptr` 绑定到 `shared_ptr` 不会改变 `shared_ptr` 的引用计数. 当我们创建一个 `weak_ptr` 时, 要用一个 `shared_ptr` 来初始化它. 由于 `weak_ptr` 不会改变 `shared_ptr` 的引用计数, 所以 `weak_ptr` 的对象可能被释放.

+   由于对象可能不存在, 因此我们不能用 `weak_ptr` 直接访问对象, 而 **必须调用 lock()**

    +   该函数检查 `weak_ptr` 指向的对象是否存在, 如果存在, 返回一个指向该对象 `shared_ptr`, 否则返回一个空 `shared_ptr.`

+   `weak_ptr` 的方法:

    ```cpp
    w.reset(); // 将 w 置为空
    w.use_count(); // 与 w 共享对象的 shared_ptr 的数量
    w.expired(); // (过期的, 失效的) 若 w.use_count() 为 0, 返回 true
    w.lock();  // 如果 expired 为 true, 返回一个空 shared_ptr, 否则返回一个指向 w 的对象的 shared_ptr
    ```



### new 和 数组

+   可以用一个表示数组类型的类型别名来分配一个数组;
+   通常称 `new T[]` 分配的内存为 "动态数组", 但这种叫法有一定的误导. **当用 new 分配一个数组时, 我们并未得到一个数组类型的对象, 而是得到一个数组元素类型的指针**. 由于分配的内存并不是一个数组类型, 因此不能对动态数组调用 `begin` 或 `end`, 这些函数使用数组的维度来返回指向首元素和尾后元素的指针. 出于同样的原因, 也不能用范围 for 语句来处理 (所谓的) 动态数组中的元素.
+   动态分配一个空数组是合法的, 但是不能解引用相应的指针.



#### 释放动态数组

+   `delete[] pa;` 数组中的元素按逆序销毁, 即最后一个元素首先被销毁, 然后是倒数第二个, 以此类推.



### 智能指针和动态数组

+   标准库提供了一个可以管理 new 分配的动态数组的 `unique_ptr` 版本. 为了用一个 `unique_ptr` 来管理动态数组, 我们必须在对象类型后面跟一对空方括号:

    ```cpp
    unique_ptr<int[]> u(new int[42]);
    u.release(); // 自动用 delete[] 来销毁其指针.
    ```

+   `unique_ptr` 指向一个数组时, 我们可以使用下标运算符来访问数组中的元素.

+   `shared_ptr` 不支持管理动态数组. 如果希望使用 `shared_ptr` 来管理动态数组, 则需要自己定义删除器: (默认情况下, `shared_ptr` 是使用 delete 来销毁它指向的对象的.)

    ```cpp
    shared_ptr<int> sp(new int[10], [](int *p) { delete[] p; });
    sp.reset(); // 使用 lambda 来释放数组
    ```

    另外, `shared_ptr` 未定义下标运算符, 而且智能指针类型不支持指针算术运算. 因此, 为了访问数组中的元素, 必须用 get 获取一个内置指针, 然后用它来访问数组:

    ```cpp
    for (size_t i = 0; i != 10; ++i) {
      *(sp.get() + i) = i;
    }
    ```



### allocator 类

+   new 将内存分配和对象构造组合在了一起, 而 delete 将对象析构和内存释放组合在了一起. 在分配大块内存时, 我们希望在这块内存上按需构造对象. 在此情况下, 我们希望将内存分配和对象构造分离. 这意味着我们可以分配大块内存, 而只在真正需要时才真正执行对象创建操作(同时付出一定开销).