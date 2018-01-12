# `__init__.py`

## 2018 年 1 月 12 日

导入 `torch` 这个包, 主要是导入 `__init__.py` 文件中的内容, 有了前面代码的阅读经验, 现在就来看看我们常用的 `torch` 到底给我们提供了什么功能, 并以一种怎样的方式提供的.

先看看 `__init__.py` 文件中 import 的内容:

```python
from torch._C import *
import sys
import math

_tensor_classes = set()
_storage_classes = set()
```

前面在 `lib/TH/TH.md` 文档中已经介绍了 `torch._C` 中提供的功能是用 `csrc/` 目录下的 `Module.cpp` 等文件编译得到的, 其中 `Module.cpp` 给出了这些功能的汇总.

下面可以先打印出 `_tensor_classes` 和 `_storage_classes` 中的内容, 它们初始为空的 Set, 但之后面会往里头添加对象:

```python
# torch._storage_classes
{torch.LongStorage,
 torch.IntStorage,
 torch.ShortStorage,
 torch.CharStorage,
 torch.ByteStorage,
 torch.FloatStorage,
 torch.DoubleStorage}
# torch._tensor_classes
{torch.DoubleTensor,
 torch.FloatTensor,
 torch.LongTensor,
 torch.IntTensor,
 torch.ShortTensor,
 torch.CharTensor,
 torch.ByteTensor}
```

下面介绍 `torch._C` 中的内容.

## torch._C

首先使用如下命令, 观看 `torch._C` 中定义的对象, 从结果可以看出, 它们和 `csrc/Module.cpp` 文件中定义的 `static PyMethodDef TorchMethods[]` 数组中的结构体相同.

```python
# ipython
import  torch._C
dir(torch._C)

# 结果
['ByteStorageBase',                          
 'ByteTensorBase',                                                                           
 'CharStorageBase',
 'CharTensorBase',                                                                            'DoubleStorageBase',                        
 'DoubleTensorBase',                                                                          'FloatStorageBase',   
 'FloatTensorBase',                                                                           'Generator',
 'IntStorageBase',                                                                           
 'IntTensorBase', 
 'LongStorageBase',                                                                          
 'LongTensorBase', 
 'ShortStorageBase',                                                                         
 'ShortTensorBase',
 '__doc__',                                                                                  
 '__file__',      
 '__name__',                                                                                 
 '__package__',   
 '_initExtension',                                                                           
 '_storageCopy',
 '_tensorCopy',                                                                              
 'abs',        
 'acos',                                                                                     
 'add',
 # .....
  
]
```

下面是 `Module.cpp` 中定义的内容:

```c
static PyMethodDef TorchMethods[] = {
  {"_initExtension",  (PyCFunction)THPModule_initExtension,     METH_NOARGS,  NULL},
  // .....
  {"_tensorCopy",     (PyCFunction)THPModule_tensorCopyWrapper, METH_VARARGS, NULL},
  {"_storageCopy",    (PyCFunction)THPModule_storageCopyWrapper, METH_VARARGS, NULL},
  {"getNumThreads",   (PyCFunction)THPModule_getNumThreads,     METH_NOARGS,  NULL},
  {"setNumThreads",   (PyCFunction)THPModule_setNumThreads,     METH_O,       NULL},
  {"getRNGState",     (PyCFunction)THPModule_getRNGState,       METH_VARARGS, NULL},
  {"setRNGState",     (PyCFunction)THPModule_setRNGState,       METH_VARARGS, NULL},
  {"manualSeed",      (PyCFunction)THPModule_manualSeed,        METH_VARARGS, NULL},
  {"sigmoid",         (PyCFunction)THPModule_sigmoid,           METH_VARARGS, NULL},
  // .....
};
```

但如果使用如下命令, 则会得到不同的结果:

