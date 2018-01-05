# thnn.py

## 2018 年 1 月 4 日

前面介绍了 `linear.py`, 下面看对于其他模块的定义.

其实我们可以猜一下这个文件的作用. 之后我看完代码之后再来验证是否想对了. 首先要注意到在 `pytorch/torch/_thnn/_THNN.so` 模块中, 并没有定义对 Linear 函数的求解, 因此, 在 linear.py 文中, 使用了 `torch.addmm_`, `torch.mm` 等方法完成了 output 的求取, 同时定义了自己的 backward 方法.

但是  `pytorch/torch/_thnn/_THNN.so` 模块中定义了如卷积模块, Criterion 模块等前向传播和参数梯度的求取方法, 那么在这个 `thnn.py` 文件中, 只需要将这些模块继承于 Function, 并定义各自的 `forward` 和 `backward` 方法即可.

我想, 这个文件做的工作就是这个了. 下面就来进行具体的分析.

首先这是 import 的内容:

```python
import torch._thnn.thnn
from torch._thnn.utils import parse_header, THNN_H_PATH
from torch.autograd import Function
from torch._thnn import type2backend
```

前面介绍过的内容都用上了.

首先来看 `_make_function_class`, 虽然它不是该文件的第一个函数.



## _make_function_class

先看结果, 最后返回的是一个类!

这个类的类名为 `class_name`, 继承于 `Function`, 同时还具有 `__init__`, `forward`, `backward` 等方法. 和我前面的想法如出一辙... 

```python
def _make_function_class(class_name, update_output, update_grad_input, acc_grad_parameters):
    
    # ....... #

    return type(class_name, (Function,), dict(__init__=__init__, forward=forward, backward=backward))
```

下面看这个函数的具体处理:

### has_argument

我们拿 `SpatialConvolutionMM` 举例.(有些内容需要先看看本文件中后面的代码), `_make_function_class` 的输入参数有: 

+   `class_name` 为 `SpatialConvolutionMMFunction` (看后面的 `_generate_function_classes` 函数)

+   `update_output` 为 `_thnn/utils.py` 文件中定义的 Function 对象, 打印这个对象可以得到 

    ```bash
    SpatialConvolutionMM_updateOutput(THNNState* state, THTensor* input, THTensor* output, THTensor* weight, THTensor* bias, THTensor* finput, THTensor* fgradInput, int kW, int kH, 
    int dW, int dH, int padW, int padH)

    # 可以采取如下方式得到这个结果, 首先看看 utils.md 中的指示使用 ipython
    # l = parse_header(THNN_H_PATH)
    # fn_by_name = {fn.name: fn for fn in l}
    # print fn_by_name['SpatialConvolutionMM_updateOutput']
    # 即可得到结果. 之后的 update_grad_input, acc_grad_parameters 均可以使用如上方式得到
    ```

+   `update_grad_input` 和 `acc_grad_parameters` 均如 `update_output`. 不妨都打印一下:

    ```bash
    SpatialConvolutionMM_updateGradInput(THNNState* state, THTensor* input, THTensor* gradOutput, THTensor* gradInput, THTensor* weight, THTensor* finput, THTensor* fgradInput, int 
    kW, int kH, int dW, int dH, int padW, int padH)

    SpatialConvolutionMM_accGradParameters(THNNState* state, THTensor* input, THTensor* gradOutput, THTensor* gradWeight, THTensor* gradBias, THTensor* finput, 
    THTensor* fgradInput, int kW, int kH, int dW, int dH, int padW, int padH, real scale)
    ```

+   之后看 `has_argument` 函数, 由于 fn 是 Function 对象(定义在 `_thnn/utils.py` 中而不是 autograd 中), 它有参数 `arguments` 保存着所有的参数. 这个函数判断参数 `name` 是否在 `fn` 中. 由于 `update_grad_input` 中没有 `output` 参数, 所以 `save_output` 为 False.

    ```python
    # grad_input 为 Function 对象('_thnn/utils.py' 中定义)
    grad_input = fn_by_name['SpatialConvolutionMM_updateGradInput']
    print grad_input.arguments
    [THNNState* state,                                                                           
     THTensor* input,                                                                            
     THTensor* gradOutput,                                                                       
     THTensor* gradInput,                                                                       
     THTensor* weight,
     THTensor* finput,
     THTensor* fgradInput,
     int kW,
     int kH,
     int dW,
     int dH,
     int padW,
     int padH]

    # 而 update_output 中的参数为:
    update_output = fn_by_name['SpatialConvolutionMM_updateOutput']
    print update_output.arguments
    [THNNState* state,                                                                           
     THTensor* input,                                                                            
     THTensor* output,                                                                           
     THTensor* weight,                                                                           
     THTensor* bias,
     THTensor* finput,
     THTensor* fgradInput,
     int kW,
     int kH,
     int dW,
     int dH,
     int padW,
     int padH]
    ```

