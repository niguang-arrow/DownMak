# thnn.py

## 2018 年 1 月 5 日

由于该目录下的文件的代码比较少, 我就将两个文件 `backend.py` 和 `thnn.py` 中的内容一起说明了. 分开来说的话反倒不便于理解.

虽然 `thnn.py` 文件要调用 `backend.py` 文件中定义的 `FunctionBackend` 类, 我们首先还是看看 `thnn.py` 文件中的内容.



## THNNFunctionBackend

这个类继承于 `backend.py` 文件中定义的 FunctionBackend 类:

```python
from .backend import FunctionBackend

class THNNFunctionBackend(FunctionBackend):
    pass
```

但仅仅是继承而已, 并没有定义自己的方法. 而 `FunctionBackend` 在 `backend.py` 中的定义如下:

```python
class FunctionBackend(object):
    def __init__(self):
        self.function_classes = {}

    def __getattr__(self, name):
        fn = self.function_classes.get(name)
        if fn is None:
            raise NotImplementedError
        return fn

    def register_function(self, name, function_class):
        if self.function_classes.get(name):
            raise RuntimeError("Trying to register second function under name " + name + " in " + type(self).__name__)
        self.function_classes[name] = function_class
```

**插个楼:** 必须得说, 这个实现非常像 `pytorch/torch/_thnn/utils.py` 文件中定义的 `THNNBackendBase` 类 , 比如:

```python
class THNNBackendBase(object):
    def __init__(self):
        self.methods = {}

    def __getattr__(self, name):
        method = self.methods.get(name, None)
        if method is None:
            raise NotImplementedError
        return method

    def register_method(self, name, ctypes_fn):
        self.methods[name] = ctypes_fn

    @property
    def library_state(self):
        return 0
```

它们都定义了 `__getattr__` 方法, 用于直接通过属性获得对应的 method. 另外, 都定义了 `register_` 方法. 用于注册对应的 `function` 或 `method`.  `backend.py` 要考虑那些 `Function` 类, 而 `THNNBackendBase` 考虑的是底层的各种方法.

下面看对于这个类的使用.

## _initialize_backend

首先在文件的最后定义了 `backend` 对象, 再调用 `_initialize_backend` 函数:

```python
backend = THNNFunctionBackend()
_initialize_backend()
```

函数的定义为:

```python
def _initialize_backend():
    from ..functions.thnn import _generated_functions
    from ..functions.linear import LinearFunction

    backend.register_function('Linear', LinearFunction)
    name_remap = {
        'SpatialConvolutionMMFunction': 'Conv2dFunction',
        'SpatialMaxPoolingFunction': 'MaxPooling2dFunction',
        'SoftMaxFunction': 'SoftmaxFunction',
        'LogSoftMaxFunction': 'LogSoftmaxFunction',
        'BatchNormalizationFunction': 'BatchNormFunction',
    }
    for cls in _generated_functions:
        name = cls.__name__
        new_name = name_remap.get(name, name)
        backend.register_function(new_name.replace('Function', ''), cls)
```

主要利用了 `functions/` 目录下定义的内容. 首先 `_generated_functions` 包含了所有的 Function 类(除了 `LinearFunction`), 比如 `_generated_functions` 中的部分内容为:

```python
[BatchNormalizationFunction,
 TemporalSubSamplingFunction,
 LogSoftMaxFunction,
 SpatialFullConvolutionMapFunction,
 MSECriterionFunction
 # ...... # 
]

# 总共有 56 项
```

在 `functions.linear` 中还有 `LinearFunction`, 那么总共有 57 个 Function 类.

之后 `backend` 首先注册 `Linear` Function 类, 注册成功后, backend 的 `function_classes` 属性(是个字典)中保存着从名字到类的映射:

```python
{'Linear': LinearFunction}
```

之后可以使用 `backend.Linear` 直接使用 `LinearFunction` 类.

下一步设置了 `name_remap`, 重新给某些类取新的名字, 比如我能判断 `nn.Conv2d` 的底层代码是有 `SpatialConvolutionMM` 执行的, 就是从这里看到的.

之后将 `_generated_functions` 中的类注册时, 一方面将 `name_remap` 中给定的类的名字给替换掉; 另一方面将名字中的 `Function` 给去除.

最终得到 `backend.function_classes` 为:

```python
{'Abs': torch.nn.functions.thnn.AbsFunction,                                                 
 'AbsCriterion': torch.nn.functions.thnn.AbsCriterionFunction,
 'BatchNorm': torch.nn.functions.thnn.BatchNormalizationFunction,
 'ClassNLLCriterion': torch.nn.functions.thnn.ClassNLLCriterionFunction,
 'Conv2d': torch.nn.functions.thnn.SpatialConvolutionMMFunction,
 'DistKLDivCriterion': torch.nn.functions.thnn.DistKLDivCriterionFunction,
 'ELU': torch.nn.functions.thnn.ELUFunction,
 ### ...... ###
}
# 总共 57 项
```

注: 要得到这个结果, 可以在 ipython 下使用:

```python
# cd pytorch
# ipython
# %run -m torch.nn.backends.thnn
# backend.function_classes 即可得到结果.
```

我常犯的错误是:

```python
# %run -m torch/nn/backends/thnn.py
{Python_PATH}/lib/python2.7/site-packages/IPython/core/magics/execution$
py:608: UserWarning: u'torch/nn/backends/thnn.py' is not a valid modulename on sys.path      
  warn('%r is not a valid modulename on sys.path'%modulename)
```

