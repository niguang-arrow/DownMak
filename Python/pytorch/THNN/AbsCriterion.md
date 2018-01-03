# AbsCriterion

## 2018 年 1 月 3 日

这个方法和 Abs 的区别在于, Abs 只有 input 作为输入, 但是 Criterion 还需要 target 作为跟 input 的比较. 由于该方法是 Criterion, 所以它只需要求 output 以及向后传播 $\delta$ 即可.



## THNN_(AbsCriterion_updateOutput)

`TH_TENSOR_APPLY2` 的 CODE 中将 input 和 target 之间的差值的绝对值进行累加, 如果 `sizeAverage` 为 True 的话, 那么最后就能得到误差的平均值. 

其中 `THTensor_(set1d)` 在 `pytorch/torch/lib/TH/generic/THTensor.c` 文件中被定义:

```c
void THTensor_(set1d)(THTensor *tensor, long x0, real value)
{
  THArgCheck(tensor->nDimension == 1, 1, "tensor must have one dimension");
  THArgCheck( (x0 >= 0) && (x0 < tensor->size[0]), 2, "out of range");
  THStorage_(set)(tensor->storage, tensor->storageOffset+x0*tensor->stride[0], value);
}
```

将结果设置为 1d 的 tensor. 可以看到底层是在操作 storage.

```c
void THNN_(AbsCriterion_updateOutput)(
          THNNState *state,
          THTensor *input,
          THTensor *target,
          THTensor *output,
          bool sizeAverage)
{
  real sum = 0;

  TH_TENSOR_APPLY2(real, input, real, target,
    sum += fabs(*input_data - *target_data);
  );

  if (sizeAverage)
    sum /= THTensor_(nElement)(input);

  THTensor_(set1d)(output, 0, sum);
}
```



## THNN_(AbsCriterion_updateGradInput)

同样的, 导数的求取, 如果输出 `>= 0` 那么导数为 1, 否则为 -1. 使用 `norm` 是还要考虑 `sizeAverage`.

```c
void THNN_(AbsCriterion_updateGradInput)(
          THNNState *state,
          THTensor *input,
          THTensor *target,
          THTensor *gradInput,
          bool sizeAverage)
{
  real norm = (sizeAverage ? 1./((real)THTensor_(nElement)(input)) : 1.);

  THTensor_(resizeAs)(gradInput, input);
  TH_TENSOR_APPLY3(real, gradInput, real, input, real, target,
    *gradInput_data = (*input_data - *target_data) >= 0 ? norm : -norm;
  );
}
```

