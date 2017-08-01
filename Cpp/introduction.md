# Introduction

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

        程序将无法通过编译, 这是因为, 注意到 ia 中的元素都是大小为 3 的数组, **因为 row 不是引用类型, 所以编译器初始化 row 时会自动将这些数组形式的元素转化成指向该数组内首元素的指针.** 这样得到的 row 类型就是 `int*`,  显然内层的循环就不合法了, 编译器将试图在一个 `int*` 内遍历, 这显然和程序的初衷相去甚远.

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

    ​

+   ​

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