+   之后定义了 `buffers_idx` ... 具体意义还需看后面的代码. 

+   补充: 看了后面的代码, 到 `forward` 函数中会明白 `buffers_idx` 的作用, 主要用来处理函数中的额外参数的.

```python
# 定义在 _make_function_class 函数中
def has_argument(fn, name):
    for arg in fn.arguments:
        if arg.name == name:
            return True
    return False
save_output = has_argument(update_grad_input, 'output')

buffers_idx = []
additional_arg_idx = 0
for arg in update_output.arguments[3:]:
    if arg.name in {'weight', 'bias'}:
        continue
    # TODO: index tensors, etc.
    if arg.type == 'THTensor*':
        buffers_idx.append(additional_arg_idx)
        additional_arg_idx += 1
```

### `__init__`

要理解这个函数, 我们要将类给写出来, 还是以 `SpatialConvolutionMM` 为例:

```python
class  SpatialConvolutionMMFunction(Function):
    def __init__(self, *args):
        super(type(self), self).__init__()
        self.additional_args = list(args)
```

只是用列表 `additional_args` 保存额外的参数.



### forward

依旧如上面的分析

+   首先, 根据输入 input 的类型(Float 或 Double) 选择合适的后端;

+   由于 `buffers_idx` 值为 `[0, 1]` (还是以 `SpatialConvolutionMM` 为例, 它的 `update_output.arguments` 中有两个参数符合上面代码的要求: `finput` 和 `fgradInput`), 那么之后 `self.additional_args` 的结果为:

    ```python
    # 要得到这个结果, 我修改了代码, 使其能输出中间结果, 但是 ipython 下我暂时
    # 没有去考虑怎样让改变立即生效, 因此直接使用的 Python 而不是 ipython.
    self.additional_args:  
    [
        [torch.DoubleTensor with no dimension], 
        [torch.DoubleTensor with no dimension], 
        arg0, 
        arg1
        #....
    ]
    # [torch.DoubleTensor with no dimension] 是 input.new() 的结果
    ```

    其中 `arg0` 和 `arg1` 等是在 `__init__` 时传入的参数.

    那么这个时候, 我们就知道了:

    传入底层函数的参数是如下顺序组织的:

    ```python
    (self.backend.library_state, input, output, *additional_args)
    ```

    而 `additional_args` 前面有:

    ```python
    additional_args = params + self.additional_args
    ```

    此时注意, `params` 是传入给 `SpatialConvolutionFunction` 的 forward 方法中的参数, 联想一下 LinearFunction 的做法, `params` 主要是 `weight` 以及 `bias`. 而 `self.additional_args` 是这个类初始化时传入的参数, 结合一下我们使用 pytorch 的经验, 比如 `nn.Conv2d`, 我们要传入 kernel 的大小, padding 的大小等参数, 相当于底层函数参数中的 `kW, kH, padW, padH` 等, 而 `self.additional_args` 从 `0` 开始的几个参数都是为一些特殊的参数, 比如 `finput`, `fgradInput` 等保留的. 经过上面的处理之后, `addtional_args` 中参数的顺序就和底层函数参数的顺序一致了:

    ```python
    update_output = fn_by_name['SpatialConvolutionMM_updateOutput']
    print update_output.arguments # 底层函数的参数
    [THNNState* state, # 相当于 self.backend.library_state
     THTensor* input,  # input
     THTensor* output, # output
     THTensor* weight, # weight 和 bias 相当于 params
     THTensor* bias,   # 包括在 params 中
     THTensor* finput, # self.additional_args[0], 它和下面的相当于 input.new()
     THTensor* fgradInput, # self.additional_args[1]
     int kW, # 后面的都是 self.additional_args[2:]
     int kH,
     int dW,
     int dH,
     int padW,
     int padH]
    ```

