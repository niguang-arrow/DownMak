# Python C API

## 2017 年 9 月 30 日

### 利用 Python C API 编写 Hello World

+ 利用 Python 源码

  ```bash
  cd ~
  git clone https://github.com/python/cpython
  cd cpython
  git checkout v2.7.13
  ./configure
  make
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
  gcc -pthread -fPIC -I./cpython/Include -I./cpython -c main.c
  gcc -pthread -shared main.o -L./cpython/ -L./cpython/Python -o hello.so
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

  ​



















