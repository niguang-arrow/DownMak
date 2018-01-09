# Readme

## 2018 年 1 月 9 日

本目录存放的是对 `pytorch/torch/autograd/functions` 目录下文件的阅读情况. 阅读本目录下的文档假设你已经阅读过上一层目录下的文档.

终于了解了 pytorch 的 autograd 机制, 当然还有一些细节需要解决, 就需要借助这个文件中的代码来理解.



## `__init__.py`

这个文件的内容为:

```python
from .basic_ops import *
from .tensor import *
from .pointwise import *
```

下面我们先来看 `basic_ops.py` 文件中的内容.