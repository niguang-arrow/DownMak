# THNN

## 2018 年 1 月 3 日

下面来分析一下 `pytorch/torch/lib/THNN/` 这个目录, 看看是如何组织的. 首先这个目录的组成如下:

```bash
# tree -L 1
.
├── CMakeLists.txt
├── doc
├── generic
├── init.c
├── README.md
└── THNN.h

2 directories, 4 files
```

在 `generic` 目录下定义了前面介绍过的 `SpatialFullConvolution.c`, `Abs.c` 等文件, 是实际的 API 接口. 但是由于 C 语言不支持函数重载, 所以必然要做一些处理. 那么首先来看一下 `init.c` 中的内容.



## init.c

这个文件的内容有点多, 把前面部分内容展示出来. 注意, 像 `TH.h`, `THGenerateFloatTypes.h` 等文件定义在 `pytorch/torch/lib/TH/` 目录下. 

```c
#include "TH.h"
#include "THNN.h"

#define torch_(NAME) TH_CONCAT_3(torch_, Real, NAME)
#define nn_(NAME) TH_CONCAT_3(nn_, Real, NAME)

#include "generic/Abs.c"
#include "THGenerateFloatTypes.h"

#include "generic/AbsCriterion.c"
#include "THGenerateFloatTypes.h"

#include "generic/ClassNLLCriterion.c"
#include "THGenerateFloatTypes.h"

#include "generic/SpatialClassNLLCriterion.c"
#include "THGenerateFloatTypes.h"

/* ..... */ 
```

由于上面 include 了 `THNN.h` (位于 `THNN/` 目录下), 那么首先来看下 `THNN.h` 的内容.



## THNN.h

可以看到:

+   里面用到了 `<omp.h>`, 所以能在代码中使用 omp 提供的多线程语法.

+   定义了 `THNN_(NAME)`, 会被连接成 `THNN_RealNAME`, 其中 Real 会被 `init.c` 中的 `THGenerateFloatTypes.h` 根据情况替换为 Double 或者 Float. 比如, 定义 `THNN_(Abs_updateOutput)` 会被展开成 `THNN_DoubleAbs_updateOutput` 或者 `THNN_FloatAbs_updateOutput`.

+   由于 `generic/` 目录下的所有 .c 文件都是在 `init.c` 中被处理, 也就是函数名都会被替换; 那么这个时候就需要对应的 `.h` 文件中声明了替换名字之后的函数. 在 `THNN.h` 文件中是这样处理的, 使用:

    ```c
    #include "generic/THNN.h"
    #include <THGenerateFloatTypes.h>
    ```

    而在 `generic/THNN.h` 文件中的内容是:

    ```c
    #ifndef TH_GENERIC_FILE
    #define TH_GENERIC_FILE "generic/THNN.h"
    #else

    TH_API void THNN_(Abs_updateOutput)(
              THNNState *state,            // library's state
              THTensor *input,             // input tensor
              THTensor *output);           // [OUT] Abs output
    TH_API void THNN_(Abs_updateGradInput)(
              THNNState *state,            // library's state
              THTensor *input,             // input tensor
              THTensor *gradOutput,        // gradient w.r.t. output
              THTensor *gradInput);        // [OUT] gradient w.r.t. input
    /* ...... */
    ```

    将所有的函数都声明了, 之后通过 `<THGenerateFloatTypes.h>` 的替换操作, 那么所有的函数都被声明.

下面是 THNN.h 的所有代码.

```c
#ifndef THNN_H
#define THNN_H

#include <stdbool.h>
#include <TH.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#define THNN_(NAME) TH_CONCAT_3(THNN_, Real, NAME)

#define THIndexTensor THLongTensor
#define THIndexTensor_(NAME) THLongTensor_ ## NAME

#define THIntegerTensor THIntTensor
#define THIntegerTensor_(NAME) THIntTensor_ ## NAME

typedef long THIndex_t;
typedef int THInteger_t;
typedef void THNNState;

#include "generic/THNN.h"
#include <THGenerateFloatTypes.h>

#endif
```



