# engine.py

## 2018 年 1 月 6 日

这个要结合 variable.py 文件一起看. 主要被用在 Variable 类的 backward 方法中.

先记录一些只言片语:

+   `assert d_prev_fn is None`: 由于 `prev_fn.requires_grad` 为 False, 那么在 Function 中反向传播(backward) 时, 返回的对应的 tuple 中相应 input 的位置的梯度为 None. (因为输入就不要求保存 grad 了, 所以 `d_prev_fn` 必须为 None)
+   `grad_input` 中包含参数的梯度, 比如对于 LinearFunction, 包含 `(input, weight, bias)`