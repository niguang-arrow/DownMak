# Readme

## 2018 年 1 月 4 日

本目录下的文档主要针对 pytorch(v0.1.1) 中 `pytorch/torch/nn/` 目录下代码中的见解.

本文档假设你已经阅读过 `_thnn/` 目录下的文档. 

这个目录情况复杂, 还要考虑 autograd... 我还没有看, 不知能否搞定... 总之, 即使现在理解出现了偏差, 以后多使用, 必然会理解得更为深刻, 到时候再回来修改. (立个 Flag!)

目前这些目录之间的关系终于清楚了.

## `__init__.py`

将 `modules/` 目录下定义的所有方法导入进来.

```python
from .modules import *
```



## cuda.py

知道大致意思即可, 在 `_thnn/__init__.py` 中定义的 type2backend 增加对 Cuda 浮点类型的引用. 但是我暂时不考虑 Cuda 的情况.

```python
# Importing this adds THCUNN to type2backend dict in torch._thnn
# This automatically enables CUDA in all THNN ops
import torch._thnn.thcunn
```

