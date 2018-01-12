# TH.h

## 2018 年 1 月 12 日

该文件位于 `pytorch/torch/lib/TH/` 目录下.

所有的内容为:

```c
#ifndef TH_INC
#define TH_INC

#include "THGeneral.h"

#include "THBlas.h"
#ifdef USE_LAPACK
#include "THLapack.h"
#endif

#include "THAtomic.h"
#include "THVector.h"
#include "THLogAdd.h"
#include "THRandom.h"
#include "THStorage.h"
#include "THTensor.h"
#include "THTensorApply.h"
#include "THTensorDimApply.h"

#include "THFile.h"
#include "THDiskFile.h"
#include "THMemoryFile.h"

#endif
```

而其中的, 比如 `THTensor.h`, 内容如下:

```c
#ifndef TH_TENSOR_INC
#define TH_TENSOR_INC

#include "THStorage.h"
#include "THTensorApply.h"

#define THTensor          TH_CONCAT_3(TH,Real,Tensor)
#define THTensor_(NAME)   TH_CONCAT_4(TH,Real,Tensor_,NAME)

#define TH_DESC_BUFF_LEN 64
typedef struct {
    char str[TH_DESC_BUFF_LEN];
} THDescBuff;

/* basics */
#include "generic/THTensor.h"
#include "THGenerateAllTypes.h"

#include "generic/THTensorCopy.h"
#include "THGenerateAllTypes.h"

#include "THTensorMacros.h"

/* random numbers */
#include "THRandom.h"
#include "generic/THTensorRandom.h"
#include "THGenerateAllTypes.h"

/* maths */
#include "generic/THTensorMath.h"
#include "THGenerateAllTypes.h"

/* convolutions */
#include "generic/THTensorConv.h"
#include "THGenerateAllTypes.h"

/* lapack support */
#include "generic/THTensorLapack.h"
#include "THGenerateFloatTypes.h"

#endif
```

在 `THTensor.c` 中的内容如下:

```c
#include "THAtomic.h"
#include "THTensor.h"
#include "THVector.h"
#include "THBlas.h"
#include "THLapack.h"
#include "THRandom.h"
#include "THTensorDimApply.h"
#include "THMath.h"

#include "generic/THTensor.c"
#include "THGenerateAllTypes.h"

#include "generic/THTensorCopy.c"
#include "THGenerateAllTypes.h"

#include "generic/THTensorRandom.c"
#include "THGenerateAllTypes.h"

#include "generic/THTensorMath.c"
#include "THGenerateAllTypes.h"

#include "generic/THTensorConv.c"
#include "THGenerateAllTypes.h"

#include "generic/THTensorLapack.c"
#include "THGenerateFloatTypes.h"
```



也就是说, `TH/` 目录下的 `THTensor.h` 文件处理 `TH/generic/` 目录下的 `THTensor.h` 文件以及其他和 Tensor 有关的头文件, 而 `TH/` 目录下的 `THTensor.c` 文件处理 `TH/generic/` 目录下的相关 `.c` 文件, 最终, 在 `TH/` 目录下, 所有的 `.h` 和 `.c` 文件中的函数名都被正确的展开. 函数声明和函数定义都一一对应. 在 `TH/TH.h` 文件中将所有的函数声明集中在一起. 

编译之后, `csrc/generic/` 目录下的 `Tensor.cpp` 等函数使用, 比如我在 `pytorch/torch/csrc/generic/Tensor.cpp` 发现:

```c
//extern PyObject * THPTensor_(copy)(THPTensor *self, PyObject *other);
int THPTensor_(setValue)(THPTensor *self, PyObject *index, PyObject *value)
{
    // ......
      THTensor_(maskedFill)(LIBRARY_STATE self->cdata, mask->cdata, v);
    } else if (THPTensor_(IsSubclass)(value)) {
      THTensor_(maskedCopy)(LIBRARY_STATE self->cdata, mask->cdata, ((THPTensor*)value)->cdata);
    } else {
    //.......
      
        if (!THPUtils_(parseReal)(value, &v))
          return -1;
        THTensor_(fill)(LIBRARY_STATE tresult, v);
      } else {
	// .......        
}
```

可以看到, `THPTensor_(setValue)` 函数中, 调用了很多在 `lib/TH` 目录中定义的函数, 比如 `THTensor_(fill)` 等. 

之后, 在 `csrc/` 目录下的 `Module.cpp` 文件中(这个文件是 Python 和 C 交互的文件, 具体可以看 setup.py 中关于 `torch._C` 的生成部分). 注意, 在 `Module.cpp` 中:

```c
static PyMethodDef TorchMethods[] = {
  
  // ......
  {"sigmoid",         (PyCFunction)THPModule_sigmoid,           METH_VARARGS, NULL},
  {"log",             (PyCFunction)THPModule_log,               METH_VARARGS, NULL},
  {"log1p",           (PyCFunction)THPModule_log1p,             METH_VARARGS, NULL},
  {"exp",             (PyCFunction)THPModule_exp,               METH_VARARGS, NULL},
  {"cos",             (PyCFunction)THPModule_cos,               METH_VARARGS, NULL},
  {"acos",            (PyCFunction)THPModule_acos,              METH_VARARGS, NULL},
  {"cosh",            (PyCFunction)THPModule_cosh,              METH_VARARGS, NULL},

  // .....
  
};  
```

