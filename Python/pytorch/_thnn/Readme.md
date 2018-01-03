# Readme

## 2018 年 1 月 3 日

本目录存放的是对 `pytorch/torch/_thnn` 目录下文件的阅读情况. 阅读本目录下的文档假设你已经阅读过上一层 `csrc/` 目录下的文档.

下面再介绍下 `__init__.py` 文件.



## init 文件

代码定义在 `pytorch/torch/_thnn/__init__.py`  文件中.

代码非常简单, 如下: 

初始化了一个 `_backends` 对象, 用于记录...(之后补充)

另外定义了一个名为 `type2backend` 的字典, 用于...(之后补充)

```python
class Backends(object):
    pass
_backends = Backends()

type2backend = {}
```

