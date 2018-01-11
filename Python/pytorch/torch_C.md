# torch._C

## 2018 年 1 月 11 日

要说明 `torch/_C.so` 文件是怎么来的, 必须去 `pytorch/torch/setup.py` 文件中看一看:

```python
# setup.py
## ..... ##
cwd = os.path.dirname(os.path.abspath(__file__))
lib_path = os.path.join(cwd, "torch", "lib")

tmp_install_path = lib_path + "/tmp_install"
include_dirs += [
    cwd,
    os.path.join(cwd, "torch", "csrc"),
    tmp_install_path + "/include",
    tmp_install_path + "/include/TH",
]

extra_link_args.append('-L' + lib_path)


main_libraries = ['TH']
main_sources = [
    "torch/csrc/Module.cpp",
    "torch/csrc/Generator.cpp",
    "torch/csrc/Tensor.cpp",
    "torch/csrc/Storage.cpp",
    "torch/csrc/utils.cpp",
    "torch/csrc/serialization.cpp",
]


C = Extension("torch._C",
    libraries=main_libraries,
    sources=main_sources,
    language='c++',
    extra_compile_args=extra_compile_args,
    include_dirs=include_dirs,
    extra_link_args=extra_link_args + [make_relative_rpath('lib')]
)
## ..... ##
```

可以看到, 如果要编译 `_C.so` 的话, 需要 `pytorch/torch/csrc/` 中的 `Module.cpp`, `Generator.cpp`, `Tensor.cpp`, `Storage.cpp`, `utils.cpp`, `serialization.cpp`. 但这些代码中必然要调用更为底层中的 `pytorch/torch/lib/TH/` 目录下的库(`lib_path` 不就说明了... 而且还指明了会产生一个 `tmp_install` 目录)

那么之后, 我们就需要去 `lib/TH/` 目录下看看, 要知道, 我们在前面可是分析了 `lib/THNN/` 目录下的文件.



## 2018 年 1 月 10 日

寻寻觅觅, 冷冷清清

进入到 `pytorch/` 下, 

```python
import torch._C
from pprint import pprint as pp
pp(dir(torch._C))
```

结果如下: 总共有 150 项.

```python
['ByteStorageBase',
 'ByteTensorBase',
 'CharStorageBase',   
 'CharTensorBase',   
 'DoubleStorageBase',                                        
 'DoubleTensorBase',   
 'FloatStorageBase',   
 'FloatTensorBase',   
 'Generator',   
 'IntStorageBase',   
 'IntTensorBase',   
 'LongStorageBase',
 'LongTensorBase',   
 'ShortStorageBase',
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
 'addbmm',
 'addcdiv',
 'addcmul',
 'addmm',
 'addmv',
 'addr',
 'all',
 'any',
 'asin',
 'atan',
 'atan2',
 'baddbmm',
 'bernoulli',
 'bmm',
 'cat',
 'cauchy',
 'cdiv',
 'ceil',
 'cfmod',
 'cinv',
 'clamp',
 'cmax',
 'cmin',
 'cmod',
 'cmul',
 'cos',
 'cosh',
 'cpow',
 'cremainder',
 'cross',
 'csub',
 'cumprod',
 'cumsum',
 'diag',
 'dist',
 'div',
 'dot',
 'eq',
 'equal',
 'exp',
 'exponential',
 'eye',
 'fill',
 'floor',
 'fmod',
 'frac',
 'gather',
 'ge',
 'geometric',
 'ger',
 'getNumThreads',
 'getRNGState',
 'gt',
 'histc',
 'indexAdd',
 'indexCopy',
 'indexFill',
 'indexSelect',
 'kthvalue',
 'le',
 'lerp',
 'linspace',
 'log',
 'log1p',
 'logNormal',
 'logspace',
 'lt',
 'manualSeed',
 'maskedSelect',
 'max',
 'mean',
 'median',
 'min',
 'mm',
 'mod',
 'mode',
 'mul',
 'multinomial',
 'mv',
 'narrow',
 'ne',
 'neg',
 'nonzero',
 'norm',
 'normal',
 'numel',
 'ones',
 'pow',
 'prod',
 'rand',
 'randn',
 'random',
 'randperm',
 'range',
 'remainder',
 'renorm',
 'reshape',
 'round',
 'rsqrt',
 'scatter',
 'setNumThreads',
 'setRNGState',
 'sigmoid',
 'sign',
 'sin',
 'sinh',
 'sort',
 'sqrt',
 'squeeze',
 'std',
 'sum',
 't',
 'tan',
 'tanh',
 'topk',
 'trace',
 'transpose',
 'tril',
 'triu',
 'trunc',
 'unfold',
 'uniform',
 'var',
 'zero',
 'zeros']
```

而使用如下代码总共得到 172 项结果, 

```python
import torch
from pprint import pprint as pp
pp(dir(torch))
```

结果中删除了 `(TPYE)StorageBase`(共 14 项), 增加了如下内容:

```python
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
 'isTensor',
 'isStorage',
 'isLongStorage',
 'getDefaultTensorType',
 'setDefaultTensorType',
 'typename',
 'save',
 'load',
 # 上面总共 40 项(注意 torch._C 共有的有 4 项, 其余 36 项是 torch 中的)

 # 下面总共 132 项.
 # 下面的内容上面的 torch._C 中有
 'abs',                                                                                      
 'acos',                                                                                     
 'add',                                                                                      
 'addbmm',                                                                                   
 'addcdiv',                                                                                  
 'addcmul',
 # ....# 
]
```

