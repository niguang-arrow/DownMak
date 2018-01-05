# Readme

## 2018 年 1 月 5 日

本目录存放的是对 `pytorch/torch/autograd` 目录下文件的阅读情况. 阅读本目录下的文档假设你已经阅读过上一层 `nn/` 目录下的文档.

在看完 `nn/modules/` 下的代码之后, 终于可以来研究一下 pytorch 的 autograd 机制了. 希望看明白之后能加深对 `nn/modules/` 目录下代码的理解, 我对那里的代码还有一丝丝小小的疑惑, 希望能在这里找到解答.



## `__init__`

先看看 `__init__.py` 文件中的内容:

```python
from .variable import Variable
from .function import Function
```

 看来 autograd 主要提供 Variable 和 Function.