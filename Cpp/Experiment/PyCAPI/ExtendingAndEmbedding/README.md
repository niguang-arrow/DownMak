# Extending and Embedding Python in Application

## 2017 年 10 月 1 日

今天尝试了利用 Python C API 对 Python 进行了扩展, 以及在应用程序中调用 Python. 下面是一个总结, 避免没几天就给忘了. 倘若有模糊不清之处, 应参阅下方的参考资料.

### 扩展 (Extending) Python

#### I. Hello World 程序

首先利用 Python C API 来写一个 Hello World 程序, 了解一下相关的基础知识, 首先给出可以运行的代码:

```c
// hellomodule.c 
#include "Python.h"

static PyObject* hello(PyObject *self, PyObject *args) {
    printf("Hello World! -- Using Python C API\n");
  	Py_RETURN_NONE;
}

static PyMethodDef HelloMethods[] = {
  {"hello", hello, METH_VARARGS, "A Hello World Program"},
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC inithello() {
    (void) Py_InitModule("hello", HelloMethods);
}
```

要编译的话, 最好还是使用 Makefile, 编写如下:

```bash
CC = gcc
CFLAGS = -Wall -ansi -fPIC -I/home/ieric/cpython/MyPython/include/python2.7 \
		 -L/home/ieric/cpython/MyPython/lib
CLINK = -pthread -shared

SRC = hellomodule.c
TAR = hello.so

$(TAR) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

.PHONY : clean
clean :
	rm -rf $(TAR)

.PHONY : run
run :
	python test.py
```

同时还要编写 Python 测试文件:

```python
# test.py
import hello
hello.hello()
```

编译并且运行程序:

```bash
make
make run

# 输出
Hello World! -- Using Python C API
```

​    现在来详细分析上面的代码, 首先来看 `hellomodule.c` 文件, 虽然我想创建名为 hello 的模块, 但是 c 程序的名字不一定非要叫做 `hellomodule`, 但这是约定俗成的叫法. 在该文件中, 我们首先将 `Python.h` 头文件给包含进来, 这样我们才能调用 Python 的 C API. 但是注意, 虽然 Ubuntu 系统中自带了 Python, 我使用的是通过源码编译得到的 Python, 当时我将 Python 安装在 `/home/ieric/cpython/MyPython` 目录下, 所以在 Makefile 的 CFLAGS 中要指定搜索路径为 `/home/ieric/cpython/MyPython/include`.

```c
static PyObject* hello(PyObject *self, PyObject *args) {
    printf("Hello World! -- Using Python C API\n");
  	Py_RETURN_NONE;
}
```

​    这个函数非常重要, 目前这一点并没有体现出来, 毕竟该函数中对参数 `args` 没有处理, 只是单纯的打印 hello world. 还可以编写其他的函数, 函数名不用是 hello, 只要返回值类型和参数类型都是 `PyObject*` 即可. 编写该函数后, 结合下面谈到的 `PyMethodDef HelloMethods[]`, 我们可以在 Python 代码中使用该函数. 上面的代码相当于在 Python 中定义:

```python
def hello(self, arg1, arg2, ...):
  print "Hello World...."
  return None
```

也就是说, 我们使用 `args` 来存放一个或多个参数. hello 函数就是在 C 中写一个 Python method 的简单样式.

下一段代码:

```c
static PyMethodDef HelloMethods[] = {
  {"hello", hello, METH_VARARGS, "A Hello World Program"},
  {NULL, NULL, 0, NULL}
};
```

​    注意 `HelloMethods` 是一个数组, 所以最后别忘了分号; 该数组中的元素类型都是 `PyMethodDef`, 是一个结构体. 通过这个数组, 我们可以告诉 Python, 在这个 module 中, 有那些 methods 是可以用的, 这些信息就保存在 HelloMethods 中. 而对于每一个元素:

```c
{"hello", hello, METH_VARARGS, "A Hello World Program"}
```

