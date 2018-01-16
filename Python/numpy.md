# Numpy Note

日期: 2018 年 1 月 16 日

+ `np.random.permutation(x)`

  [url](https://docs.scipy.org/doc/numpy-1.13.0/reference/generated/numpy.random.permutation.html)

  x 是 int 或者 `array_like`(比如 list 之类的). 如果 x 为整数, 相当于处理 `np.arange(x)`. 如果 x 是后者, 那么对 x 进行拷贝然后再随机 shuffle 其中的元素. 输出为 ndarray.

  另外, 如果 x 为 multi-dimensional array, 那么只 shuffle 第一个维度.

  ​

