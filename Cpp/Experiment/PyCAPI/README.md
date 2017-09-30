# Python C API

## 2017 年 9 月 30 日

### 利用 Python C API 编写 Hello World

+ 利用 Python 源码

  ```bash
  cd ~
  git clone https://github.com/python/cpython
  cd cpython
  git checkout v2.7.13
  mkdir MyPython
  ./configure --prefix=/home/ieric/cpython/MyPython --enable-shared
  make
  make install
  ```

  + 参考 http://0w0.logdown.com/posts/36859-python-c-api-p1 写一个简单的 Hello World 程序

  ```c
  // main.c
  #include <Python.h>

  static PyObject* hello(PyObject* self, PyObject* args) {
      printf("Hello! A C function called!\n");
      Py_RETURN_NONE;
  }

  static PyMethodDef HelloMethods[] = {
      {"hello", hello, METH_VARARGS, "A hello function."},
      {NULL, NULL, 0, NULL}
  };

  PyMODINIT_FUNC inithello(void) {
       (void) Py_InitModule("hello", HelloMethods);
  }
  ```

  + **重点**: 编译

  ```bash
  cd ~
  # 在当前目录下生成 main.o 文件 
  # 关键是需要 -fPIC 选项, 否则在链接时会出现
  # relocation R_X86_64_32 against `.rodata' can not be used when 
  # making a shared object; recompile with -fPIC
  ## 另外注意一点就是生成的 .so 文件名字必须是 hello.so, 不能改为 main.so 或别的什么的
  gcc -pthread -fPIC -I./cpython/Include -I./cpython -c main.c
  gcc -pthread -shared main.o -o hello.so


  # 或者更简单的
  gcc -pthread -fPIC -shared -I./include -o hello.so main.c
  python -c "import hello"
  ```

  + 之后创建 main.py 文件

  ```python
  // main.py
  import hello
  hello.hello()
  ```

  运行得到

  ```bash
  Hello! A C function called!
  ```

+ 直接编写 C 程序而不是在 Python 中调用 

  下面 Hello World 来自 http://blog.e3rp4y.me/2015/08/07/call-python-from-c-I/


```c
#include "python2.7/Python.h"

int main() {
  // 初始化Python环境
  Py_Initialize();
  
  // 执行Python代码, 调用Python的print
  PyRun_SimpleString("print \"Hello, C! -- from Python\"");
  
  // 清理Python环境
  Py_Finalize();
  return 0;
}
```

+ 编译

  ```bash
  cd ~/cpython/MyPython
  ls
  # 结果是
  # bin include lib share
  # 在当前目录下创建 test.c 并将上述程序拷贝到该文件中
  gcc -I./include -L./lib -o test test.c -lpython2.7
  ```

+ 注意要编译成功前面编译 Python 源码时必须使用 

  ```bash
  ./configure --enable-shared

  # 另外注意查询需要哪些动态库可以使用
  # python2.7-config --libs
  ```

  ​

### 推荐阅读

+ [用C语言扩展Python的功能](https://www.ibm.com/developerworks/cn/linux/l-pythc/index.html)
















