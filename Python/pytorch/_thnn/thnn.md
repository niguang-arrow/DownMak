# thnn.py

## 2018 年 1 月 4 日

该文件定义在 `pytorch/torch/_thnn/` 目录下, 代码也比较少:

```python
import torch._thnn._THNN
from .utils import THNN_H_PATH, parse_header, load_backend
from . import type2backend

generic_functions = parse_header(THNN_H_PATH)
for t in ['Float', 'Double']:
    backend = load_backend(t, torch._thnn._THNN, generic_functions)
    type2backend['torch.' + t + 'Tensor'] = backend
    type2backend[getattr(torch, t + 'Tensor')] = backend
```



### generic_functions

根据我在 `utils.md` 文档中的描述, `generic_functions` 是一个 list, 里面是一堆 Function 对象,(每个 Function 对象保存自己的函数参数对象 Argument) 将一个 Function 对象打印出来, 可以得到比如:

```bash
Abs_updateOutput(THNNState* state, THTensor* input, THTensor* output)
```

在 pytorch (v0.1.1) 中, 打印出 `len(generic_functions)` 可以得到 138. 

### load_backend

`load_backend` 主要做两件事情:

1.  对于每个 `t`, 设置 `_backends` 对象(在 `__init__.py` 中定义)的属性, 也即:

```bash
# THNNFloatBackend 这个类实在 load_backend 中定义的
# backend = type("THNNFloatBackend", THNNBackendBase,), {})()
# 也就是说 backend 是一个 THNNFloatBackend 对象
# 我用 := 表示设置属性
_backends.THNNFloatBackend := THNNFloatBackend 对象
_backends.THNNFDoubleBackend := THNNDoubleBackend 对象
```

2.  对于每个 backend, 注册所有对应类型的函数:

```python
for function in generic_functions:
    full_fn_name = '{}{}'.format(t, function.name)
    fn = getattr(lib_handle, full_fn_name)
    backend.register_method(function.name, fn)
```

由于 `lib_handle` 为这里的 `_THNN.so` 模块, `full_fn_name` 根据 `t` 的不同, 可能是 `FloatAbs_updateOutput` 或 `DoubleAbs_updateOutput`. 而在 `lib_handle` 中已经定义了这些函数, 这样只需要将模块中的函数对象 `fn` 加到 backend 的 methods 字典中.

我们将 `backend.methods` 打印出来看看: (总共有 138 项), 而且还要注意这只是 backend 为 `THNNFloatBackend` 对象时的结果, 当 backend 为 `THNNDoubleBackend` 对象时, 也有 138 项.

```bash
{'VolumetricFullConvolution_accGradParameters': 
	<built-in function FloatVolumetricFullConvolution_accGradParameters>,   
 'PReLU_updateGradInput': 
 	<built-in function FloatPReLU_updateGradInut>, 
 'VolumetricFullConvolution_updateGradInput': 
    <built-in function FloatVolumetricFullConvolution_updateGradInput>,   
 'SoftShrink_updateGradInput': 
    <built-in function FloatSoftShrink_updateGradInput>, 
 'TemporalSubSampling_updateGradInput': 
    <built-in function FloatTemporalSubSampling_updateGradInput>, 
    
 # .............. #
 }
```

(本文档最后面给出如何打印出这个结果)

继续对 `thnn.py` 中代码的讨论, 之后我们有:

```python
type2backend['torch.' + t + 'Tensor'] = backend
type2backend[getattr(torch, t + 'Tensor')] = backend
```

`type2backend` 是在 `__init__.py` 文件中定义的字典, 而 backend 是 `THNNFloatBackend` 对象与 `THNNDoubleBackend` 对象之一. 代码最后的结果是:(Double 的我没有给出)

```python
{'torch.FloatTensor': <torch._thnn.utils.THNNFloatBackend at 0x7fda6775d810>,
 torch.FloatTensor: <torch._thnn.utils.THNNFloatBackend at 0x7fda6775d810>,
 'torch.DoubleTensor': .....,
 torch.DoubleTensor: .......
}
```

也就是说, 如果我们有 `type2backend` 这个字典, 我们可以通过类型字符串 `torch.FloatTensor` 或者 `torch.FloatTensor` 这个类型对象直接找到对应的后端 backend, 从 backend 中可以调用底层的函数.



## 总结

thnn.py 主要给 `__init__.py` 中定义的 `_backends` 和 `type2backend` 填充了内容, 对于 `_thnn` 这个包来说, 现在调用 `_backends` 可以获得 `THNNFloatBackend` 与 `THNNDoubleBackend` 两个后端. 以及通过 `type2backend` 获取对应的后端. 

注意在 `NN` (神经网络) 中所有的数据类型都是浮点型, 所以只 generate 了 Float 和 Double 类型.



## 代码的运行

现在简要说一下如何运行 pytorch 中的代码查看结果, 比如前面介绍过的打印出 `backend` 的 methods 等.

首先下载 pytorch 源代码(v0.1.1), 然后进入主目录 `pytorch/`, 使用 `python setup.py build develop` 进行编译, 由于使用的 `develop` 模式, 之后在该目录下使用 `import torch` 时导入的是当前目录的 `torch`, 而不是系统目录中的 torch. (使用 `develop` 编译之后会在 `pytorch/` 目录下看到一个名为 `torch.egg-info/` 的目录).

你看在 python 中使用 `torch.__file__` 查看 python 调用的是哪个 torch. 并且在系统的 python 的 `lib/python2.7/site-packages/` 目录下的 `torch.egg-link` 文件中会出现如下内容:

```bash
# cd {Python_Path}/lib/python2.7/site-packages
# cat torch.egg-link
/home/ieric/pytorch
.%
```

这个时候, 由于使用的是 pytorch(v0.1.1)

在 `pytorch/` 目录下启动 ipython, 输入下面的命令:

```python
In [1]: import torch                                                                         
                                                                                              
In [2]: torch.__file__                                                                       
Out[2]: 'torch/__init__.pyc'

In [3]: import torch._thnn._THNN

In [4]: from torch._thnn.utils import THNN_H_PATH

In [5]: from torch._thnn.utils import parse_header, load_backend

In [6]: generic_functions = parse_header(THNN_H_PATH)

In [7]: backend = load_backend("Float", torch._thnn._THNN, generic_functions)

In [8]: backend
Out[8]: <torch._thnn.utils.THNNFloatBackend at 0x7fda6775d810>
    
In [11]: print backend.methods
{'VolumetricFullConvolution_accGradParameters': 
 	<built-in function FloatVolumetricFullConvolution_accGradParameters>,   
 'PReLU_updateGradInput': 
 <built-in function FloatPReLU_updateGradInput>, 
 # ....... (138 个结果#
}
```