+   `update_output.name` 为 `'SpatialConvolutionMM_updateOutput'` 字符串, 而 `self.backend` 定义了 `__getattr__` 方法, 可以根据这个字符串, 找到对应的 `FloatSpatialConvolutionMM_updateOutput` 或者 `DoubleSpatialConvolutionMM_updateOutput` 函数对象, 然后这个函数对象会接受 `(state, input, output ....)` 等参数.

    注意到:

    ```python
    getattr(self.backend, update_output.name)(self.backend.library_state, input, output, *additional_args)
    ```

    这句代码将调用底层的卷积方法计算卷积层的结果, 输出保存在 output 中. 注意 `output = input.new()` 可以这样做是因为底层代码中会将 output 设置为合适的大小.

+   由于 `SpatialConvolutionMM` 的反向传播中并没有 output 参数, 所以 `save_output` 为 False. 因此并没有保存 `self.output`, 而是直接输出.

```python
class  SpatialConvolutionMMFunction(Function):
    def forward(self, input, *params):
        self.backend = type2backend[type(input)]
        self.params = params
        self.input = input
        for idx in buffers_idx:
            self.additional_args = self.additional_args[:idx] + 
            	[input.new()] + self.additional_args[idx:]
        self.additional_args = tuple(self.additional_args)
        output = input.new()
        additional_args = params + self.additional_args
        getattr(self.backend, update_output.name)(self.backend.library_state, input, output, *additional_args)
        if save_output:
            self.output = output
        return output
```



### 总结 forward

从 forward 的代码我们可以知道, 首先 forward 对参数进行处理, 使得底层函数能使用正确的参数. 之后调用底层的 `_updateOutput` 函数计算最终的输出.

另外注意和 LinearFunction 不同, 这里的 forward 中没有保存 `self.forward_args` 用于反向传播的计算, 这是因为底层定义了反向传播的代码.



### backward

先看看完整的代码:

+   `grad_params` 是权重比如 `weight`, `bias` 的梯度, `grad_input_tuple` 是 input 的梯度. `additional_args` 根据情况增加一个额外的参数 output(因为底层函数中 `output` 参数可能是 optional 的)

+   代码中给出了注释, 对于传入 `update_grad_input_fn` 的参数, 看看对应的参数

    ```python
    grad_input = fn_by_name['SpatialConvolutionMM_updateGradInput']
    print grad_input.arguments
    [THNNState* state, # self.backend.library_state             
     THTensor* input,  # self.input
     THTensor* gradOutput, # grad_output
     THTensor* gradInput, # grad_input 要求的值
     THTensor* weight, # gi_args[0]: params_without_bias
     THTensor* finput, # 后面就是 additional_args 了
     THTensor* fgradInput,
     int kW,
     int kH,
     int dW,
     int dH,
     int padW,
     int padH]
    ```

    之后将 `grad_input` 保存在 `grad_input_tuple` 中.

+   对于 `SpatialConvolutionMM_accGradParameters` 函数的参数, 如下:

    ```python
    acc_grad_parameters = fn_by_name['SpatialConvolutionMM_accGradParameters']
    print acc_grad_parameters.arguments
    [THNNState* state, # self.backend.library_state
     THTensor* input, # self.input
     THTensor* gradOutput, # grad_output
     THTensor* gradWeight, # grad_params[0]
     THTensor* gradBias, # grad_params[1]
     THTensor* finput, # additional_args
     THTensor* fgradInput,
     int kW,
     int kH,
     int dW,
     int dH,
     int padW,
     int padH,
     real scale] # 最后的 (1, ), 就是 1
    ```

    注意最后还有个 `scale` 参数, 所以 `param_args` 最后还加上了 `(1, )`, 表示 scale 为 1.

    最后将梯度的参数保存在 `grad_params` 中.