```python
# ipython
obj = __import__('torch._C')
obj
<module 'torch' from 'torch/__init__.pyc'>
dir(obj)
['ByteStorage',
 'ByteTensor',                                                                               
 'CharStorage',
 'CharTensor',
 'DoubleStorage',
 'DoubleTensor',
 'FloatStorage',
 'FloatTensor',
 'Generator',
 'IntStorage',
 'IntTensor',
 'LongStorage',
 'LongTensor',
 'ShortStorage',
 'ShortTensor',
 'Storage',
 'Tensor',
 'TensorPrinting',
 '_C',
 '_StorageBase',
 '_TensorBase',
 '__builtins__',
 '__doc__',
 '__file__',
 '__name__',
 '__package__',
 '__path__',
 '_defaultTensorTypeName',
 '_import_dotted_name',
 '_pyrange',
 '_storage_classes',
 '_tensor_classes',
 'abs',
 'acos',
 'add',
 # ....
] 
```

这些内容是 `__init__.py` 文件执行过后得到的结果, 或者说, 它就相当于 `import torch` 然后 `dir(torch)`. 因为

>   __import__(...)
>
>       __import__(name, globals={}, locals={}, fromlist=[], level=-1) -> module
>
>   When importing a module from a package, note that __import__('A.B', ...) returns package A when fromlist is empty, but its submodule B when
>   fromlist is not empty.



## Tensor.py

之后 `__init__.py` 定义了一些函数, 但是我们先需要看看和 `_C.so` 同一目录的 Tensor.py 文件中的内容, 因为:

```python
from .Storage import _StorageBase
from .Tensor import _TensorBase
```

在 `Tensor.py` 中主要是定义了 `_TensorBase` 类:

```python
class _TensorBase(object):
    def new(self, *args, **kwargs):
        return self.__class__(*args, **kwargs)

    def type(self, t=None):
        if isinstance(t, str) or t is None:
            current = self.__module__ + '.' + self.__class__.__name__
            if t is None:
                return current
            if t == current:
                return self
            _, _, typename = t.partition('.')
            return torch._import_dotted_name(t)(self.size()).copy_(self)
        else:
            if t == type(self):
                return self
            return t(self.size()).copy_(self)

    def typeAs(self, t):
        return self.type(t.type())

    def double(self):
        return self.type(torch.DoubleTensor)

    def float(self):
        return self.type(torch.FloatTensor)

    ## ..... ##
    
    def __add__(self, other):
        return self.add(other)
    __radd__ = __add__

    def __iadd__(self, other):
        return self.add_(other)

    def __sub__(self, other):
        return self.sub(other)
    
	## ..... ##
```

这个类之后会在 `__init__.py` 中用到. 我们从这个类最后面的几个函数中可以发现, 比如 `__add__` 方法, 它调用了 `self.add()` 方法, 可是 `_TensorBase` 类继承于 object, 它并没有定义自己的 `add` 方法, 那这是为何... 要知道原因还得回到 `__init__.py` 文件, 有如下定义:

```python
################################################################################
# Define Storage and Tensor classes
################################################################################

class DoubleStorage(_C.DoubleStorageBase, _StorageBase):
    pass
class FloatStorage(_C.FloatStorageBase, _StorageBase):
    pass
class LongStorage(_C.LongStorageBase, _StorageBase):
    pass
class IntStorage(_C.IntStorageBase, _StorageBase):
    pass
class ShortStorage(_C.ShortStorageBase, _StorageBase):
    pass
class CharStorage(_C.CharStorageBase, _StorageBase):
    pass
class ByteStorage(_C.ByteStorageBase, _StorageBase):
    pass

class DoubleTensor(_C.DoubleTensorBase, _TensorBase):
    pass
class FloatTensor(_C.FloatTensorBase, _TensorBase):
    pass
class LongTensor(_C.LongTensorBase, _TensorBase):
    pass
class IntTensor(_C.IntTensorBase, _TensorBase):
    pass
class ShortTensor(_C.ShortTensorBase, _TensorBase):
    pass
class CharTensor(_C.CharTensorBase, _TensorBase):
    pass
class ByteTensor(_C.ByteTensorBase, _TensorBase):
    pass
```

现在可以料想和 `_C.so` 同一目录的 `Storage.py` 文件中必然定义了一个 `_StorageBase` 的类.

