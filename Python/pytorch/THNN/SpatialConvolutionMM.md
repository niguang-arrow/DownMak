# SpatialConvolutionMM

## 2018 年 1 月 2 日

昨天和今天介绍了 `SpatialFullConvolution.c` 文件的内容. 今天再介绍一下 `SpatialConvolutionMM.c` 的内容. 这个文件应该就是用来做 `nn.Conv2d` 的底层代码, 该文件位于 https://github.com/pytorch/pytorch/blob/v0.1.1/torch/lib/THNN/generic/SpatialConvolutionMM.c



## THNN_(SpatialConvolutionMM_updateOutput_frame)

首先要注意到这个函数是一个 static 函数, 因此只能被本文件内的函数访问. 真正用于计算 Output 的是下面要介绍的 `THNN_(SpatialConvolutionMM_updateOutput)`.

我大致浏览了这个文件中关于 output 的实现, 终于意识到, 这和 Caffe 实现卷积的方式是一致的. Caffe 实现卷积的原理可以参考 [在 Caffe 中如何计算卷积？](https://www.zhihu.com/question/28385679) 另外, 在论文 [*High Performance Convolutional Neural Networks for Document Processing*](https://hal.archives-ouvertes.fr/file/index/docid/112631/filename/p1038112283956.pdf) 也给出了相应的介绍 (这篇论文中的图画得好啊) 另外还可以看下博客 [[Caffe的卷积原理](http://www.cnblogs.com/yymn/articles/5330587.html)](http://www.cnblogs.com/yymn/articles/5330587.html). 而且, 我在 `DownMak/Cpp/Experiment/Caffe/Caffe_Conv.md` 中还介绍了一下 Caffe 的卷积.

下面看看代码:

+ 这里让人疑惑的是 `finput` 的参数, 其他的参数都是计算所必须的. `finput` 是将输入展开成 col 等形式(联想一下 Caffe 对卷积的处理), 其大小为 `[kW*kW*nInputPlane, outputHeight*outputWidth]`. 这些内容需要看下面将要介绍的 `THNN_(SpatialConvolutionMM_updateOutput)` 可知.
+ 假设网络的输入大小为 `[batchSize, nInputPlane, inputHeight, inputWidth]`, 那么 `THNN_(SpatialConvolutionMM_updateOutput_frame)` 就是将其中某一个输入 `[1, nInputPlane, inputHeight, inputWidth]` 进行展开成 finput 的形式(联想 Caffe 卷积), 然后再使用 `THTensor_(addmm)` 进行卷积运算. 
+ 总结一下, 也就是说, 这个函数实际上完成了卷积的操作, 是实际进行卷积的函数, 之后的 `THNN_(SpatialConvolutionMM_updateOutput)` 即是调用该函数进行整个 batch 的卷积处理的.

```c
static void THNN_(SpatialConvolutionMM_updateOutput_frame)(
          THTensor *input,
          THTensor *output,
          THTensor *weight,
          THTensor *bias,
          THTensor *finput,
          int kW,
          int kH,
          int dW,
          int dH,
          int padW,
          int padH,
          long nInputPlane,
          long inputWidth,
          long inputHeight,
          long nOutputPlane,
          long outputWidth,
          long outputHeight)
{
  long i;
  THTensor *output2d;
  
  // THNN_(unfolded_copy) 的具体细节不管, 实现比较复杂, 
  // 该函数定义在 THNN/generic/unfold.c 文件中
  // 该函数的作用是将 input 中的内容复制到 finput 中, 也就是将 input 展开成 col 的形式.
  // caffe 主要就是使用了 im2col, 然后在使用 gemm 之类的 blas 库完成卷积计算.
  THNN_(unfolded_copy)(finput, input, kW, kH, dW, dH, padW, padH, nInputPlane, inputWidth, inputHeight, outputWidth, outputHeight);

  output2d = THTensor_(newWithStorage2d)(output->storage, output->storageOffset,
                                         nOutputPlane, -1,
                                         outputHeight*outputWidth, -1);
  if (bias) {
    for(i = 0; i < nOutputPlane; i++)
        THVector_(fill)(output->storage->data+output->storageOffset+output->stride[0]*i, THTensor_(get1d)(bias, i), outputHeight*outputWidth);
  } else {
    THTensor_(zero)(output);
  }
  
  // 卷积, 这里计算了一个 batch 中某一个输入 finput 与权重的卷积
  THTensor_(addmm)(output2d, 1, output2d, 1, weight, finput);

  THTensor_(free)(output2d);
}
```



## THNN_(SpatialConvolutionMM_updateOutput)

这个函数是真正的接口, 毕竟函数定义中没有 `static` 关键字. 在该函数中, 主要做输入参数尺寸的确认, 以满足卷积的要求; 之后根据输入 input 是 3d 的还是 4d 的分别处理, 如果是 4d 的, 那么使用多线程对 batch 中的每个输入使用前面定义的 `THNN_(SpatialConvolutionMM_updateOutput_frame)` 进行处理. 代码如下:

+ 注意到代码中有 `#pragma omp parallel for private(t)` 这样一段代码, 经查阅, 使用的 OpenMP 的语法, 进行多线程计算. `private(t)` 表示 t 是每个线程的局部变量. 关于 OpenMP 可以参考 http://www.eng.utah.edu/~cs4960-01/lecture9.pdf 这份 ppt. 另外还可以到官网上查看相关内容: http://www.openmp.org/

+ 另外留一个问题思考: 

  注意下面一句代码: `nOutputPlane = weight->size[0];` 也就是说这段代码中指明了 weight 的大小为 `[nOutputPlane, nInputPlane, kH, kW]`. 但是在 `SpatialFullConvolution.md` 中, 我说 weight 的大小为 `[nInputPlane, nOutputPlane, kH, kW]`, 在 `nn.Conv2d` 是前一种情况. 那么这是为何...?

```c
void THNN_(SpatialConvolutionMM_updateOutput)(
          THNNState *state,
          THTensor *input,
          THTensor *output,
          THTensor *weight,
          THTensor *bias,
          THTensor *finput,
          THTensor *fgradInput,
          int kW,
          int kH,
          int dW,
          int dH,
          int padW,
          int padH)
{
  int dimf = 0;
  int dimw = 2;
  int dimh = 1;

  long nInputPlane;
  long inputWidth;
  long inputHeight;
  long nOutputPlane;
  long outputWidth;
  long outputHeight;

  int freeWeight = 0;

  THArgCheck( input->nDimension == 3 || input->nDimension == 4, 2, "3D or 4D (batch mode) tensor expected");
  THArgCheck(kW > 0 && kH > 0, 8, "kernel size should be greater than zero");
  THArgCheck(dW > 0 && dH > 0, 10, "stride should be greater than zero");
  THArgCheck(weight->nDimension == 2 || weight->nDimension == 4, 4, "weight tensor should be 2D or 4D");

  if (input->nDimension == 4) {
    dimf++;
    dimw++;
    dimh++;
  }

  nInputPlane = input->size[dimf];
  inputWidth   = input->size[dimw];
  inputHeight  = input->size[dimh];
  nOutputPlane = weight->size[0];
  outputWidth  = (inputWidth + 2*padW - kW) / dW + 1;
  outputHeight = (inputHeight + 2*padH - kH) / dH + 1;

  if (outputWidth < 1 || outputHeight < 1)
    THError("Given input size: (%dx%dx%d). Calculated output size: (%dx%dx%d). Output size is too small",
        nInputPlane,inputHeight,inputWidth,nOutputPlane,outputHeight,outputWidth);


  int expectedWeightSize = weight->nDimension == 2 ? nInputPlane*kW*kH : nInputPlane;
  int weightInputPlanes = weight->nDimension == 2 ? weight->size[1]/(kW*kH) : weight->size[1];
  if (expectedWeightSize != weight->size[1])
    THError("Wrong number of input channels! Input has %d channels, expected %d",
        nInputPlane, weightInputPlanes);

  if (weight->nDimension == 4) {
    long s1 = weight->size[0];
    long s2 = weight->size[1] * weight->size[2] * weight->size[3];
    weight = THTensor_(newWithStorage2d)(weight->storage, 0, s1, -1, s2, -1);
    freeWeight = 1;
  }

  if(input->nDimension == 3)
  {
    // 注意到 finput 的大小, 结合 THNN_(SpatialConvolutionMM_updateOutput_frame) 中的
    // 内容理解.
    THTensor_(resize2d)(finput, kW*kH*nInputPlane, outputHeight*outputWidth);
    THTensor_(resize3d)(output, nOutputPlane, outputHeight, outputWidth);

    THNN_(SpatialConvolutionMM_updateOutput_frame)(input, output, weight, bias, finput,
                                                 kW, kH, dW, dH, padW, padH,
                                                 nInputPlane, inputWidth, inputHeight,
                                                 nOutputPlane, outputWidth, outputHeight);
  }
  else
  {
    long T = input->size[0];
    long t;

    THTensor_(resize3d)(finput, T, kW*kH*nInputPlane, outputHeight*outputWidth);
    THTensor_(resize4d)(output, T, nOutputPlane, outputHeight, outputWidth);

    //使用 omp, 利用多线程对 batch(T) 中每个输入进行卷积操作.
#pragma omp parallel for private(t)
    for(t = 0; t < T; t++)
    {
      THTensor *input_t = THTensor_(newSelect)(input, 0, t);
      THTensor *output_t = THTensor_(newSelect)(output, 0, t);
      THTensor *finput_t = THTensor_(newSelect)(finput, 0, t);

      THNN_(SpatialConvolutionMM_updateOutput_frame)(input_t, output_t, weight, bias, finput_t,
                                                   kW, kH, dW, dH, padW, padH,
                                                   nInputPlane, inputWidth, inputHeight,
                                                   nOutputPlane, outputWidth, outputHeight);

      THTensor_(free)(input_t);
      THTensor_(free)(output_t);
      THTensor_(free)(finput_t);
    }
  }

  if (freeWeight)
    THTensor_(free)(weight);
}
```



## THNN_(SpatialConvolutionMM_updateGradInput_frame)

处理完 output 之后, 下面更新 gradInput, 也就是求前一层的 $\delta$. 另外注意该函数是 static 的, 和前面一样, 应该是用于一个输入的处理.

+ `gradOutput` 为当前层的 $\delta$. 使用 `THTensor_(addmm)` 令权重和当前层的 $\delta$ 进行卷积即可求出 `gradOutput`.

```c
static void THNN_(SpatialConvolutionMM_updateGradInput_frame)(
          THTensor *gradInput,
          THTensor *gradOutput,
          THTensor *weight,
          THTensor *fgradInput,
          int kW,
          int kH,
          int dW,
          int dH,
          int padW,
          int padH)
{
  THTensor *gradOutput2d = THTensor_(newWithStorage2d)(gradOutput->storage, gradOutput->storageOffset,
                                                       gradOutput->size[0], -1,
                                                       gradOutput->size[1]*gradOutput->size[2], -1);
  THTensor_(addmm)(fgradInput, 0, fgradInput, 1, weight, gradOutput2d);
  THTensor_(free)(gradOutput2d);

  THTensor_(zero)(gradInput);

  THNN_(unfolded_acc)(fgradInput, gradInput, kW, kH, dW, dH, padW, padH, gradInput->size[0], gradInput->size[2], gradInput->size[1], gradOutput->size[2], gradOutput->size[1]);
}
```



## THNN_(SpatialConvolutionMM_updateGradInput)

实际的 API. 这个... 有了前面的基础, 可以了解个大概了.

```c
void THNN_(SpatialConvolutionMM_updateGradInput)(
          THNNState *state,
          THTensor *input,
          THTensor *gradOutput,
          THTensor *gradInput,
          THTensor *weight,
          THTensor *finput,
          THTensor *fgradInput,
          int kW,
          int kH,
          int dW,
          int dH,
          int padW,
          int padH)
{
  long nOutputPlane = weight->size[0];
  int freeWeight = 0;

  THArgCheck( nOutputPlane == gradOutput->size[input->nDimension == 4 ? 1 : 0], 3, "Number of output features is not equal to nOutputPlane" );
  THArgCheck(kW > 0 && kH > 0, 9, "kernel size should be greater than zero");
  THArgCheck(dW > 0 && dH > 0, 11, "stride should be greater than zero");
  THArgCheck(weight->nDimension == 2 || weight->nDimension == 4, 4, "weight tensor should be 2D or 4D");

  THTensor_(resizeAs)(gradInput, input);
  THTensor_(resizeAs)(fgradInput, finput);
  // depending on the BLAS library, fgradInput (result tensor) might
  // be left uninitialized on zero alpha, which might lead to weird behavior
  // hence, to be safe, zero it
  THTensor_(zero)(fgradInput);

  if (weight->nDimension == 4) {
    long s1 = weight->size[0];
    long s2 = weight->size[1] * weight->size[2] * weight->size[3];
    weight = THTensor_(newWithStorage2d)(weight->storage, 0, s1, -1, s2, -1);
    freeWeight = 1;
  }

  THTensor_(transpose)(weight, weight, 0, 1);

  if(input->nDimension == 3)
  {
    THNN_(SpatialConvolutionMM_updateGradInput_frame)(gradInput, gradOutput, weight, fgradInput, kW, kH, dW, dH, padW, padH);
  }
  else
  {
    long T = input->size[0];
    long t;

#pragma omp parallel for private(t)
    for(t = 0; t < T; t++)
    {
      THTensor *gradInput_t = THTensor_(newSelect)(gradInput, 0, t);
      THTensor *gradOutput_t = THTensor_(newSelect)(gradOutput, 0, t);
      THTensor *fgradInput_t = THTensor_(newSelect)(fgradInput, 0, t);

      THNN_(SpatialConvolutionMM_updateGradInput_frame)(gradInput_t, gradOutput_t, weight, fgradInput_t, kW, kH, dW, dH, padW, padH);

      THTensor_(free)(gradInput_t);
      THTensor_(free)(gradOutput_t);
      THTensor_(free)(fgradInput_t);
    }
  }

  THTensor_(transpose)(weight, weight, 0, 1);

  if (freeWeight)
    THTensor_(free)(weight);
}
```