```python
class  SpatialConvolutionMMFunction(Function):
    grad_params = tuple(None for p in self.params)
    grad_input_tuple = (None,)
    additional_args = self.additional_args if not save_output else (self.output,) + self.additional_args
	
    # 若要求 input 的梯度
    if self.needs_input_grad[0]:
        # 初始化
        grad_input = self.input.new().resizeAs_(self.input).zero_()
        # weight(不包括 bias) 参数, 因为求 input 的梯度不需要 bias
        params_without_bias = self.params if len(self.params) < 2 else self.params[:1]
        # 获取底层的求 input_grad 的函数, 比如 
        # FloatSpatialConvolutionMM_updateGradInput
        update_grad_input_fn = getattr(self.backend, update_grad_input.name)
        # gi_args 表示 grad input 需要的参数
        gi_args = params_without_bias + additional_args
        update_grad_input_fn(self.backend.library_state, self.input, grad_output, grad_input, *gi_args)
        grad_input_tuple = (grad_input,)
    # 如果要求参数的梯度, 还要满足 acc_grad_parameters 不为空
    # 毕竟底层代码中还有许多函数是 Criterion, 它们并没有定义
    # _accGradParameters 方法.
    if acc_grad_parameters and any(self.needs_input_grad[1:]):
        grad_params = tuple(p.new().resizeAs_(p).zero_() for p in self.params)
        acc_grad_parameters_fn = getattr(self.backend, acc_grad_parameters.name)
        param_args = grad_params + additional_args + (1,)
        acc_grad_parameters_fn(self.backend.library_state, self.input, grad_output, *param_args)
    
    return grad_input_tuple + grad_params

```

结合 LinearFunction 的 backward 代码, 我们知道, 最后 backward 要返回一个 tuple, 分别保存是 input 和 params 的梯度.



### 总结 _make_function_class

你可以把这个函数理解为可以产生大量的类似 LinearFunction 的类, 每个类都定义了 `__init__`, `forward` 以及 `backward` 方法, 类名类似于: `SpatialConvolutionMMFunction` 等.





## _make_function_class_criterion

下面看 `_make_function_class_criterion` 这个函数的作用. 

这个函数和前面的 `_make_function_class` 一样, 一开始有一个对参数的处理, 我们先不管, 直接看类的定义, 之后再来看参数的处理. 不过代码还是要先放在这里:

+ 首先 `arg.name.startswith('weight')` 这段代码是处于如下的考虑, 在底层的 Criterion 函数的参数中, 有部分函数有 `weights` 这个参数, 比如:

  ```c
  TH_API void THNN_(ClassNLLCriterion_updateOutput)(
            THNNState *state,            // library's state
            THTensor *input,             // input tensor (1D/2D)
            THIndexTensor *target,       // tensor containing indexes of target classes
            THTensor *output,            // [OUT] a one-element tensor with loss
            bool sizeAverage,            // if true, the loss will be normalized by batch size and class weights
            THTensor *weights,           // [OPTIONAL] class weights
            THTensor *total_weight);     // [BUFFER]
  ```

  这段代码应该就是要处理这种情况. 另外, `update_output.arguments[4:]` 表明, 额外的参数是从索引为 4 时开始, 那么像

  ```c
  THNNState *state,            // library's state
  THTensor *input,             // input tensor (1D/2D)
  THIndexTensor *target,       // tensor containing indexes of target classes
  THTensor *output,            // [OUT] a one-element tensor with loss
  ```

  这前四个参数是所有 Criterion 函数所共有的.

  `buffers_idx` 记录除了 `weights` 以及前四个参数之外的其他参数. 下面看类的生成.

```python
def _make_function_class_criterion(class_name, update_output, update_grad_input, acc_grad_parameters):
    weight_arg_idx = -1
    for i, arg in enumerate(update_output.arguments):
        if arg.name.startswith('weight'):
            weight_arg_idx = i
            break

    buffers_idx = []
    additional_arg_idx = 0
    for arg in update_output.arguments[4:]:
        # TODO: index tensors, etc.
        if not arg.name.startswith('weight') and arg.type == 'THTensor*':
            buffers_idx.append(additional_arg_idx)
        additional_arg_idx += 1

    def __init__(self, target, *args, **kwargs):
        super(type(self), self).__init__()
        self.target = target
        self.weight = kwargs.get('weight')
        self.additional_args = list(args)

    def forward(self, input):
        self.backend = type2backend[type(input)]
        self.input = input
        if weight_arg_idx >= 0:
            insert_idx = weight_arg_idx - 4 # state, input, target, output
            self.additional_args.insert(insert_idx, self.weight)
        for idx in buffers_idx:
            self.additional_args.insert(idx, input.new(1))
        output = input.new(1)
        getattr(self.backend, update_output.name)(self.backend.library_state, input, self.target,
            output, *self.additional_args)
        return output

    def backward(self, grad_output):
        grad_input = grad_output.new().resizeAs_(self.input).zero_()
        getattr(self.backend, update_grad_input.name)(self.backend.library_state, self.input, self.target,
            grad_input, *self.additional_args)
        return grad_input

    return type(class_name, (Function,), dict(__init__=__init__, forward=forward, backward=backward))
```



