# SpatialConvolutionMM

## 2018 年 1 月 2 日

昨天和今天介绍了 `SpatialFullConvolution.c` 文件的内容. 今天再介绍一下 `SpatialConvolutionMM.c` 的内容. 这个文件应该就是用来做 `nn.Conv2d` 的底层代码, 该文件位于 https://github.com/pytorch/pytorch/blob/v0.1.1/torch/lib/THNN/generic/SpatialConvolutionMM.c



## THNN_(SpatialConvolutionMM_updateOutput_frame)

首先要注意到这个函数是一个 static 函数, 因此只能被本文件内的函数访问. 真正用于计算 Output 的是下面要介绍的 `THNN_(SpatialConvolutionMM_updateOutput)`.

