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

caffe 对于 im2col 的实现可以看 https://github.com/BVLC/caffe/blob/master/src/caffe/util/im2col.cpp

而对于代码中 `channels_col` 的处理, 首先看下图, 对于 2d 的情况, 有:

![](http://opoddugn5.bkt.clouddn.com/DownMak/Python/pytorch/channel.png)

而对于 3d 的情况, 我们有:

![](http://opoddugn5.bkt.clouddn.com/DownMak/Python/pytorch/channels.png)

也就是说, 这里要做的就是根据一维的 index(代码中是 `channels_col`) 找到在 3d 层次中的位置索引.

根据 index 找到对应高维位置的 `(h_offset, w_offset, c_offset)`, 之后代码中便是在确认 `data_col` 中的每一个点和 `data_im` 中的每一个点之间的关系, 如下图:

![](http://opoddugn5.bkt.clouddn.com/DownMak/Python/pytorch/offset1.png)

可以看到, `w_im` 与 `w_col` 之间存在如下关系:

```c
int w_im = w_col * stride_w - pad_w + w_offset * dilation_w;
```

需要注意的是上图中最上面的 3 个框分别是 $5*5$ 的大小, 但是 kernel 的大小是 $3*3$, 这是由于 dilation 的缘故, 因此, 在确认稍下面的红点在 image 上的位置时, `w_offset` 需要先乘上 `w_dilation`. (`w_offset` 是在 `kernel_w` 的基础上计算出来的, 所以有个对应关系.)

下一段代码中:

```c
data_col[(c_col * height_col + h_col) * width_col + w_col] =
  (h_im >= 0 && w_im >= 0 && h_im < height && w_im < width) ?
  data_im[(c_im * height + h_im) * width + w_im] : 0;
```

说明, 对于 `data_col` 中的数据, 如果原本是从图像(也就是图中的 image)中获得的, 那么就是图像的原值, 否则就是 padding 的值, 也就是 0.

#### 总结

总结一下, `data_col` 就是通过将图像 `data_im` 上 kernel 覆盖的部分拉成一列而成的, 总共有 `height_col * width_col` 列, 每一列有 `channels_col` 个元素. 情况在加上 padding, 进行 stride 以及将 kernel 进行 dilation 之后变得稍微复杂一些. 关键是明确 `data_col` 中每个元素和原图像 `data_im` 的元素之间的关系.



## THNN_(col2im)

有了前面 im2col 的经验, 下面直接看 col2im 的实现结果:

```c
static void THNN_(col2im)(const real* data_col, const int channels,
      const int height, const int width, const int kernel_h, const int kernel_w,
      const int pad_h, const int pad_w,
      const int stride_h, const int stride_w,
      const int dilation_h, const int dilation_w,
      real* data_im) {
  memset(data_im, 0, sizeof(real) * height * width * channels);
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
        if (h_im >= 0 && h_im < height && w_im >= 0 && w_im < width)
          data_im[(c_im * height + h_im) * width + w_im] +=
            data_col[(c_col * height_col + h_col) * width_col + w_col];
      }
    }
  }
}
```

注意到一开始有:

```c
memset(data_im, 0, sizeof(real) * height * width * channels);
```

将 `data_im` 中的数据都初始化为 0; 之后有:

```c
if (h_im >= 0 && h_im < height && w_im >= 0 && w_im < width)
  data_im[(c_im * height + h_im) * width + w_im] +=
  	data_col[(c_col * height_col + h_col) * width_col + w_col];
```

这里需要注意的地方是, col2im 的结果会将图像中同一个位置的值进行多次相加, 那么如果先将一个图像进行 im2col, 再通过 col2im, 其实是变不回来的. 但是能达到累加的功效.



## THNN_(SpatialFullConvolution_updateOutput)

看到这个函数中输出与输入的大小时, 我就知道自己入了一个坑...这个 Full Convolution 应该是反卷积的实现(或者说类似的...因为最后的输出结果是变大的), 大致看看:

首先注意参数, 更新 output 的话, 需要输入 input, 权重 weight 以及 bias, `kW` 和 `kH` 是 kernel 的大小, `dW` 和 `dH` 是 stride 的大小, `padW` 和 `padH` 是 padding 的大小. 最后的 `adjW` 和 `adjH` 没有见过... 不关心.

+   首先 weight 的大小应该是 `[nInputPlane, nOutputPlane, kH, kW]`, 所以可以使用 `THTensor_(size)` 获得相应的大小. 之后检查输入 input 的维度要符合要求. 默认需要 input 最好是 4d 的, 即 `[batchSize, nInputPlane, input_height, input_width]`. 如果是 3d 的话, 那么会只用 `THTensor_(resize4d)` 转换为 `[1, nInputPlane, input_height, input_width]` 大小. 此时还会设置 `batch = 0`, 用于最后输出时将 4d 的结果重新转换为 3d 的结果.

+   另外是 `outputWidth` 和 `outputHeight` 的大小, 是变大的节奏, 应该是类似于反卷积的效果.

+   `columns` 用于保存 output 的结果的 column 表示(即 im2col), 方便进行矩阵运算(比如后面的 `THBlas_(gemm)`); 而 `ones` 参数用于 bias 的相关计算.

+   根据作者的注释, 查看 http://docs.nvidia.com/cuda/cublas/#cublas-lt-t-gt-gemm 关于矩阵乘法的各参数的意义:

    ```c
    cublasStatus_t cublasSgemm(cublasHandle_t handle, 
                               cublasOperation_t transa, cublasOperation_t transb, 
                               int m, int n, int k, 
                               const float *alpha, 
                               const float *A, int lda, 
                               const float *B, int ldb, 
                               const float *beta, float *C, 
                               int ldc)
    ```

    +   最后的结果为 $C = \alpha\times op(A)op(B) + \beta\times C$ (重新写一遍: `C = alpha*op(A)*op(B) + beta*C`)
    +   其中 op 就是参数 `transa` 与 `transb`, 如果为 `n` 的话, 表示不变化; 如果为 `t`, 表示矩阵需要转置.
    +   m: number of rows of matrix op(A) and C
    +   n: number of columns of matrix op(B) and C.
    +   k: number of columns of op(A) and rows of op(B).
    +   也即是说, `op(A)` 是 `m * k`, `op(B)` 是 `k * n`, `C` 是 `m * n`.
    +   lda 和 ldb, ldc 分别表示: leading dimension of two-dimensional array used to store the matrix A and matrix B and C.

+   注意传入 `THNN_(col2im)` 中的参数. 看一下代码中我的解释.

+   用 `THTensor_(new)` 产生的 Tensor 最后要记得使用 `THTensor_(free)` 释放.

```c
void THNN_(SpatialFullConvolution_updateOutput)(
    THNNState *state,
    THTensor *input,
    THTensor *output,
    THTensor *weight,
    THTensor *bias,
    THTensor *columns,
    THTensor *ones,
    int kW, int kH,
    int dW, int dH,
    int padW, int padH,
    int adjW, int adjH)
{
  int nInputPlane = THTensor_(size)(weight,0);
  int nOutputPlane = THTensor_(size)(weight,1);

  THArgCheck(input->nDimension == 3 || input->nDimension == 4, 2, "3D or 4D (batch mode) tensor is expected");

  int batch = 1;
  if (input->nDimension == 3) {
    THArgCheck(input->size[0] == nInputPlane, 2, "input channels and nInputPlane dont match");
    // Force batch
    batch = 0;
    THTensor_(resize4d)(input, 1, input->size[0], input->size[1], input->size[2]);
  } else {
    THArgCheck(input->size[1] == nInputPlane, 2, "input channels and nInputPlane dont match");
  }

  long inputWidth   = input->size[3];
  long inputHeight  = input->size[2];
  long outputWidth  = (inputWidth - 1) * dW - 2*padW + kW + adjW;
  long outputHeight = (inputHeight - 1) * dH - 2*padH + kH + adjH;

  // Batch size + input planes
  long batchSize = input->size[0];

  // Resize output
  THTensor_(resize4d)(output, batchSize, nOutputPlane, outputHeight, outputWidth);

  // Resize temporary columns
  THTensor_(resize2d)(columns, nOutputPlane*kW*kH, inputHeight*inputWidth);
  THTensor_(zero)(columns);

  // Define a buffer of ones, for bias accumulation
  // Note: this buffer can be shared with other modules, it only ever gets increased,
  // and always contains ones.
  if (ones->nDimension != 2 || ones->size[0]*ones->size[1] < outputHeight*outputWidth) {
    // Resize plane and fill with ones...
    THTensor_(resize2d)(ones, outputHeight, outputWidth);
    THTensor_(fill)(ones, 1);
  }

  // Helpers
  THTensor *input_n = THTensor_(new)();
  THTensor *output_n = THTensor_(new)();

  int elt;
  // For each elt in batch, do:
  for (elt = 0; elt < batchSize; elt ++) {
    // Matrix mulitply per output:
    THTensor_(select)(input_n, input, 0, elt);
    THTensor_(select)(output_n, output, 0, elt);

    // M,N,K are dims of matrix A and B
    // (see http://docs.nvidia.com/cuda/cublas/#cublas-lt-t-gt-gemm)
    long m = weight->size[1] * weight->size[2] * weight->size[3];
    long n = columns->size[1];
    long k = weight->size[0];

    // Do GEMM (note: this is a bit confusing because gemm assumes column-major matrices)
    // n: inputHeight x inputWidth
    // m: nOuputPlane x kH x kW
    // k: nInputPlane
    // 这里可以得到, 
    // input_n 的大小为 nxk: [inputHeight x inputWidth, nInputPlane]
    // trans(weight) 的大小为 k*m: [nInputPlane, nOuputPlane x kH x kW]
    // columns 的大小为 nxm: [inputHeight x inputWidth, nOuputPlane x kH x kW]
    // 可能我的理解有错误, 似乎和前面 columns 的 resize 结果不对应. 但大致如此. 以后需要多用 blas 库.
    THBlas_(gemm)(
        'n', 't',
        n, m, k,
        1,
        THTensor_(data)(input_n), n,
        THTensor_(data)(weight), m,
        0,
        THTensor_(data)(columns), n
    );

    // Unpack columns back into input:
    // 这里输入参数分别是 outputHeight 与 outputWidth, 是因为在 col2im 中, 会有相应的处理, 比如:
    // height_col = (height + 2 * pad_h - kernel_h) / stride_h + 1;
    THNN_(col2im)(
      THTensor_(data)(columns),
      nOutputPlane, outputHeight, outputWidth, kH, kW, padH, padW, dH, dW,
      1, 1,
      THTensor_(data)(output_n)
    );

    // Do Bias after:
    // M,N,K are dims of matrix A and B
    // (see http://docs.nvidia.com/cuda/cublas/#cublas-lt-t-gt-gemm)
    long m_ = nOutputPlane;
    long n_ = outputHeight * outputWidth;
    long k_ = 1;

    // Do GEMM (note: this is a bit confusing because gemm assumes column-major matrices)
    if (bias) {
      THBlas_(gemm)(
          't', 'n',
          n_, m_, k_,
          1,
          THTensor_(data)(ones), k_,
          THTensor_(data)(bias), k_,
          1,
          THTensor_(data)(output_n), n_
      );
    }
  }

  // Free
  THTensor_(free)(input_n);
  THTensor_(free)(output_n);

  // Resize output
  if (batch == 0) {
    THTensor_(resize3d)(output, nOutputPlane, outputHeight, outputWidth);
    THTensor_(resize3d)(input, nInputPlane, inputHeight, inputWidth);
  }
}
```



## THNN_(SpatialFullConvolution_updateGradInput)

下面是反向传播的代码, 更新 `GradInput`, 也就是求当前层的 $\delta$.

首先注意一下传入的参数, 回忆一下 pytorch 的代码风格中规定的内容. 另外, 由于这里是求当前层的 $\delta$, 所以需要前一层的 $\delta$ 的值 `gradOutput`, 以及输入 `input`, 权重 `weight`. `gradColumns` 用于保存 `gradOutput` 的 column 的形式(即前一层的 $\delta$). 对了, 记得 `dW` 和 `dH` 是 stride. 其他参数不多解释.

代码中最为关键的内容是:

```c
THBlas_(gemm)(
        'n', 'n',
        n, m, k,
        1,
        THTensor_(data)(gradColumns), n,
        THTensor_(data)(weight), k,
        0,
        THTensor_(data)(gradInput_n), n
    );
```

即将前一层的 $\delta$ 和当前层的权重做卷积(或者说加权求和...), 从而获得当前层的 $\delta$. 这些内容是反向传播的重点, 不多解释. 具体可以看下我对 tiny-dnn 的解析, 深入了解反向传播: [TinyDnn 源码阅读](https://www.kancloud.cn/ieric_1993/tiny_dnn)

```c
void THNN_(SpatialFullConvolution_updateGradInput)(
    THNNState *state,
    THTensor *input,
    THTensor *gradOutput,
    THTensor *gradInput,
    THTensor *weight,
    THTensor *gradColumns,
    int kW, int kH,
    int dW, int dH,
    int padW, int padH,
    int adjW, int adjH)
{
  int nInputPlane = THTensor_(size)(weight,0);
  int nOutputPlane = THTensor_(size)(weight,1);

  THArgCheck(input->nDimension == 3 || input->nDimension == 4, 2, "3D or 4D (batch mode) tensor is expected");

  int batch = 1;
  if (input->nDimension == 3) {
    // Force batch
    batch = 0;
    THTensor_(resize4d)(input, 1, input->size[0], input->size[1], input->size[2]);
    THTensor_(resize4d)(gradOutput, 1, gradOutput->size[0], gradOutput->size[1], gradOutput->size[2]);
  }

  long inputWidth   = input->size[3];
  long inputHeight  = input->size[2];
  long outputWidth  = (inputWidth - 1) * dW - 2*padW + kW + adjW;
  long outputHeight = (inputHeight - 1) * dH - 2*padH + kH + adjH;

  // Batch size + input planes
  long batchSize = input->size[0];

  // Resize output
  THTensor_(resize4d)(gradInput, batchSize, nInputPlane, inputHeight, inputWidth);
  THTensor_(zero)(gradInput);

  // Resize temporary columns
  THTensor_(resize2d)(gradColumns, nOutputPlane*kW*kH, inputHeight*inputWidth);

  // Helpers
  THTensor *gradInput_n = THTensor_(new)();
  THTensor *gradOutput_n = THTensor_(new)();

  int elt;
  // For each elt in batch, do:
  for (elt = 0; elt < batchSize; elt ++) {
    // Matrix mulitply per sample:
    THTensor_(select)(gradInput_n, gradInput, 0, elt);
    THTensor_(select)(gradOutput_n, gradOutput, 0, elt);

    // Extract columns:
    THNN_(im2col)(
      THTensor_(data)(gradOutput_n),
      nOutputPlane, outputHeight, outputWidth, kH, kW, padH, padW, dH, dW,
      1, 1,
      THTensor_(data)(gradColumns)
    );


    // M,N,K are dims of matrix A and B
    // (see http://docs.nvidia.com/cuda/cublas/#cublas-lt-t-gt-gemm)
    long m = weight->size[0];
    long n = gradColumns->size[1];
    long k = weight->size[1] * weight->size[2] * weight->size[3];

    // Do GEMM (note: this is a bit confusing because gemm assumes column-major matrices)
    THBlas_(gemm)(
        'n', 'n',
        n, m, k,
        1,
        THTensor_(data)(gradColumns), n,
        THTensor_(data)(weight), k,
        0,
        THTensor_(data)(gradInput_n), n
    );
  }


  // Free
  THTensor_(free)(gradInput_n);
  THTensor_(free)(gradOutput_n);

  // Resize output
  if (batch == 0) {
    THTensor_(resize3d)(gradOutput, nOutputPlane, outputHeight, outputWidth);
    THTensor_(resize3d)(input, nInputPlane, inputHeight, inputWidth);
    THTensor_(resize3d)(gradInput, nInputPlane, inputHeight, inputWidth);
  }
}
```



## THNN_(SpatialFullConvolution_accGradParameters)

这是该文件最后一个函数了, 欧耶... 用于求权重(包括 bias)的梯度.

仍然是反向传播的内容, 此时注意一下该函数的参数, 由于需要求权重的梯度, 就需要前一层的 $\delta$ 值 `gradOutput`, 当前层的输入 input; 我们需要求取 `gradWeight` 以及 `gradBias`. `columns` 仍然是保存 `gradOutput` 的 column 形式. 最为重点的代码是:

```c
THBlas_(gemm)(
        't', 'n',
        n, m, k,
        scale,
        THTensor_(data)(columns), k,
        THTensor_(data)(input_n), k,
        1,
        THTensor_(data)(gradWeight), n
    );
```

注意到两点变化. 首先要明确权重的梯度是前一层的 $\delta$ 和当前层输入的卷积, 那么 `THBlas_(gemm)` 中的 `beta` 参数便设置为 1, 这样的话, `gradWeight` 是会被累加的.

第二, 引入了参数 scale. 另外注意一下 `t` 和 `n`.

还有, 求 `gradBias` 用的是: `THBlas_(gemv)`:

```c
THBlas_(gemv)(
          't',
          k_, m_,
          scale,
          THTensor_(data)(gradOutput_n), k_,
          THTensor_(data)(ones), 1,
          1,
          THTensor_(data)(gradBias), 1
      );
```

实际上就是 `gradOutput` 的累加.

```c
void THNN_(SpatialFullConvolution_accGradParameters)(
    THNNState *state,
    THTensor *input,
    THTensor *gradOutput,
    THTensor *gradWeight,
    THTensor *gradBias,
    THTensor *columns,
    THTensor *ones,
    int kW, int kH,
    int dW, int dH,
    int padW, int padH,
    int adjW, int adjH,
    real scale)
{
  int nInputPlane = THTensor_(size)(gradWeight,0);
  int nOutputPlane = THTensor_(size)(gradWeight,1);

  THArgCheck(input->nDimension == 3 || input->nDimension == 4, 2, "3D or 4D (batch mode) tensor is expected");

  int batch = 1;
  if (input->nDimension == 3) {
    // Force batch
    batch = 0;
    THTensor_(resize4d)(input, 1, input->size[0], input->size[1], input->size[2]);
    THTensor_(resize4d)(gradOutput, 1, gradOutput->size[0], gradOutput->size[1], gradOutput->size[2]);
  }

  long inputWidth   = input->size[3];
  long inputHeight  = input->size[2];
  long outputWidth  = (inputWidth - 1) * dW - 2*padW + kW + adjW;
  long outputHeight = (inputHeight - 1) * dH - 2*padH + kH + adjH;

  // Batch size + input planes
  long batchSize = input->size[0];

  // Define a buffer of ones, for bias accumulation
  if (ones->nDimension != 2 || ones->size[0]*ones->size[1] < outputHeight*outputWidth) {
    // Resize plane and fill with ones...
    THTensor_(resize2d)(ones, outputHeight, outputWidth);
    THTensor_(fill)(ones, 1);
  }

  // Resize temporary columns
  THTensor_(resize2d)(columns, nOutputPlane*kW*kH, inputHeight*inputWidth);

  // Helpers
  THTensor *input_n = THTensor_(new)();
  THTensor *gradOutput_n = THTensor_(new)();

  int elt;
  // For each elt in batch, do:
  for (elt = 0; elt < batchSize; elt ++) {
    // Matrix mulitply per output:
    THTensor_(select)(input_n, input, 0, elt);
    THTensor_(select)(gradOutput_n, gradOutput, 0, elt);

    // Extract columns:
    THNN_(im2col)(
      THTensor_(data)(gradOutput_n),
      nOutputPlane, outputHeight, outputWidth, kH, kW, padH, padW, dH, dW,
      1, 1,
      THTensor_(data)(columns)
    );

    // M,N,K are dims of matrix A and B
    // (see http://docs.nvidia.com/cuda/cublas/#cublas-lt-t-gt-gemm)
    long n = columns->size[0];   // nOutputPlane * kh * kw
    long m = input_n->size[0];   // nInputPlane
    long k = columns->size[1];   // inputHeight * inputWidth

    // Do GEMM (note: this is a bit confusing because gemm assumes column-major matrices)
    THBlas_(gemm)(
        't', 'n',
        n, m, k,
        scale,
        THTensor_(data)(columns), k,
        THTensor_(data)(input_n), k,
        1,
        THTensor_(data)(gradWeight), n
    );


    // Do Bias:
    // M,N,K are dims of matrix A and B
    // (see http://docs.nvidia.com/cuda/cublas/#cublas-lt-t-gt-gemm)
    long m_ = nOutputPlane;
    long k_ = outputHeight * outputWidth;

    // Do GEMV (note: this is a bit confusing because gemv assumes column-major matrices)
    if (gradBias) {
      THBlas_(gemv)(
          't',
          k_, m_,
          scale,
          THTensor_(data)(gradOutput_n), k_,
          THTensor_(data)(ones), 1,
          1,
          THTensor_(data)(gradBias), 1
      );
    }
  }

  // Free
  THTensor_(free)(input_n);
  THTensor_(free)(gradOutput_n);

  // Resize
  if (batch == 0) {
    THTensor_(resize3d)(gradOutput, nOutputPlane, outputHeight, outputWidth);
    THTensor_(resize3d)(input, nInputPlane, inputHeight, inputWidth);
  }
}
```