现在看 `DoubleTensor`, 它分别继承于 `_C.DoubleTensorBase` 和 `_TensorBase`, 如果我们之后只使用 `DoubleTensor`, 那么就能正常调用 `_TensorBase` 中调用的 `__add__` 方法了, 因为 `self.add()` 方法必然在 `_C.DoubleTensorBase` 中有定义:

```python
dir(torch._C.DoubleTensorBase)
[
  #.......
 '_cdata',
 '_new_with_metadata_file',
 '_write_metadata',
 'abs',
 'abs_',
 'acos',
 'acos_',
 'add',
 'add_',
 'addbmm',
 'addbmm_',
 'addcdiv',
 'addcdiv_',
 # ......    
]
```

另外, 我们还可以看看 `csrc/generic/TensorMethods.cpp` 文件, 看看文件末尾的 `THPTensor_(methods)` 数组:

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
  {"nElement", (PyCFunction)THPTensor_(numel), METH_VARARGS, NULL},
  {"set_", (PyCFunction)THPTensor_(set_), METH_VARARGS, NULL},
  {"select", (PyCFunction)THPTensor_(select), METH_VARARGS, NULL},
// ......
#if defined(TH_REAL_IS_FLOAT) || defined(TH_REAL_IS_DOUBLE) || defined(TH_REAL_IS_LONG) || defined(TH_REAL_IS_INT) || CUDA_FLOAT
  {"abs", (PyCFunction)THPTensor_(abs), METH_VARARGS, NULL},
#endif
#if defined(TH_REAL_IS_FLOAT) || defined(TH_REAL_IS_DOUBLE) || defined(TH_REAL_IS_LONG) || defined(TH_REAL_IS_INT) || CUDA_FLOAT
  {"abs_", (PyCFunction)THPTensor_(abs_), METH_VARARGS, NULL},
#endif
#if defined(TH_REAL_IS_FLOAT) || defined(TH_REAL_IS_DOUBLE) || CUDA_FLOAT
  {"sigmoid_", (PyCFunction)THPTensor_(sigmoid_), METH_VARARGS, NULL},
#endif
#if defined(TH_REAL_IS_FLOAT) || defined(TH_REAL_IS_DOUBLE) || CUDA_FLOAT
  {"sigmoid", (PyCFunction)THPTensor_(sigmoid), METH_VARARGS, NULL},
#endif
// .....
};
```

这就说明了, `torch.DoubleTensor` 或者 `torch.FloatTensor` 中的方法就是在 `csrc/generic/TensorMethods.cpp` 中定义的.

另外, 我们一般会直接使用 `torch.Tensor`, `torch` 有 Tensor 这个类是由于 `__init__.py` 定义的 `setDefaultTensorType` 函数.



## setDefaultTensorType

这里可以把 `__init__.py` 中定义的函数给列出来:

```python
################################################################################
# Define basic utilities
################################################################################

def _import_dotted_name(name):
    components = name.split('.')
    obj = __import__(components[0])
    for component in components[1:]:
        obj = getattr(obj, component)
    return obj

# range gets shadowed by torch.range
def _pyrange(*args, **kwargs):
    return __builtins__['range'](*args, **kwargs)

def typename(o):
    return o.__module__ + "." + o.__class__.__name__

def isTensor(obj):
    return obj.__class__ in _tensor_classes

def isStorage(obj):
    return obj.__class__ in _storage_classes

def isLongStorage(obj):
    return isinstance(obj, LongStorage)

def setDefaultTensorType(t):
    global Tensor
    global Storage
    global _defaultTensorTypeName
    _defaultTensorTypeName = t
    Tensor = _import_dotted_name(t)
    Storage = _import_dotted_name(t.replace('Tensor', 'Storage'))

def getDefaultTensorType():
    return _defaultTensorTypeName

# This shadows Torch.py and Storage.py
setDefaultTensorType('torch.DoubleTensor')
```

默认的 Tensor 类型为 `torch.DoubleTensor`. 而在 `setDefaultTensorType` 函数中, 设置了 `Tensor`, `Storage` 以及 `_defaultTensorTypeName` 为全局变量, 所以我们能直接访问 `torch.Tensor` 以及 `torch.Storage`, 直接访问它们得到的结果就是默认的 `torch.DoubleTensor` 以及 `torch.DoubleStorage`.

现在再把代码给完整:

```python
_tensor_classes = set()
_storage_classes = set()

