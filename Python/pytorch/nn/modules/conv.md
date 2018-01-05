# conv.py

## 2018 年 1 月 5 日

这是 pytorch 卷积层提供的 API, 我们在 Python 代码中使用的就是它了.

```python
import math
import torch
from torch.autograd import Variable

from .module import Module

class Conv2d(Module):
    def __init__(self, in_channels, out_channels, kh, kw, dh=1, dw=1, padh=0, padw=0):
        super(Conv2d, self).__init__()
        self.in_channels = in_channels
        self.out_channels = out_channels
        self.kh = kh
        self.kw = kw
        self.dh = dh
        self.dw = dw
        self.padh = padh
        self.padw = padw

        self.weight = Variable(torch.DoubleTensor(self.out_channels, self.in_channels, self.kh, self.kw))
        self.bias = Variable(torch.DoubleTensor(self.out_channels))

        self.reset_parameters()

    def reset_parameters(self):
        stdv = 1. / math.sqrt(self.kh * self.kw * self.in_channels)
        self.weight.data.uniform_(-stdv, stdv)
        self.bias.data.uniform_(-stdv, stdv)

    def _forward(self, input):
        return self._backend.Conv2d(self.kw, self.kh, self.dw, self.dh, self.padw, self.padh)(input, self.weight, self.bias)
```

感觉新版本的 Conv2d 的接口会发生点变化, 比如 `kh` 和 `kw` 会统一成一个 tuple: `kernel_size` 暂时先不管新版本.

+   首先, 初始化代码中给定卷积层的各个参数, `reset_parameters` 使用高斯函数去初始化权重和 bias. 这种初始化方式在深度学习中非常常见.
+   之后, 由于 `Conv2d` 继承于 `Module`, 所以它应该定义自己的 `_forward()` 方法(否则在 Module 中会出现 `raise NotImplementedError`)

`self._backend` 继承于 Module 中的 `_backend`, 它是定义于 `pytorch/torch/nn/functions/thnn.py` 中的 `backend` 对象, 使用 `.Conv2d` 属性相当于调用 `self._backend.function_class['Conv2d']` 从而获得名为 `SpatialConvolutionMMFunction` 的类. 这个类在 `pytorch/torch/nn/functions/thnn.py` 中的 `_make_function_class` 中已经定义好自己的 forward 方法, 另一方面, 由于这个类继承于 Function 类(定义于 `pytorch/torch/autograd/function.py`), 而 Function 类定义了自己的 `__call__` 方法, 会在得到输入时自动调用 `SpatialConvolutionMMFunction` 类的 forward 方法.

这部分内容在前面的 `module.md` 文档中已经充分描述了.



## Linear

打包一起看看 `linear.py` 中关于 Linear 的定义:

```python
import math

import torch
from torch.autograd import Variable

from .module import Module


class Linear(Module):
    def __init__(self, in_features, out_features):
        super(Linear, self).__init__()
        self.in_features = in_features
        self.out_features = out_features

        self.weight = Variable(torch.DoubleTensor(out_features, in_features))
        self.bias = Variable(torch.DoubleTensor(out_features))

        self.reset_parameters()

    def reset_parameters(self):
        stdv = 1./math.sqrt(self.weight.size(1))
        self.weight.data.uniform_(-stdv, stdv)
        self.bias.data.uniform_(-stdv, stdv)

    def _forward(self, input):
        return self._backend.Linear()(input, self.weight, self.bias)
```

没啥说的, 和 `Conv2d` 一模一样.



## MaxPooling2d

定义在 `pytorch/torch/nn/modules/pooling.py` 文件中:

```python
import torch
from torch.autograd import Variable

from .module import Module

class MaxPooling2d(Module):

    def __init__(self, kh, kw, dh=None, dw=None, padh=0, padw=0, dilh=1, dilw=1, ceil_mode=False):
        super(MaxPooling2d, self).__init__()
        self.kw = kw
        self.kh = kh
        self.dw = dw or kw
        self.dh = dh or kh
        self.padw = padw
        self.padh = padh
        self.dilh = dilh
        self.dilw = dilw
        self.ceil_mode = ceil_mode

    def __call__(self, input):
        return self._backend.MaxPooling2d(self.kw, self.kh, self.dw, self.dh, self.padw, self.padh, self.dilh, self.dilw, self.ceil_mode)(input)[0]
```

可以看到, 并没有定义自己的 `_forward`, 而是直接调用了 `__call__` 处理输入, 直接得到输出结果.



## ReLU

`relu.py` 文件竟然是空的 : ) 

果然还是初版代码好读啊, 刚刚看了一下最新版本的代码, `module.py` 中的代码量已经接近 700 行了(包括大量的注释...)



## Criterion

再看看 Criterion:

```python
from torch.autograd import Variable
from .module import Module

def _assert_no_grad(variable):
    assert not variable.creator.requires_grad, "nn criterions don't compute " \
        "the gradient w.r.t. targets - please mark these variables as not" \
        "requiring gradients"

class AbsCriterion(Module):

    def __init__(self, size_average=True):
        super(AbsCriterion, self).__init__()
        self.size_average = size_average

    def _forward(self, input, target):
        if isinstance(target, Variable):
            _assert_no_grad(target)
            target = target.data
        return self._backend.AbsCriterion(target, self.size_average)(input)

class ClassNLLCriterion(Module):

    def __init__(self, weight=None, size_average=True):
        super(ClassNLLCriterion, self).__init__()
        self.weight = weight
        self.size_average = size_average

    def _forward(self, input, target):
        if isinstance(target, Variable):
            _assert_no_grad(target)
            target = target.data
        return self._backend.ClassNLLCriterion(target, self.size_average, weight=self.weight)(input)

```

注意, `_assert_no_grad(target)` 要求 target 的 `creator` 的 `requires_grad` 属性为 False, 也即不能对 target 求导, 否则会报错.



## 激活函数 activation

`activation.py` 文件的类较多:

+   `ReLU` 类是继承于 `Threshold` 类的. 注意还有 `inplace` 选项.

```python
import torch
from torch.autograd import Variable

from .module import Module


class Threshold(Module):

    def __init__(self, threshold, value, inplace=False):
        super(Threshold, self).__init__()
        self.threshold = threshold
        self.value = value
        self.inplace = inplace
        # TODO: check in THNN (if inplace == True, then assert value <= threshold)

    def _forward(self, input):
        return self._backend.Threshold(self.threshold, self.value, self.inplace)(input)


class ReLU(Threshold):

    def __init__(self, inplace=False):
        super(ReLU, self).__init__(0, 0, inplace)


class HardTanh(Module):

    def __init__(self, min_value=-1, max_value=1, inplace=False):
        super(HardTanh, self).__init__()
        self.min_val = min_value
        self.max_val = max_value
        self.inplace = inplace
        assert self.max_val > self.min_val

    def _forward(self, input):
        return self._backend.HardTanh(self.min_val, self.max_val, self.inplace)(input)


class ReLU6(HardTanh):

    def __init__(self, inplace=False):
        super(ReLU6, self).__init__(0, 6, inplace)


class Sigmoid(Module):

    def _forward(self, input):
        return self._backend.Sigmoid()(input)


class Tanh(Module):

    def _forward(self, input):
        return self._backend.Tanh()(input)


class Softmax(Module):

    def _forward(self, input):
        assert input.dim() == 2, 'Softmax requires a 2D tensor as input'
        return self._backend.Softmax()(input)


class Softmax2d(Module):

    def _forward(self, input):
        assert input.dim() == 4, 'Softmax2d requires a 4D tensor as input'
        return self._backend.Softmax()(input)

class LogSoftmax(Module):

    def _forward(self, input):
        return self._backend.LogSoftmax()(input)
```



## BatchNorm

看来 `BatchNorm2d` 还没有真正完成啊...

`BatchNorm` 也是直接定义了自己的 `__call__` 方法.

```python
import torch
from torch.autograd import Variable

from .module import Module

# TODO: check contiguous in THNN
class BatchNorm(Module):
    expected_dim = 2

    def __init__(self, num_features, eps=1e-5, momentum=0.1, affine=True):
        super(BatchNorm, self).__init__()

        self.affine = affine
        self.eps = eps
        self.momentum = momentum
        self.running_mean = torch.zeros(num_features)
        self.running_var = torch.ones(num_features)

        if self.affine:
            self.weight = Variable(torch.Tensor(num_features))
            self.bias = Variable(torch.Tensor(num_features))
            self.reset_parameters()
        else:
            self.weight = None
            self.bias = None

    def reset_parameters(self):
        if self.weight:
            self.weight.data.uniform_()
        if self.bias:
            self.bias.data.zero_()

        self.running_mean.zero_()
        self.running_var.fill_(1)

    def _checkInputDim(self, input):
        if input.dim() != self.expected_dim:
            raise RuntimeError('only mini-batch supported ({}D tensor), got {}D tensor instead'.format(self.expected_dim, input.dim()))
        if input.size(1) != self.running_mean.nElement():
            raise RuntimeError('got {}-feature tensor, expected {}'.format(input.size(1), self.running_mean.nElement()))

    def __call__(self, input):
        self._checkInputDim(input)
        args = (input,)
        if self.weight is not None:
            args = args + (self.weight, self.bias)
        return self._backend.BatchNorm(self.running_mean,
                self.running_var, self.train, self.momentum, self.eps)(*args)[0]


class BatchNorm2d(Module):
    expected_dim = 4
```