也就是 `torch.cos()` 之类的方法底层是调用: `THPModule_cos` 方法, 而这个方法在 `Module.cpp` 中是通过一个名为 `IMPLEMENT_STATELESS` 的宏定义的:

```c
#define IMPLEMENV_STATELESS(name)                                              \
static PyObject * TH_CONCAT_2(THPModule_, name)(PyObject *_unused, PyObject *args) \
{                                                                              \
  PyObject *tensor = THPDefaultTensorClass;                                    \
  for (int i = 0; i < PyTuple_Size(args); i++) {                               \
    PyObject *item = PyTuple_GET_ITEM(args, i);                                \
    if (THPModule_isTensor(item)) {                                            \
      tensor = item;                                                           \
      break;                                                                   \
    }                                                                          \
  }                                                                            \
                                                                               \
  PyObject *methods = PyObject_GetAttrString(tensor, STATELESS_ATTRIBUTE_NAME);     \
  THPUtils_assert(methods, "Type %s doesn't implement statless methods",       \
      Py_TYPE(tensor)->tp_name);                                               \
  PyObject *method = PyObject_GetAttrString(methods, #name);                   \
  THPUtils_assert(method, "Type %s doesn't implement stateless method " #name, \
      Py_TYPE(tensor)->tp_name);                                               \
  return PyObject_Call(method, args, NULL);                                    \
}
```

之后使用 `IMPLEMENT_STATELESS(cos)` 定义 `THPModule_cos` 方法.

但是, cos 方法在 `csrc/generic/TensorMethods.cpp` 文件中定义的:

```c
PyObject * THPTensor_(cos)(PyObject *self, PyObject *args)
{
    // .....
      // 调用 lib/TH/ 中定义的 THTensor_(cos) 函数
      THTensor_(cos)(LIBRARY_STATE ((THPTensor*)result)->cdata, ((THPTensor*)self)->cdata);Py_INCREF(result);
      return (PyObject*)(result);
    
    // .....
}

PyObject * THPTensor_(cos_)(PyObject *self, PyObject *args)
{

	// .....      
      THTensor_(cos)(LIBRARY_STATE ((THPTensor*)self)->cdata, ((THPTensor*)self)->cdata);
   // ....
}

```

那么如何从 `THPTensor_(cos)` 转换到 `THPModule_cos` 估计是在 `IMPLEMENT_STATELESS` 这个宏中完成的. 具体细节等以后有能力再来看.

另外, 从 pytorch 的文档也可以看出, torch 模块中本身定义了许多对 Tensor 的操作方法, 而 torch.Tensor 也定义了同样的方法. 但是有趣的是, torch.Tensor 中还定义了这些方法的 inplace version, 比如的 cos 方法, 在 `TensorMethods.cpp` 中还定义了 `THPTensor_(cos_)`, 但是在 `Module.cpp` 中只定义了 `THPModule_cos`.

注意, 在 `TensorMethods.cpp` 文件的最后, 有如下内容:

```c
static PyMethodDef THPTensor_(methods)[] = {
  {"_write_metadata", (PyCFunction)THPTensor_(writeMetadata), METH_VARARGS, NULL},
  {"_new_with_metadata_file", (PyCFunction)THPTensor_(newWithMetadataFile), METH_VARARGS | METH_STATIC, NULL},
#if defined(NUMPY_TYPE_ENUM)
  {"numpy", (PyCFunction)THPTensor_(toNumpy), METH_VARARGS, NULL},
#endif
#if IS_CUDA
  {"getDevice", (PyCFunction)THPTensor_(getDevice), METH_VARARGS, NULL},
#endif
  {"elementSize", (PyCFunction)THPTensor_(elementSize), METH_VARARGS, NULL},
  {"storage", (PyCFunction)THPTensor_(storage), METH_VARARGS, NULL},
  {"storageOffset", (PyCFunction)THPTensor_(storageOffset), METH_VARARGS, NULL},
  {"nDimension", (PyCFunction)THPTensor_(nDimension), METH_VARARGS, NULL},
  {"dim", (PyCFunction)THPTensor_(nDimension), METH_VARARGS, NULL},
  {"free", (PyCFunction)THPTensor_(free), METH_VARARGS, NULL},
  {"retain", (PyCFunction)THPTensor_(retain), METH_VARARGS, NULL},
  {"resize_", (PyCFunction)THPTensor_(resize_), METH_VARARGS, NULL},
  {"zeros_", (PyCFunction)THPTensor_(zeros_), METH_VARARGS, NULL},
  {"ones_", (PyCFunction)THPTensor_(ones_), METH_VARARGS, NULL},
  {"numel", (PyCFunction)THPTensor_(numel), METH_VARARGS, NULL},

  //// ......
  
};
```

至于 `TensorMethods.cpp` 中定义的这些方法怎么和 `Module.cpp` 中的方法进行交互的, 目前能力所限, 就不考虑了.

最后, 编译好之后, 在 `torch/` 目录下会产生 `_C.so` 文件, 那么下一步我们就要考虑 `torch/__init__.py` 文件中的内容, 该文件展示了如何在 Python 中使用 C 中定义的方法.