+ 第一项 "hello": 表示 method name, 即方法的名字
+ 第二项 hello : 表示实际执行的 C 函数, 即一开始定义的 C 函数
+ 第三项 `METH_VARARGS` : 指示该如何解读这个 method 的 flag
+ 最后一项是对这个 method 的说明 (Documentation)
+ 而 `{NULL, NULL, 0, NULL}` 则用于作为这个数组的结尾标示.

通过以上两步定义, 我们完成了 hello 方法的实际执行程序, 以及通知 Python 有关该方法的必要信息.

最后是对这个模块的初始化:

```c
PyMODINIT_FUNC inithello() {
    (void) Py_InitModule("hello", HelloMethods);
}
```

​    注意到它并不是静态函数. 请注意: **当你在 Python 中执行 `import hello`, 解释器找到 shared object file 时, 它会查找名为 `inithello` 的函数, 并且如果执行该函数后并没有成功的将 hello 模块初始化, 它便没有办法导入该模块.** 如果此时我要写一个最小的模块, 那么可以这样写:

```c
// minimal.c
PyMODINIT_FUNC initminimal() {
    (void) Py_InitModule("minimal", NULL);
}
// 也就是说, NULL 表示空数组, 说明没有任何方法的信息. 编译该文件
// 可以在 Python 中使用 import minimal 导入.
```

另外, 根据 http://starship.python.net/crew/mwh/toext/your-first-extension.html (这是一份非常好的教程) 中的介绍, `PyMODINIT_FUNC` 是有 Python 提供的预处理符号, 它包含了必要的动态链接器 (dynamic linker), 确保当系统需要 import 一个动态扩展(dynamic extension) 能成功. (The symbol `PyMODINIT_FUNC` is a preprocessor symbol provided by Python that contains the necessary dynamic linker magic your platform needs for the import of a dynamic extension to work). 另外, 作者建议扩展中所有的函数(symbol)都使用 static 关键字修饰.

#### II. Fibonacci 数列

下面看一个更为复杂的例子, 在 Python 中调用用 C 写的 Fibnacci 函数. 先给出整体的代码:

```c
// fib.c
#include "Python.h"

static long _fib(int n) {
    if (n < 2)
        return n;
    int i = 1;
    int a = 0, b = 1;
    long res;
    while (i < n) {
        res = a + b;
        a = b;
        b = res;
        ++i;
    }
    return res;
}

static PyObject* fib(PyObject *self, PyObject *args) {
    int n;
    if (!PyArg_ParseTuple(args, "i", &n))
        return NULL;
    return Py_BuildValue("i", _fib(n));
}

static PyMethodDef FibMethods[] = {
    {"fib", fib, METH_VARARGS, "Calculate Fib numbers"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initfib() {
    (void) Py_InitModule("fib", FibMethods);
}
```

使用 make 来编译:

```bash
CC = gcc
CFLAGS = -Wall -ansi -fPIC -I/home/ieric/cpython/MyPython/include/python2.7 \
		 -L/home/ieric/cpython/MyPython/lib
CLINK = -pthread -shared

SRC = fib.c
TAR = fib.so

$(TAR) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

.PHONY : clean
clean :
	rm -rf $(TAR)

.PHONY : run
run :
	python test.py
```

python 的测试文件为:

```python
# test.py
import fib 
import time

def py_fib(n):
    if n < 2:
        return n
    a, b = 0, 1
    for _ in xrange(n - 1):
        res = a + b
        a = b
        b = res
    return res

num = 10000

t = time.time()
for i in xrange(1, num):
    fib.fib(i)
print "c program: {:.5f}s".format(time.time() - t)

t = time.time()
for i in xrange(1, num):
    py_fib(i)
print "python program: {:.5f}s".format(time.time() - t)
```

编译并且运行:

```bash
make
make run

# 在笔记本上运行得到的结果如下:
c program: 0.16110s
python program: 6.43470s

# 当 num = 20000 时, 
c program: 0.63284s
python program: 41.90426s
```

可以看到使用 C 来编写程序可以大幅提高运行速度!

下面介绍每段代码的作用:

```c
static PyObject* fib(PyObject *self, PyObject *args) {
    int n;
    if (!PyArg_ParseTuple(args, "i", &n))
        return NULL;
    return Py_BuildValue("i", _fib(n));
}
```