class DoubleStorage(_C.DoubleStorageBase, _StorageBase):
    pass
class FloatStorage(_C.FloatStorageBase, _StorageBase):
    pass
class LongStorage(_C.LongStorageBase, _StorageBase):
    pass
class IntStorage(_C.IntStorageBase, _StorageBase):
    pass
class ShortStorage(_C.ShortStorageBase, _StorageBase):
    pass
class CharStorage(_C.CharStorageBase, _StorageBase):
    pass
class ByteStorage(_C.ByteStorageBase, _StorageBase):
    pass

class DoubleTensor(_C.DoubleTensorBase, _TensorBase):
    pass
class FloatTensor(_C.FloatTensorBase, _TensorBase):
    pass
class LongTensor(_C.LongTensorBase, _TensorBase):
    pass
class IntTensor(_C.IntTensorBase, _TensorBase):
    pass
class ShortTensor(_C.ShortTensorBase, _TensorBase):
    pass
class CharTensor(_C.CharTensorBase, _TensorBase):
    pass
class ByteTensor(_C.ByteTensorBase, _TensorBase):
    pass

_storage_classes.add(DoubleStorage)
_storage_classes.add(FloatStorage)
_storage_classes.add(LongStorage)
_storage_classes.add(IntStorage)
_storage_classes.add(ShortStorage)
_storage_classes.add(CharStorage)
_storage_classes.add(ByteStorage)

_tensor_classes.add(DoubleTensor)
_tensor_classes.add(FloatTensor)
_tensor_classes.add(LongTensor)
_tensor_classes.add(IntTensor)
_tensor_classes.add(ShortTensor)
_tensor_classes.add(CharTensor)
_tensor_classes.add(ByteTensor)

# This shadows Torch.py and Storage.py
setDefaultTensorType('torch.DoubleTensor')
```

这样的话, `torch.Tensor` 对象就能使用 `csrc/generic/TensorMethods.cpp` 中定义的各种方法了, 如 `add, add_` 等. 综上 `torch` 中的 `add` 方法由 `csrc/Module.cpp` 等提供, 而 `torch.Tensor` 中的 `add` 以及 `add_` 等方法由 `csrc/generic/TensorMethods.cpp` 提供.



## 剩下的部分内容

`__init__.py` 文件还有一些内容, 比如:

```python
from .serialization import save, load
```

加载 `torch.save` 以及 `torch.load`;

另外还有:

```python
################################################################################
# Initialize extension
################################################################################

_C._initExtension()
```

用于载入 `torch.cuda` 模块, 当然, 这个需要检测安装好了 CUDA.

该方法在 `csrc/Module.cpp` 中定义:

```c
// csrc/Module.cpp

// Callback for python part. Used for additional initialization of python classes
static PyObject * THPModule_initExtension(PyObject *self)
{
  // 下面的三个函数定义在 Module.cpp 文件的开头
  if (!THPModule_loadClasses(self))         return NULL;
  if (!THPModule_assignStateless(self))     return NULL;
  if (!THPModule_initCopy(self))            return NULL;
  return PyBool_FromLong(true); // 看起来如果调用成功, 则返回 True
}


// 下面是 csrc/Module.cpp 中的内容
static PyMethodDef TorchMethods[] = {
  {"_initExtension",  (PyCFunction)THPModule_initExtension,     METH_NOARGS,  NULL},
 // ...
  
};  
```

`__init__.py` 最后一点剩下的内容是:

```python
################################################################################
# Remove unnecessary members
################################################################################

del DoubleStorageBase
del FloatStorageBase
del LongStorageBase
del IntStorageBase
del ShortStorageBase
del CharStorageBase
del ByteStorageBase
del DoubleTensorBase
del FloatTensorBase
del LongTensorBase
del IntTensorBase
del ShortTensorBase
del CharTensorBase
del ByteTensorBase
```

不需要这些接口, 我们以后直接使用 `torch.DoubleTensor` 等接口.