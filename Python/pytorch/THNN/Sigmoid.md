# Sigmoid

## 2018 年 1 月 3 日

sigmoid 的公式是:
$$
f(x) = \frac{1}{1 + e^{-x}} = \frac{e^x}{1 + e^x}
$$
pytorch 将该文件命名为 `Sigmoid.c`, 位于 `pytorch/torch/lib/THNN/generic/Sigmoid.c`. 实现了 `THNN_(Sigmoid_updateOutput)` 和 `THNN_(Sigmoid_updateGradInput)` 两个 API, 根据 `THNN/doc/style_guidelines.md` 文件的说明, 它是一个 Criterion, 所以只需要实现这两个 API 即可.



## THNN_(Sigmoid_updateOutput)

由于输入和输出大小必须是一样的, 所以先使用 `THTensor_(resizeAs)` 保证这一条件. 然后使用 `TH_TENSOR_APPLY2` 对输入进行处理. `TH_TENSOR_APPLY2` 这个宏定义在 `pytorch/torch/lib/TH/THTensorApply.h` 文件中. 这个宏定义的非常复杂. 不过后来我在新版本的 `pytorch/aten/src/TH/THTensorApply.h` 文件中找到了比较简洁的定义, 摘录如下:

```c
#define TH_TENSOR_APPLY2_D(TYPE1, TENSOR1, TYPE2, TENSOR2, DIM, CODE) \
{ \
  int TH_TENSOR_APPLY_hasFinished = 0; \
  int64_t TH_TENSOR_dim_index = 0; \
  __TH_TENSOR_APPLYX_PREAMBLE(TYPE1, TENSOR1, DIM, 1) \
  __TH_TENSOR_APPLYX_PREAMBLE(TYPE2, TENSOR2, DIM, 1) \
\
    if(TENSOR1##_n != TENSOR2##_n) {                                    \
      THDescBuff T1buff = _THSizeDesc(TENSOR1->size, TENSOR1->nDimension); \
      THDescBuff T2buff = _THSizeDesc(TENSOR2->size, TENSOR2->nDimension); \
      THError("inconsistent tensor size, expected %s %s and %s %s to have the same " \
              "number of elements, but got %d and %d elements respectively", \
              #TENSOR1, T1buff.str, #TENSOR2, T2buff.str, TENSOR1##_n, TENSOR2##_n); \
    }                                                                   \
  while(!TH_TENSOR_APPLY_hasFinished) \
  { \
    /* Loop through the inner most region of the Tensor */ \
    for(; TENSOR1##_i < TENSOR1##_size && TENSOR2##_i < TENSOR2##_size; TENSOR1##_i++, TENSOR2##_i++, TENSOR1##_data += TENSOR1##_stride, TENSOR2##_data += TENSOR2##_stride) /* 0 et pas TENSOR##_dim! */ \
    { \
      CODE \
    } \
    __TH_TENSOR_APPLYX_UPDATE_COUNTERS(TENSOR1, 0) \
    __TH_TENSOR_APPLYX_UPDATE_COUNTERS(TENSOR2, 0) \
  } \
  if(TENSOR1##_counter != NULL) \
    THFree(TENSOR1##_counter); \
  if(TENSOR2##_counter != NULL) \
    THFree(TENSOR2##_counter); \
}

#define TH_TENSOR_APPLY2(TYPE1, TENSOR1, TYPE2, TENSOR2, CODE) \
  TH_TENSOR_APPLY2_D(TYPE1, TENSOR1, TYPE2, TENSOR2, -1, CODE)
```

可以看到, 很多细节其实隐藏在了 `__TH_TENSOR_APPLYX_PREAMBLE` 宏中. 这里要注意的是 `TH_TENSOR_APPLY2` 提供了那些接口, 比如参数的意义分别是指定第一个参数的类型 `TYPE1` 以及第一个参数 `TENSOR1`, 第二个参数的类型 `TYPE2` 以及第二个参数 `TENSOR2`, 最后是执行代码. 而在 `TH_TENSOR_APPLY2_D` 中, `CODE` 主要在 for 循环中执行, 其中定义了局部参数, `TENSOR1##_i` 和 `TENSOR2##_i` 分别对 `TENSOR1` 和 `TENSOR2` 进行计数; 并且从 `THNN_(Sigmoid_updateOutput)` 的 CODE 中可以看出, 带上后缀 `_data` 的宏是实际被处理的 tensor. 

另外, 需要注意的是, output 是我们要求的结果, 它放在 `TENSOR1` 中. 之后要说明的 `THNN_(Sigmoid_updateGradInput)` 函数中也有类似的用法, 注意.

下面就可以说说 `TH_TENSOR_APPLY2` 的作用, 它将输出结果保存在 `TENSOR1` 中. 它对 `TENSOR2` 中每个输入利用 `CODE` 进行处理后, 将结果依次保存在 `TENSOR1` 中. (毕竟输入一般都是一个 batch)

这是 Sigmoid 求输出的代码:

```c
void THNN_(Sigmoid_updateOutput)(
          THNNState *state,
          THTensor *input,
          THTensor *output)
{
  THTensor_(resizeAs)(output, input);

  TH_TENSOR_APPLY2(real, output, real, input,
    *output_data = 1./(1.+ exp(- *input_data));
  );
}
```



## THNN_(Sigmoid_updateGradInput)

有了前面的介绍, 这里的代码也非常好理解. 要求前一层的 $\delta$ (`gradInput`), 需要知道当前层的 $\delta$ (`gradOutput`) 以及当前层 Sigmoid 的输出, 毕竟 Sigmoid 的导数可以使用 $f^{\prime}(z) = (1 - z) * z$ 求取. 之后只要使用 `gradOutput` 乘上该导数即可.

首先仍使用 `THTensor_(resizeAs)`, 由于 input 和 output 的输出大小是一样的, 所以 `gradInput` 和 output 的大小是一样的. `TH_TENSOR_APPLY3` 和 `TH_TENSOR_APPLY2` 的作用是一样的, 只不过多用了一个参数而已, 结果保存在 `gradInput` 中. 

```c
void THNN_(Sigmoid_updateGradInput)(
          THNNState *state,
          THTensor *input,
          THTensor *gradOutput,
          THTensor *gradInput,
          THTensor *output)
{
  THTensor_(resizeAs)(gradInput, output);
  TH_TENSOR_APPLY3(real, gradInput, real, gradOutput, real, output,
    real z = *output_data;
    *gradInput_data = *gradOutput_data * (1. - z) * z;
  );
}
```

