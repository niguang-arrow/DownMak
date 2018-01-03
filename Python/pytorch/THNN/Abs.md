# Abs

## 2018 年 1 月 3 日

越战越勇.

看懂了前面的内容, 后面的代码理解起来会越来越快的. 原来风格好, 质量高的代码读起来这么爽. 慢慢来, 一步一步学习.

Abs 计算简单, 主要关注的是对于求导的处理, 主要在 `x = 0` 的位置处导数的处理.



## THNN_(Abs_updateOutput)

直接使用 `THTensor_(abs)` 求出结果. 其中 `THTensor_(abs)` 在 `pytorch/torch/lib/THNN/generic/THTensorMath.h` 中声明, 但是在 `pytorch/torch/lib/THNN/generic/THTensorMath.c` 中定义稍微不容易找到, 实现为:

```c
#define LAB_IMPLEMENT_BASIC_FUNCTION(NAME, CFUNC)             \
  void THTensor_(NAME)(THTensor *r_, THTensor *t)                \
  {                                                           \
    THTensor_(resizeAs)(r_, t);                               \
    TH_TENSOR_APPLY2(real, t, real, r_, *r__data = CFUNC(*t_data);); \
  }                                                           \

#if defined(TH_REAL_IS_LONG)
LAB_IMPLEMENT_BASIC_FUNCTION(abs,labs)
#endif /* long only part */

#if defined(TH_REAL_IS_INT)
LAB_IMPLEMENT_BASIC_FUNCTION(abs,abs)
#endif /* int only part */
```

根据输入参数类型的不同, 选择不同的函数. 注意在 `LAB_IMPLEMENT_BASIC_FUNCTION` 宏中使用了 `TH_TENSOR_APPLY2`, 最后的结果是保存在 `r_` 中的, 所以和我在 `Sigmoid.md` 中关于 `TH_TENSOR_APPLY2` 的描述稍有偏差... 但明白意思就可以了. 不知道为什么这里需要交换一下位置, 为了可读性?

```c
void THNN_(Abs_updateOutput)(
          THNNState *state,
          THTensor *input,
          THTensor *output)
{
  THTensor_(resizeAs)(output, input);
  THTensor_(abs)(output, input);
}
```



## THNN_(Abs_updateGradInput)

求前一层的 $\delta$, 可以看到, pytorch 的 abs 的导数求解是: 如输出 `z >= 0` 的话, 那么导数为 `1`, 否则为 `-1`.

```c
void THNN_(Abs_updateGradInput)(
          THNNState *state,
          THTensor *input,
          THTensor *gradOutput,
          THTensor *gradInput)
{
  THTensor_(resizeAs)(gradInput, input);
  TH_TENSOR_APPLY3(real, gradInput, real, gradOutput, real, input,
    real z = *input_data;
    *gradInput_data = *gradOutput_data * (z >= 0 ? 1 : -1);
  );
}
```