## 总结

现在来理清一下其中的关系:

1.  首先, 编译的时候, 源文件应该是 `init.c`, 它 `include` 了与它同目录下的 `THNN/THNN.h` 文件.
2.  `THNN/THNN.h` 文件将 `THNN/generic/THNN.h` 文件 include 进来, 该文件声明了所有函数的名字, 比如 `THNN_(Abs_updateOutput)` 等, 然后 `THNN/THNN.h` 利用 `<THGenerateFloatTypes.h>` 将所有函数名进行替换, 变成 `THNN_DoubleAbs_updateOutput` 等
3.  之后 `init.c` 中的内容是将 `generic` 中所有的 .c 文件 `include` 进来, 这些 .c 文件中包含了所有的函数定义, 但是函数名字却是  `THNN_(Abs_updateOutput)` 等, 然后再利用 `<THGenerateFloatTypes.h>` 将所有函数的类型给替换(和 `THNN/THNN.h` 中的操作一致), 函数名变成了 ``THNN_DoubleAbs_updateOutput` 等.
4.  最终的结果是, `THNN/` 目录下, `THNN.h` 包含了所有函数声明, `init.c` 包含了所有函数定义.
5.  它们两个的共同特点是: `THNN.h` include 了位于 `generic/` 目录下的 `THNN.h` 文件, 而 `init.c` include 了 `generic/` 目录下的所有 .c 文件.
6.  这个真相给我的启示是: 当前目录下的 .h 和 .c 就是我们需要的, 但是要找实际干活的文件, 还是要到 `generic/` 目录下去查阅.



## 插个楼

刚刚浏览了一下 `pytorch/torch/csrc/nn/THNN.cpp` 文件, 这是将 `THNN/` 目录中定义的函数扩展到 Python 中的关键. 文件最后有:

```c
#if PY_MAJOR_VERSION == 2
  ASSERT_TRUE(module = Py_InitModule("torch._thnn._THNN", module_methods));
#else
  ASSERT_TRUE(module = PyModule_Create(&module_def));
#endif
```

也就是说在 `torch/_thnn/` 目录下会生成 `_THNN.so` 文件. 果然, 我到 `pytorch/torch/_thnn/` 目录下, 果然看到了该文件. 

进入 `pytorch/torch/_thnn/` 目录, 结构为:

```bash
# tree -L 1
.
├── __init__.py
├── thcunn.py
├── thnn.py
├── _THNN.so
└── utils.py

0 directories, 5 files
```

在 `__init__.py` 文件中定义了:

```python
class Backends(object):
    pass
_backends = Backends()

type2backend = {}
```

也就是定义了 `_backends` 对象, 以及 `type2backend` 字典.

而在 `thnn.py` 文件中, 我们有:

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

也就是在 utils.py 文件中定义着我们需要的内容, 如 `load_backend`, `parse_header`.

首先看一下 `THNN_H_PATH` 的定义:

```python
THNN_H_PATH = os.path.join(os.path.dirname(__file__), '..', 'lib', 'THNN.h')
```

`parse_header` 就是处理 `pytorch/torch/lib/THNN.h` 文件.

而在 `THNN.h` 文件中, 定义着:

```c
#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/THNN.h"
#else

TH_API void THNN_(Abs_updateOutput)(
          THNNState *state,            // library's state
          THTensor *input,             // input tensor
          THTensor *output);           // [OUT] Abs output
TH_API void THNN_(Abs_updateGradInput)(
          THNNState *state,            // library's state
          THTensor *input,             // input tensor
          THTensor *gradOutput,        // gradient w.r.t. output
          THTensor *gradInput);        // [OUT] gradient w.r.t. input
/* ...... */
```

也就是所有在 `pytorch/torch/lib/THNN/` 目录下, 定义的所有函数!

`parse_header` 就是解析这个文件, 将这些函数名提取出来! 这个可以运行 Python 看效果. 之后可以做做实验.