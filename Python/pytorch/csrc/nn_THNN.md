# THNN.cpp

## 2018 年 1 月 3 日

这个文件位于 `pytorch/torch/csrc/nn/` 目录下, 该目录下还有一些其他的文件:

```bash
# tree -L 1
.
├── THCUNN.cpp
├── THCUNN.cwrap
├── THNN.cpp
└── THNN.cwrap

0 directories, 4 files

```

先来看下 THNN.cpp 文件中的内容;



## THNN.cpp

这个文件的内容超级多, 根据 pytorch 官方博客 http://pytorch.org/2017/05/11/Internals.html 的介绍, 这个文件应该是由 `THNN.cwrap` 生成的. 

把这个文件拉到 172 行左右, 可以看到如下代码:

```cpp
#include <TH/TH.h>


extern "C" void THNN_FloatAbs_updateOutput(void*, THFloatTensor*, THFloatTensor*);

PyObject * FloatAbs_updateOutput(PyObject *_unused, PyObject *args)
{
  int __argcount = args ? PyTuple_Size(args) : 0;
  try {
      
    if (__argcount == 3 &&
          __checkLong(PyTuple_GET_ITEM(args, 0)) &&
          (PyObject*)Py_TYPE(PyTuple_GET_ITEM(args, 1)) == THPFloatTensorClass &&
          (PyObject*)Py_TYPE(PyTuple_GET_ITEM(args, 2)) == THPFloatTensorClass) {

      THNN_FloatAbs_updateOutput((void*)__getLong(PyTuple_GET_ITEM(args, 0)), (THFloatTensor*)(((Tensor*)PyTuple_GET_ITEM(args, 1))->cdata), (THFloatTensor*)(((Tensor*)PyTuple_GET_ITEM(args, 2))->cdata));
      Py_RETURN_NONE;
    
    } else {
      __invalidArgs(args, "");
      return NULL;
    }
  } catch (std::exception &e) {
      PyErr_SetString(PyExc_RuntimeError, e.what());
      return NULL;
  }
}
    
/* ....... */
```

哈, 这不就是对 Python 进行扩展的代码吗?

在 Python 中, 当导入这个模块时, 我们就能够调用 `FloatAbs_updateOutput` 方法了. 另外, 可以在代码中看到, 首先声明了 `THNN_FloatAbs_updateOutput` 函数, 这个函数不就是我们在 `pytorch/torch/lib/THNN/THNN.h` 中声明的函数吗? 

+ **也就是说, `csrc/` 目录下的文件是 Python 的 C 扩展文件, 用于和 Python 进行交互; 但是底层的 C 代码全都保存在 `lib/THNN/` 目录下, 和 Python 没有一点关系. `csrc/THNN.cpp` 会调用 `lib/THNN/` 目录下定义的 C 函数.**

第二点要注意的是, 由于 `csrc/THNN.cpp` 是由 `THNN.cwrap` 生成了, 那么肯定有一些定式(模板), (也就是代码中很多内容都是相同的), 再看下一个 `DoubleAbs_updateOutput`, 可以发现里面很多代码是一样的:

```cpp
    } else {
      __invalidArgs(args, "");
      return NULL;
    }
  } catch (std::exception &e) {
      PyErr_SetString(PyExc_RuntimeError, e.what());
      return NULL;
  }
```

简要看一下 `THNN.cwrap` 文件中的内容.



## THNN.cwrap

如下, 它就是设置了某种格式, 将 `lib/THNN/` 目录下所有的 C 函数都声明出来, 生成 `THNN.cpp` 用于扩展 Python.

```bash
#include <TH/TH.h>


extern "C" void THNN_FloatAbs_updateOutput(void*, THFloatTensor*, THFloatTensor*);
[[
  name: FloatAbs_updateOutput
  return: void
  cname: THNN_FloatAbs_updateOutput
  arguments:
    - void* state
    - THFloatTensor* input
    - THFloatTensor* output
]]


extern "C" void THNN_DoubleAbs_updateOutput(void*, THDoubleTensor*, THDoubleTensor*);
[[
  name: DoubleAbs_updateOutput
  return: void
  cname: THNN_DoubleAbs_updateOutput
  arguments:
    - void* state
    - THDoubleTensor* input
    - THDoubleTensor* output
]]

# ....... #
```

下面再返回到 `THNN.cpp` 文件.

## module_methods

当你将 `THNN.cpp` 文件拉到约 9131 行时, 你就会看到令人震惊的大数组:

```cpp
static PyMethodDef module_methods[] = {
  {"FloatAbs_updateOutput", (PyCFunction)FloatAbs_updateOutput, METH_VARARGS, NULL},
  {"DoubleAbs_updateOutput", (PyCFunction)DoubleAbs_updateOutput, METH_VARARGS, NULL},
  {"FloatAbs_updateGradInput", (PyCFunction)FloatAbs_updateGradInput, METH_VARARGS, NULL},
  {"DoubleAbs_updateGradInput", (PyCFunction)DoubleAbs_updateGradInput, METH_VARARGS, NULL},
  // ........
  {NULL, NULL, 0, NULL}
};
```

这是扩展 Python 必要的一步, 指定模块中定义的各种方法.

最后(我先只考虑 Python2), 初始化模块:

```cpp
#if PY_MAJOR_VERSION != 2
static struct PyModuleDef module_def = {
   PyModuleDef_HEAD_INIT,
   "torch._thnn._THNN",
   NULL,
   -1,
   module_methods
};
#endif

#if PY_MAJOR_VERSION == 2
PyMODINIT_FUNC init_THNN()
#else
PyMODINIT_FUNC PyInit__THNN()
#endif
{
#if PY_MAJOR_VERSION == 2
#define ASSERT_TRUE(cmd) if (!(cmd)) {PyErr_SetString(PyExc_ImportError, "initialization error"); return;}
#else
#define ASSERT_TRUE(cmd) if (!(cmd)) return NULL
#endif
  PyObject *module;

#if PY_MAJOR_VERSION == 2
  ASSERT_TRUE(module = Py_InitModule("torch._thnn._THNN", module_methods));
#else
  ASSERT_TRUE(module = PyModule_Create(&module_def));
#endif

  ASSERT_TRUE(__loadClasses());

#if PY_MAJOR_VERSION != 2
  return module;
#endif

#undef ASSERT_TRUE
}
```

我把不属于 Python2 的部分给摘除(包括异常处理的部分), 成了下面这样:

```cpp
PyMODINIT_FUNC init_THNN()
{
  PyObject *module;
  module = Py_InitModule("torch._thnn._THNN", module_methods);
  __loadClasses();
}
```

当该文件被编译之后, 我们就能在 `pytorch/torch/_thnn/` 目录下找到 `_THNN.so` 文件, 这样就可以在 Python 中使用 import 机制导入该模块了. (CUDA 部分的模块也在该目录下, 名为 `_THCUNN.so`)



## 总结

在 `pytorch/torch/_thnn/` 目录下有了 `_THNN.so` 模块, 我们就能在 Python 中使用 pytorch 定义的神经网络库了(THNN). 

+ 之后应该分析一下 `_thnn` 目录下的文件. 因为, 我们在 Python 中不是直接使用 `_THNN.so` 中提供的 Python 接口, pytorch 还提供了一层更为简洁方便的 Python 接口, 需要看看他们是如何封装好的.

