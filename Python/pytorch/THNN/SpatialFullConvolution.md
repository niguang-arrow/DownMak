# SpatialFullConvolution

## 2018 年 1 月 1 日

今天开始看 pytorch(v0.1.1) 的源码, 首先阅读的是 SpatialFullConvolution.c 中的代码.

+ 注意, 在 vim 中使用 `:echo expand("%:p")` 可以得到当前文件的完整路径. [Get the name of the current file](http://vim.wikia.com/wiki/Get_the_name_of_the_current_file)

首先最好是查看一下 `/home/ieric/pytorch/torch/lib/THNN/doc/api_reference.md` 以及 `style_guidelines.md` 两个文件, 了解一下 pytorch 中的代码风格. 注意对于 modules, 参数的格式如下:

```bash
updateOutput: state, input, output, ...
updateGradInput: state, input, gradOutput, gradInput, ...
accGradParameters: state, input, gradOutput, [gradWeight], [gradBias], ...
```

而对于 Criterions, 参数如下:

```bash
updateOutput: state, input, target, output, ...
updateGradInput: state, input, target, gradInput, ...
```

那么下面看 `SpatialFullConvolution.c` 中的代码, 该文件位于: `/home/ieric/pytorch/torch/lib/THNN/generic/` 目录下.

另外注意在 style guide 文件中说, 所有函数都应该返回 void.

## THNN_(im2col)

这是第一个函数, 首先注意 C 语言中是没有函数重载的, 所以需要对每一个类型专门写一个函数. 但是 pytorch 采用了宏替换的方法简化了这一个过程. 具体细节应查看 pytorch 的官方博客中的介绍: [A Tour of PyTorch Internals (Part I)](http://pytorch.org/2017/05/11/Internals.html)

该函数名就是一个宏, 其在 `THNN/THNN.h` 文件中定义:

```c
#define THNN_(NAME) TH_CONCAT_3(THNN_, Real, NAME)
```

之后, Real 会被替换为 Double, Float 之类的.

另外还需要关注的是 `THIndexTensor`:

```c
#define THIndexTensor THLongTensor
#define THIndexTensor_(NAME) THLongTensor_ ## NAME
```

在 Criterions 之类的函数中, 参数 target 一般是 THIndexTensor 类型, 它们实际上都是 LongTensor. 

下面具体看看这个函数, (dilation 扩张, 扩大; 膨胀)

```c
static void THNN_(im2col)(const real* data_im, const int channels,
      const int height, const int width, const int kernel_h, const int kernel_w,
      const int pad_h, const int pad_w,
      const int stride_h, const int stride_w,
      const int dilation_h, const int dilation_w,
      real* data_col) {
  const int height_col = (height + 2 * pad_h -
                          (dilation_h * (kernel_h - 1) + 1)) / stride_h + 1;
  const int width_col = (width + 2 * pad_w -
                         (dilation_w * (kernel_w - 1) + 1)) / stride_w + 1;
  const int channels_col = channels * kernel_h * kernel_w;
  for (int c_col = 0; c_col < channels_col; ++c_col) {
    int w_offset = c_col % kernel_w;
    int h_offset = (c_col / kernel_w) % kernel_h;
    int c_im = c_col / kernel_h / kernel_w;
    for (int h_col = 0; h_col < height_col; ++h_col) {
      for (int w_col = 0; w_col < width_col; ++w_col) {
        int h_im = h_col * stride_h - pad_h + h_offset * dilation_h;
        int w_im = w_col * stride_w - pad_w + w_offset * dilation_w;
        data_col[(c_col * height_col + h_col) * width_col + w_col] =
          (h_im >= 0 && w_im >= 0 && h_im < height && w_im < width) ?
          data_im[(c_im * height + h_im) * width + w_im] : 0;
      }
    }
  }
}
```

首先, 当 `dilation = 1` 时, 输出的长度(height 或者 width), 比如 height 为 `height_col = (height + 2*pad_h - kernel_h)/stride_h + 1` 非常好理解, 而对于 `dilation > 1` 时, 观察 https://github.com/vdumoulin/conv_arithmetic 中最后一个 `No padding, no stride, dilation` 的动图, 可以看到, 当加上 dilation 时, kernel 相当于扩大了, 由 `kernel_h` 扩大为 `(kernel_h - 1)` 再乘上 `dilation_h`(比如图中就是 2), 同时还得加上 1. (比如图中, kernel 为 3, dilation 为 2, 那么扩大为 (`3 - 1) * 2 + 1 = 5`), 所以就有代码中的 `height_col` 与 `width_col` 的求法. 关于 dilated Convolution 可以看 http://cs231n.github.io/convolutional-networks/, 其中给出了原论文. 当然, 还可以看看知乎上的相关讨论.

