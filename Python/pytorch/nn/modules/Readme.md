# Readme

## 2018 年 1 月 5 日

本目录下的文档是对 pytorch(v0.1.1) 的 `pytorch/torch/nn/modules/` 目录下源代码的解析.

阅读本文档假设你已经阅读过上一层目录中的 `backends/` 和 `functions/` 目录中的文档, 或者已经理解 `pytorch/torch/nn/functions/thnn.py` 文件中的内容.



终于要走到顶层的 API 了. `modules/` 文件夹中的内容提供了我们经常使用的顶层 API. 是对于 `functions/` 中定义的各种 `Function` 类(定义在 `pytorch/autograd/function.py` 文件中) 的封装.

我们首先来看最为重要的 `module.py` 文件, 其中定义了 `Module` 类.



## `__init__` 

该文件中的内容使得我们能直接使用 `nn.Conv2d`, `nn.Linear` 等 API:

```python
from .linear import Linear
from .conv import Conv2d
from .activation import Threshold, ReLU, HardTanh, ReLU6, Sigmoid, Tanh, \
    Softmax, Softmax2d, LogSoftmax
from .criterion import AbsCriterion, ClassNLLCriterion
from .container import Container, Sequential
from .pooling import MaxPooling2d
from .batchnorm import BatchNorm, BatchNorm2d
```

另外, 我们注意在 `backends/thnn.py` 中的 `backend` 对象中, 总共注册了 57 个 Function 类.