### `__init__` 

初始化:

```python
def __init__(self, target, *args, **kwargs):
    super(type(self), self).__init__()
    self.target = target
    self.weight = kwargs.get('weight')
    self.additional_args = list(args)
```

主要考虑参数中可能会用 `weight` 指明 loss 前的权重, 请看 http://pytorch.org/docs/0.3.0/nn.html#torch.nn.NLLLoss



### forward

使用 `weight_arg_idx` 指明底层 API 中 `weights` 所在的索引, 使用 `insert_idx` 指明在参数中要插入的位置. (注意一下 insert 方法, 比如对于一个空 list, `[].insert(10, None)` 最后的结果是 `[None]`, 不会报错的).

由于参数处理的代码中:

```python
for arg in update_output.arguments[4:]:
    # TODO: index tensors, etc.
    if not arg.name.startswith('weight') and arg.type == 'THTensor*':
        buffers_idx.append(additional_arg_idx)
    additional_arg_idx += 1
```

当 if 中的条件不满足, `additional_arg_idx` 是会增加的. 这样保证了满足 if 条件的参数与 `weights` 的相对位置不变.

比如下面参数中, 只有 `total_weight` 参数满足 if 的条件, 此时 `additional_arg_idx` 的值是 2 (注意前面的 sizeAverage 和 weights 都不满足 if 条件, 所以最后 `additional_arg_idx` 为 2).

```c
TH_API void THNN_(ClassNLLCriterion_updateOutput)(
          THNNState *state,
          THTensor *input,
          THIndexTensor *target, 
          THTensor *output,
          bool sizeAverage,
          THTensor *weights,
          THTensor *total_weight); 
```

那么我们就能确定了, 使用如上的参数处理方式, `__init__` 会有部分参数满足 `weights` 前面的类型不为 `THTensor*` 的参数. 其他 `THTensor*` 类型的参数和 `weights` 的相对位置不变, 而 `__init__` 中剩下的参数则会出现在 `total_weight` 的后面(也许有其他的函数还有多出来的类型不为 `THTensor*` 的参数)

这里关于参数的处理稍微有些绕, 可能是我没有真正的理解. 之后再说...

```python
def forward(self, input):
    self.backend = type2backend[type(input)]
    self.input = input
    if weight_arg_idx >= 0:
        insert_idx = weight_arg_idx - 4 # state, input, target, output
        self.additional_args.insert(insert_idx, self.weight)
    for idx in buffers_idx:
        self.additional_args.insert(idx, input.new(1))
    output = input.new(1)
    getattr(self.backend, update_output.name)(self.backend.library_state, input, self.target,
                                              output, *self.additional_args)
    return output
```

之后调用 `_updateOutput` 方法计算输出.

对了, python 运行模块并进入交互式界面的命令是:

```bash
python -i -m module
```



### backward

反向传播的代码看起来简单多了, 毕竟不用对权重进行求导. 

```python
def backward(self, grad_output):
  grad_input = grad_output.new().resizeAs_(self.input).zero_()
  getattr(self.backend, update_grad_input.name)(self.backend.library_state, self.input, self.target,
                                                grad_input, *self.additional_args)
  return grad_input

```

对比一下 `_outputOutput` 和 `_updateGradInput` 的参数:

```c
TH_API void THNN_(ClassNLLCriterion_updateOutput)(
          THNNState *state,            // library's state
          THTensor *input,             // input tensor (1D/2D)
          THIndexTensor *target,       // tensor containing indexes of target classes
          THTensor *output,            // [OUT] a one-element tensor with loss
          bool sizeAverage,            // if true, the loss will be normalized by batch size and class weights
          THTensor *weights,           // [OPTIONAL] class weights
          THTensor *total_weight);     // [BUFFER]

TH_API void THNN_(ClassNLLCriterion_updateGradInput)(
          THNNState *state,            // library's state
          THTensor *input,             // input tensor (1D/2D)
          THIndexTensor *target,       // tensor containing indexes of target classes
          THTensor *gradInput,         // [OUT] gradient w.r.t. input
          bool sizeAverage,            // if true, the loss will be normalized by batch size and class weights
          THTensor *weights,           // [OPTIONAL] class weights
          THTensor *total_weight);     // [BUFFER]

```

会发现除了 `output` 和 `gradInput` 不同之外, 其他的参数都是相同的. 所以反向传播的代码简单多了.



## _generate_function_classes

看完了以上 `_make_function_class` 和 `_make_function_class_criterion` 函数的实现原理, 下面看看各种类似于 LinearFunction 的类是如何产生的.

```python
_function_list = parse_header(THNN_H_PATH)
_function_by_name = {fn.name: fn for fn in _function_list}
def _generate_function_classes(scope_dict):
    classes_to_generate = {fn.name.partition('_')[0] for fn in _function_list}
    exceptions = {
        'SparseLinear',
        'BatchNormalization',
        'LookupTable',
        'unfolded',
    }
    classes_to_generate -= exceptions
    for fn in classes_to_generate:
        update_output = _function_by_name[fn + '_updateOutput']
        update_grad_input = _function_by_name[fn + '_updateGradInput']
        acc_grad_parameters = _function_by_name.get(fn + '_accGradParameters')
        class_name = fn + 'Function'
        # This has to call a function to retain correct references to functions
        if 'Criterion' in fn:
            cls = _make_function_class_criterion(class_name, update_output,
                    update_grad_input, acc_grad_parameters)
        else:
            cls = _make_function_class(class_name, update_output,
                    update_grad_input, acc_grad_parameters)
        scope_dict[class_name] = cls
        _generated_functions.append(cls)
```

+ 首先 `_function_list` 是一个包含一堆 Function 对象(定义在 `_thnn/utils.py` 文件中), `_function_by_name` 为字典, 根据函数名找到 Function 对象, 即:

  ```python
  {
      "Abs_updateOutput": torch._thnn.utils.Function("Abs_updateOutput"),
      "SpatialConvolutionMM_updateGradInput": 
      		torch._thnn.utils.Function("SpatialConvolutionMM_updateGradInput"),
      # ......... #
  }
  ```

+ `classes_to_generate` 是一个 Set, 它减去了 `exceptions` 这个 Set, 这是因为, 比如 `BatchNormalization`, 只定义了 `BatchNormalization_updateOutput` 与 `BatchNormalization_backward`.

+ 之后对于 `classes_to_generate` 中的每个 Class, 获得它们的 `update_output` 等 Function 对象(注意 `acc_grad_parameters` 使用 `.get` 方法得到, 结果可以为 None), 并且给每个 Class 的名字加上 `Function` 标示. 比如对于 `SpatialConvolutionMM` 这个 Class, 最后 `class_name` 为 `SpatialConvolutionMMFunction`. 

+ 然后根据 fn 名字中是否有 `Criterion`, 调用合适的函数进行处理, 它们都能生成一个类 cls. 

+ 最后将 cls 分别加入到 `scope_dict` 中以及 `_generate_functions` 中. 关于它们的定义, 在该文件最后面有:

  ```python
  # _generate_functions 初始是 [BatchNormalizationFunction], 之后会在 
  # _generate_function_classes 函数中被扩充;
  # 而 scope_dict 为 locals() 局部空间.
  _generated_functions = [BatchNormalizationFunction]
  _generate_function_classes(locals())
  ```

最后还需要单独处理一下 `BatchNormalizationFunction`

## BatchNormalizationFunction

先看初始化函数:

### `__init__`

根据前面的经验, 一般初始化函数读入的参数类型不是 `THTensor*` 的:

```python
class BatchNormalizationFunction(Function):
    def __init__(self, *args):
        super(BatchNormalizationFunction, self).__init__()
        self.additional_args = args
```

对于 `THTensor*` 类型参数的处理一般都是在 `forward` 中给出的.



### forward

forward 函数计算输出, 我们结合底层 API 的声明看看, forward 函数定义如下:

```python
def forward(self, input, *params):
    self.backend = type2backend[type(input)]
    self.params = params
    self.input = input
    self.num_features = input.size(1)
    # Add save_input and save_std
    self.additional_args = self.additional_args[:2] + \
    	(input.new(self.num_features), input.new(self.num_features)) + \
    	self.additional_args[2:]
    num_params = len(self.params)
    if num_params < 2:
        params = params + tuple(None for i in range(2 - num_params))
    additional_args = params + self.additional_args
    output = input.new().resizeAs_(input)
    self.backend.BatchNormalization_updateOutput(self.backend.library_state,
            input, output, *additional_args)
    return output
```

底层的 API 为:

```c
TH_API void THNN_(BatchNormalization_updateOutput)(
          THNNState *state,   // self.backend.library_state
          THTensor *input,  // input
          THTensor *output,  // output
          THTensor *weight, // [OPTIONAL] 作者有注释为 optional, 所以 params < 2
          THTensor *bias,  // [OPTIONAL] bias 和 weight 为 params
          THTensor *running_mean,  // 这里是 self.additional_args[:2], 这里在 __init__ 中
          THTensor *running_var, // 会读入这些参数.
          THTensor *save_mean, // input.new(self.num_features); 为它们俩保留
          THTensor *save_std,
          bool train, // 后面是 self.additional_args[2:]
          double momentum,
          double eps);
```

设置好参数后, 调用底层的 `THNN_(BatchNormalization_updateOutput)` 计算出结果.

下面看 backward.



### backward

代码如下, 同样去对比底层的 api:

```python
def backward(self, grad_output):
    grad_input = (self.input.new().resizeAs_(self.input).zero_()
            if self.needs_input_grad[0] else None,)
    grad_param = tuple(p.new().resizeAs_(p).zero_() if self.needs_input_grad[i+1]
            else None for i, p in enumerate(self.params))
    result_grad = grad_input + grad_param

    num_params = len(self.params)
    if num_params < 2:
        grad_param = grad_param + tuple(None for i in range(2 - num_params))

    weight_tuple = (self.params[0],) if len(self.params) > 0 else (None,)
    # backward takes scale instead of momentum
    additional_args = self.additional_args[:-2] + (1,) + self.additional_args[-1:]
    args = grad_input + grad_param + weight_tuple + additional_args
    self.backend.BatchNormalization_backward(self.backend.library_state,
            self.input, grad_output, *args)
    return result_grad
```

底层的 api 为: 

+   注意最后 `(gradInput, gradWeight, gradBias)` 为输出结果.

```c
TH_API void THNN_(BatchNormalization_backward)(
          THNNState *state, // self.backend.library_state
          THTensor *input, // self.input
          THTensor *gradOutput, // grad_output
          THTensor *gradInput,  // [OPTIONAL] grad_input
          THTensor *gradWeight, // [OPTIONAL] grad_param (包括下面的 gradBias)
          THTensor *gradBias,   // [OPTIONAL]
          THTensor *weight,     // [OPTIONAL] weight_tuple 中的 self.params[0], 该层的权重
          THTensor *running_mean, // 从这里开始一直到 train 是 self.additional_args[:-2]
          THTensor *running_var,
          THTensor *save_mean,
          THTensor *save_std,
          bool train,
          double scale, // 单独设置这个参数为 (1, ),
          double eps); // self.additional_args[-1:]
```



## 总结 (2018 年 1 月 5 日)

终于看完这个文件了, 内容比较充实, 这个文件主要提供了两个函数 `_make_function_class` 和 `_make_function_class_criterion` 可以批量的生产类, 简化了人工劳动. 看完之后, 不得不为其中的精巧赞叹. 要达到这样的效果, 必须首先对整个项目有个好的规划, 所有的 API 尽量统一风格.

另外, 我发现, 有时候代码的逻辑并不复杂, 难的是整体的设计. pytorch 的这一点做的实在是精彩, 代码看起来是一种享受, 每次看都能有不同的体会. 反观自己的代码, 都不想再去看第二遍....

还有, 代码中命名非常重要好, 有的时候不是代码难写, 而是名字不好取; 可以看到 pytorch 中命名的风格就非常统一, 看完一个代码之后, 变量名字的意义在其他的地方也是类似的, 减小了理解代码的工作量.