​    这段代码在该模块中年定义了 fib 方法, 它作为和 Python 的接口, 接受参数并返回结果. 获取参数的关键在于 `PyArg_ParseTuple()` 这个函数:

`args` => 存放参数的变量;

`i`: 参数的格式, 它表示一个整数. 更多的格式可以参考: http://docs.python.org/dev/c-api/arg.html

`&n`: 表示把参数读入到变量 n 中.

那么如何读入一个字符串呢?

```c
char str[128];
PyArg_ParseTuple(args, "s", str);
```

如果第一个参数是字符串, 第二个参数是整数的话:

```c
char str[128];
int n;
PyArg_ParseTuple(args, "si", str, &n);
```

最后对于回传的结果, 我们必须返回一个 `PyObject*` 对象, 而不能直接将一个 int 返回. 所以需要先将 C 的整数转换为 `PyObject` 整数, 使用的是 `Py_BuildValue` 这个函数.

```c
return Py_BuildValue("i", _fib(n));
```



### 嵌入 (Embedding) Python

#### I. Hello World 程序

同样的, 我们先看代码然后再分别做解释, 首先是最为简单的 Hello World 程序

```c
// hello.c
#include "Python.h"

int main() {
  
  Py_Initialize();
  
  PyRun_SimpleString("print \"Hello, World - from Python C API\"");
  
  Py_Finalize();
  return 0;
}
```

然后是 Makefile:

```bash
CC = gcc
PYFLAGS = -Wall -ansi -I/home/ieric/cpython/MyPython/include/python2.7 \
		 -L/home/ieric/cpython/MyPython/lib
PYLINK = -lpython2.7

SRC = hello.c
TAR = main

$(TAR) : $(SRC)
	$(CC) $(PYFLAGS) -o $@ $^ $(PYLINK) \

.PHONY : clean
clean :
	rm -rf $(TAR)

.PHONY : run
run :
	./main
```

最后编译并且运行:

```bash
make
make run

# 输出
Hello, World - from Python C API
```

首先使用 `Py_Initialize()` 初始化 Python 动态库. 最后使用 `Py_Finalize()` 来清理 Python 环境. 在这两行代码之间, 我们使用 `PyRun_SimpleString()` 来执行 Python 代码, 使用 Python 中的 print 函数来打印输出信息.

