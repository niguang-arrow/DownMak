# MSECriterion

## 2018 年 1 月 3 日

现在发现 pytorch(v0.1.1) 的 MSE 没有乘上 1/2. 不知道新版本有没有? (右转去 github 上看了一下, 并没有乘 1/2)

下面看下实现代码:

## THNN_(MSECriterion_updateOutput)

可以看到, 输出直接是平方, 没有乘上 1/2.

```c
void THNN_(MSECriterion_updateOutput)(
          THNNState *state,
          THTensor *input,
          THTensor *target,
          THTensor *output,
          bool sizeAverage)
{
  real sum = 0;

  TH_TENSOR_APPLY2(real, input, real, target,
    real z = (*input_data - *target_data);
    sum += z*z;
  );

  if (sizeAverage)
    sum /= THTensor_(nElement)(input);

  THTensor_(set1d)(output, 0, sum);
}
```



## THNN_(MSECriterion_updateGradInput)

反向传播, 考虑 `sizeAverage`.

```c
void THNN_(MSECriterion_updateGradInput)(
          THNNState *state,
          THTensor *input,
          THTensor *target,
          THTensor *gradInput,
          bool sizeAverage)
{
  real norm = (sizeAverage ? 2./((real)THTensor_(nElement)(input)) : 2.);

  THTensor_(resizeAs)(gradInput, input);
  TH_TENSOR_APPLY3(real, gradInput, real, input, real, target,
    *gradInput_data = norm * (*input_data - *target_data);
  );
}
```

