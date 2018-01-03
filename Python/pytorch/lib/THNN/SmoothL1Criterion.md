# SmoothL1Criterion

## 2018 年 1 月 3 日

关心一下 L1 的求导.



## THNN_(SmoothL1Criterion_updateOutput)

SmoothL1 的公式看起来应是:
$$
f(z) = \left\{\begin{array}{lr}0.5 z^2, & |z| < 1\\ |z| - 0.5, & |z| \geq 1
\end{array}\right.
$$

```c
void THNN_(SmoothL1Criterion_updateOutput)(
          THNNState *state,
          THTensor *input,
          THTensor *target,
          THTensor *output,
          bool sizeAverage)
{
  real sum = 0;
  TH_TENSOR_APPLY2(real, input, real, target,
    real z = fabs(*input_data - *target_data);
    sum += z < 1 ? 0.5*z*z : z - 0.5;
  );

  if (sizeAverage)
    sum /= THTensor_(nElement)(input);

  THTensor_(set1d)(output, 0, sum);
}
```

最后将误差求和, 根据 `sizeAverage` 选择是否进行平均.



## THNN_(SmoothL1Criterion_updateGradInput)

下面是比较关心的内容, 求导的时候分情况讨论, 如果 `|x| > 1`, 那么求导就是对 `|x|` 的求导结果; 如果 `|x| <= 1`, 那么结果就是 `x * (input - target)`. 注意时候需要平均.

```c
void THNN_(SmoothL1Criterion_updateGradInput)(
          THNNState *state,
          THTensor *input,
          THTensor *target,
          THTensor *gradInput,
          bool sizeAverage)
{
  real norm = (sizeAverage ? 1./((real)THTensor_(nElement)(input)) : 1.);

  THTensor_(resizeAs)(gradInput, input);
  TH_TENSOR_APPLY3(real, gradInput, real, input, real, target,
    real x = *input_data - *target_data;
    if (x < -1.)
     *gradInput_data = - norm;
    else if (x > 1.)
     *gradInput_data = norm;
    else
     *gradInput_data = norm * x;
  );
}
```