[The Very High Level Layer](https://docs.python.org/2/c-api/veryhigh.html) 中还介绍了一些功能类似的函数, 比如 `PyRun_String`, `PyRun_File` 等, 比如 `PyRun_String` 函数的声明为:

```c
PyObject*             // PyRun_String的返回值类型
PyRun_String(         // 函数名
  const char *str,    // 执行的Python代码
  int start,          // 载入类型, 分别有: Py_eval_input, Py_file_input, Py_single_input
  PyObject *globals,  // 全局对象
  PyObject *locals    // 局部对象
)
```

其中 start 参数用来指定这段代码的导入方式, 分为三种模式, 最常用的模式是 `Py_file_input`.

#### 详解

参看博客 [Python与C之间的相互调用（Python C API及Python ctypes库）](http://blog.csdn.net/vagrxie/article/details/5251306)

+ 对于在应用程序中嵌入 Python 解释器, 即执行 Python 代码, 首先要包含 Python.h 文件, 另外在接口中, `Py_Initialize()` 和 `Py_Finalize()` 一对的调用也是必须的, 一个用于初始化 Python 动态库, 一个用于释放.

+ `PyRun_SimpleString` 可以用来执行简单的 Python 语句, 比如 `PyRun_SimpleString("print \"Hello World!\"");` 但是该函数有个缺点, 它的返回值为 int, 返回 0 表示 Success, -1 表示 an exception was raised. 这样就无法在 Python 和 C 语言中传递任何信息, 我们需要更高级的函数, 比如 `PyRun_String`:

  ```c
  PyObject* PyRun_String(const char *str, int start, PyObject *globals, PyObject *locals)
  ```

  注意该函数中像 `globals` 和 `locals` 参数不能置为空, 需要获取相应的参数:

  ```c
  #include "Python.h" 

  int  main(int  argc, char * argv[])
  {
      Py_Initialize();

      PyRun_SimpleString("x = 10" );
      PyRun_SimpleString("y = 20" );
      PyObject* mainModule = PyImport_ImportModule("__main__" );
      PyObject* dict = PyModule_GetDict(mainModule);
      PyObject* resultObject = PyRun_String("x + y" , Py_eval_input, dict, dict);

      if (resultObject)
      {
          long  result = PyLong_AsLong(resultObject);
          printf(" %d " , result);
          Py_DECREF(resultObject);
      }

      Py_Finalize();

      return  0 ;
  }
  ```

  注意 `PyRun_SimpleString` 实际是将所有的代码都放在 `__main__` 模块中运行.



#### 两个例子

下面再看两个例子: 详情参阅: http://blog.e3rp4y.me/2015/08/07/call-python-from-c-II/

```c
#include <Python.h>

int main() {
  // 这里声明了三个环境变量和一个返回值.
  PyObject *pMainModule,   // __main__模块的指针对象, 等下会初始化为__main__模块.
      *pGlobalDict,        // globals对象指针
      *pLocalDict,         // locals对象指针
      *pResult;            // 用来获取返回值

  // 这是一段Python代码          
  char *cmd = ""
    "import math\n"
    "x = 3\n"
    "y = 2\n"
    "result = math.pow(x, y)";

  // 用来接受返回值
  double result;

  Py_Initialize();

  // 初始化__main__模块
  pMainModule = PyImport_AddModule("__main__");
  // 在__main__模块中获取globals对象
  pGlobalDict = PyModule_GetDict(pMainModule);
  // 初始化一个空的locals对象
  pLocalDict = PyDict_New();

  // !! 这里是整个程序的核心, 就在这里执行了cmd的Python程序
  PyRun_String(cmd, Py_file_input, pGlobalDict, pLocalDict);

  // 获取locals对象上面的result变量
  pResult = PyDict_GetItemString(pLocalDict, "result");
  // 讲PyFloat类型转换成C语言的double类型
  result = PyFloat_AsDouble(pResult);

  // 释放引用计数
  Py_DECREF(pResult);
  Py_DECREF(pLocalDict);
  Py_DECREF(pGlobalDict);
  Py_DECREF(pMainModule);

  // 打印计算得出来的值
  printf("result=%lf\n", result);

  Py_Finalize();

  return 0;
}
```

第二个例子参阅: http://huiliu.github.io/2016/06/27/extending_embedded_python.html

```c
// file: main.c
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define CHECK_NULL_ASSERT(p) \
    if (NULL == (p)) {\
        PyErr_Print();\
        assert(0);\
    }

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: exe <python_source_file> <func_name>");
    }

    setenv("PYTHONPATH", "./", 1);
    Py_SetProgramName("main");

    Py_Initialize();
    PyObject* pModule = PyImport_ImportModule(argv[1]);
    CHECK_NULL_ASSERT(pModule);
    PyObject* pDict = PyModule_GetDict(pModule);
    CHECK_NULL_ASSERT(pDict);
    PyObject* pFunc = PyDict_GetItemString(pDict, argv[2]);
    CHECK_NULL_ASSERT(pFunc);

    if (PyCallable_Check(pFunc)) {
        PyObject_CallObject(pFunc, NULL);
    }
    else
    {
        PyErr_Print();
    }
	
    Py_DECREF(pFunc);
  	Py_DECREF(pDict);
    Py_DECREF(pModule);

    Py_Finalize();
    return 0;
}
```

编译使用:

```bash
gcc main.c -o main `pkg-config –libs –cflags python2`
```

在 python 文件中使用:

```python
# file: example.py
def echo():
    print("Hello World!")
```

执行使用:

```bash
./main example echo
```

在应用程序中嵌入 Python, 关键就是如何让应用程序与 Python 脚本交互, 即在应用程序中执行 Python 代码. 回忆在 Python 中是如何调用 Python 的标准模块的? 应用程序也采用同样的方式与 Python 交互:

       	1. 导入 Python 模块(加载脚本)
        	2. 获取模块中的函数/类/类的方法等
         	3. 传入参数, 调用 Python 函数; 访问属性等
         	4. 解析返回值

#### I. 导入 Python 模块 -- Python 库搜索路径

应用程序加载 python 脚本模块，有一个模块的搜索路径问题，python 库的默认搜索路径与 Python 的安装路径有关，可以通过下面的代码查看：

```python
import sys
print sys.path
```

另外可以通过两个环境变量：*PYTHONHOME* 和 *PYTHONPATH* 来调整。根据官方文档中的说明：

- *PYTHONHOME* 用于更改 Python 标准库的（搜索）位置，默认位置为：`prefix/lib/pythonversion` 和`exec_prefix/lib/pythonversion`。如果将 *PYTHONHOME* 设置为单个目录，则 *prefix* 和 *exec_prefix* 将取相同值。也可以设置为不同的值：`prefix:exec_prefix`
- *PYTHONPATH* 用于增加模块文件的默认搜索路径。 默认搜索路径与安装相关， 通常为：*prefox/lib/pythonversion*， 且它通常应该添加到变量 *PYTHONPATH* 中。

要调整 python 解释器的搜索路径，其实就是修改 PYTHONHOME 和 PYTHONPATH 两个环境变量。方法有：

1. 使用 setenv 来修改 PYTHONHOME 和 PYTHONPATH。如上面的代码:

   ```c
   setenv("PYTHONPATH", "./", 1);
   ```

2. 在命令行修改环境变量

3. Python API 函数: `PySys_SetPath(char*)`

   ```c
   // ......
   Py_Initialize();
   PySys_SetPath("./");
   // ......
   Py_Finalize();
   ```

#### II. 导入 Python 模块 -- 加载 Python 模块

1. `PyObject* PyImport_ImportModule (const char *name)` 相当于：`import name`。*name* 可以是内置模块名，也可以是开发的新模块名（文件名）;

2. `PyObject* PyImport_Import(PyObject *name)` 参数 *name* 应该是一个 PyObject 对象，如：

   ```c
   PyObject* pFileName = PyString_FromString(argv[1]);
   PyObject* pModule = PyImport_Import(pFileName);
   ```

#### III. 取得函数的引用

如果要执行脚本中的某个特定的函数, 首先需要取得这个函数:

```c
int PyObject_HasAttrString(PyObject* o, const char* attr_name); 
PyObject* PyObject_GetAttrString(PyObject* o, const char* attr_name); 
int PyObject_SetAttrString(PyObject* o, const char* attr_name, PyObject* v); 
int PyObject_DelAttrString(PyObject* o, const char* attr_name);
```

相当于: `o.name[=val]`

通过这种方法不仅是取得属性, 也可以是函数



#### IV. 构造参数调用函数

**C 调用 Python 函数，向 Python 函数传递参数，则需要将 C 的数据类型转换为 Python 的数据类型，然后再调用Python 函数。**

##### 将 C 类型转换为 Python 类型

1. `PyObject* Py_BuildValue(const char* format, ...);` 可以将各种C数据类型，转换为python数据类型.如：

```c
Py_BuildValue("")                               None
Py_BuildValue("i", 37)                          37
Py_BuildValue("ids", 37, 3.4, "hello")          (37, 3.4, "hello") 元组
Py_BuildValue("[ii]", 37, 3)                    [37, 3]
Py_BuildValue("{s:i,s:i}", "x", 3, "y", 2)      {"x":1, "y":2}
```

2. 各种:

```c
int PyArg_ParseTuple(PyObject *args, const char *format, ...)

int PyArg_ParseTupleAndKeywords(PyObject *args, PyObject *kw, 
                                const char *format, char *keywords[], ...)

int PyArg_Parse(PyObject *args, const char *format, ...)
```

##### 调用 Python 中的函数

1. `PyCallable_Check(PyObject*)` 检查对象是否可以被调用执行
2. `PyObject_CallObject(PyObject* callable_obj, PyObject* arg)` 传递参数*arg*调用对象*callable_obj*。等同于python中： apply(callable_object, args)。注意args是元组。
3. `PyObject* PyObject_CallFunction(PyObject* callable, char* format, ...)` 直接以C类型数据调用*callable*对象， 格式串*format*与Py_BuildValue一样。
4. `PyObject* PyObject_CallFunctionObjArgs(PyObject* callable, ..., NULL)`
5. `PyObject* PyObject_CallMethod(PyObject* o, char* method, char* format, ...)` 等同于：o.method(args)
6. `PyObject* PyObject_CallMethodObjArgs(PyObject* o, PyObject* name, ..., NULL)`

#### V. 处理返回值

处理返回值，即将 Python 的数据类型转换为C的类型

1. `long PyInt_AsLong(PyObject*)`
2. `long PyLong_AsLong(PyObject*)`
3. `double PyFloat_AsDouble(PyObject*)`
4. `char* PyString_AsString(PyObject*)`

上面的内容均是 http://huiliu.github.io/2016/06/27/extending_embedded_python.html 中的, 同时作者在最后还给出了一个扩展嵌入的 Python 解释器的例子.

### 扩展嵌入的 Python 解释器

通过 Python C API 可以将标准的 Python 解释器嵌入到应用程序中, 但是如何扩展这个解释器呢? 即在解释器中内置一些模块.

```c
// main.c
#include <Python.h>

static int numargs=0;

/* Return the number of arguments of the application command line */
static PyObject*
emb_numargs(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
  	numargs += 100;
    return Py_BuildValue("i", numargs);
}

static PyMethodDef EmbMethods[] = {
    {"numargs", emb_numargs, METH_VARARGS,
     "Return the number of arguments received by the process."},
    {NULL, NULL, 0, NULL}
};

int
main(int argc, char *argv[])
{
    Py_SetProgramName(argv[0]);  /* optional but recommended */
    Py_Initialize();
    numargs = argc;
    // The key point
    Py_InitModule("emb", EmbMethods);
    PyObject* pModule = PyImport_ImportModule(argv[1]);
    PyObject* pFunc = PyObject_GetAttrString(pModule, argv[2]);
    if (PyCallable_Check(pFunc)) {
        PyObject_CallObject(pFunc, NULL);
    }
    Py_Finalize();
    printf("%d\n", numargs);
    return 0;
}
```

注意上面代码中的 `Py_InitModule()` 放置在 main 函数中, 相当于创建了一个新的模块 emb, 然后使用 `PyImport_ImportModule` 导入该模块.

```bash
# 编译之后使用如下命令运行
# ./main 为 argv[0]
# emb 为 argv[1], 即模块名
# numargs 为 argv[2], 即方法名
./main emb numargs

# 输出为 
103

# 如果
./main emb numargs 10
# 输出为 
104
```



### 参考资料

1. [如何让 Python 使用 C 函数库(Part1)](http://0w0.logdown.com/posts/36859-python-c-api-p1)
2. [如何让 Python 使用 C 函数库(Part2)](http://0w0.logdown.com/posts/40840-python-c-api-part-2)
3. [在应用程序中嵌入 Python](http://huiliu.github.io/2016/06/27/extending_embedded_python.html)
4. [Writing Python Extensions](http://starship.python.net/crew/mwh/toext/your-first-extension.html)
5. [Python Programming/Extending with C](https://en.wikibooks.org/wiki/Python_Programming/Extending_with_C)
6. [在 C 中调用 Python 程序(I)](http://blog.e3rp4y.me/2015/08/07/call-python-from-c-I/)
7. [在 C 中调用 Python 程序(II)](http://blog.e3rp4y.me/2015/08/07/call-python-from-c-II/)
8. [官方手册](https://docs.python.org/2/c-api/intro.html)
9. [Python与C之间的相互调用（Python C API及Python ctypes库）](http://blog.csdn.net/vagrxie/article/details/5251306)




## 补充 (2017 年 10 月 2 日)

阅读 http://starship.python.net/crew/mwh/toext/less-trivial.html 这份资料

+ 在 C 中定义用于扩展 Python 的方法命名一般为:

  ```c
  static PyObject *
  <MODNAME>_<FUNCNAME>(PyObject *self, PyObject *args)
  {
  	...
  }
  ```

  对于 Module-level 的函数, 比如 `import math; math.sum(...)` 中的 `sum()`, self 这个参数总是 NULL. 而 args 这个参数将包含传入函数的参数的 tuple.

  注意上面的 c 程序只是定义了函数, 而要让 Python 能访问这个函数, 还需要定义一个数组, 用于将扩展模块中的函数展示给 Python. 这个数组中的每一项都是一个结构体, 类型为 `PyMethodDef`, 拥有 4 个 fields:

  ```bash
  ml_name : The name of the function as seen from Python
  ml_meth : a pointer to the C implementation of the function
  ml_flags: A flag indicating the calling convention the function uses. 
            METH_VARARGS is the usual choice here;
  ml_doc : The function's docstring or NULL if want to be evil and have a 
            function with no docstring.
  ```

  该数组最后一个元素是 `{NULL, NULL, 0, NULL}` 用于表示数组的结束.

+ 大多数函数首要的事情是对参数进行匹配, 包括数据和类型. 由于 `<MODULE>_<FUNC>` 函数接受任意类型的参数, 因此可以使用 `PyArg_UnpackTuple` 对参数进行处理. 它参数如下:

  1. `args`: An argument tuple. **A `METH_VARARGS` function knows its args parameter is a tuple, so there is no need to check**.
  2. `name`: the name of the function, which can appear in error messages.
  3. `min`: the minimum number of arguments the function accepts.
  4. `max`: the maximum number of arguments the function accepts.
  5. `...`: max pointers to variables of type `PyObject *`.

  `PyArg_UnpackTuple` 成功的话返回一个 true value, 否则返回 0;

+ **Almost all Python objects are allocated on the heap.** 几乎所有的 Python 对象都在堆上分配. 因此不可以声明一个 PyObject 类型的对象(局部变量保存在 stack), 只能是 `PyObject*`. They can be referred to as pointers to a `PyObject`. A `PyObject` just contains a reference count and a pointer to a type object. So when a function receives arguments of arbitrary type, it will be dealing with pointers to `PyObject`. 


#### 3.1 Other Calling Conventions

对于 PyMethodDef 结构体中的 `ml_flags`, 除了可以使用 `METH_VARARGS` (default), 还有其他的选择, 比如说可以传入 keywords. 下面是另外的 `ml_flags`:

```bash
METH_OLDARGS: Obsolete; do not use this. (已经弃用了, 不要使用这个)
METH_KEYWORDS: Means the function accepts keyword arguments (接受 keywords)
METH_O: Means the function takes a single argument (只接受一个参数)
METH_NOARGS: Means the function takes no arguments (不接受任何参数)
```

上述 flags 如果和 `METH_STATIC` 以及 `METH_CLASS` 结合使用, 那么可以实现 static 或 class methods.



## Reference Count (2017 年 10 月 3 日)

- [Reference Counting in Python](https://docs.python.org/2/extending/extending.html#reference-counting-in-python)

The big question now remains: when to use `Py_INCREF(x)` and `Py_DECREF(x)`? Let's first introduce some terms.

**Nobody "owns" an object; however, you can *own a reference* to an object.** An object's reference count is now defined as the number of owned references to it. The owner of a reference is responsible for calling `Py_DECREF()` when the reference is no longer needed. Ownership of a reference can be transferred. There are three ways to dispose an owned reference: pass it on, store it, or call `Py_DECREF()`. Forgetting to dispose of an owned reference creates a memory leak.

It is also possible to *borrow* (The metaphor of “borrowing” a reference is not completely correct: the owner still has a copy of the reference.) a reference to an object. The borrower of a reference should not call `Py_DECREF()`. The borrower must not hold on to the object longer than the owner from which it was borrowed. Using a borrowed reference after the owner has disposed of it risks using freed memory and should be avoided completely. (Checking that the reference count is at least 1 **does not work** — the reference count itself could be in freed memory and may thus be reused for another object!)

A borrowed reference can be changed into an owned reference by calling [`Py_INCREF()`](https://docs.python.org/2/c-api/refcounting.html#c.Py_INCREF). 







































