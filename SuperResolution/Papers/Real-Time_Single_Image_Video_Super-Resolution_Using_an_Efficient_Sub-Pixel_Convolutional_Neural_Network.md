# Real-Time Single Image and Video Super-Resolution Using an Efficient Sub-Pixel Convolutional Neural Network

## 2017 年 7 月 19 日

### Real-Time Single Image and Video Super-Resolution Using an Efficient Sub-Pixel Convolutional Neural Network

文章作者: Wenzhe Shi, Jose Caballero, Ferenc Huszar, Johannes Totz, Andrew P. Aitken,
Rob Bishop, Daniel Rueckert, Zehan Wang

文章地址: https://arxiv.org/abs/1609.05158

文章主页: https://syncedreview.com/2017/04/16/real-time-single-image-and-video-super-resolution-using-an-efficient-sub-pixel-convolutional-neural-network/

论文补充材料: https://twitter.app.box.com/s/47bhw60d066imhh88i2icqnbu7lwiza2

文章代码: https://github.com/pytorch/examples/tree/master/super_resolution

视频测试数据集 

+   (Xiph 数据集): https://media.xiph.org/video/derf/
+   Ultra Video Group 数据集: http://ultravideo.cs.tut.fi/



本文提出 ESPCN 算法 (efficient sub-pixel convolutional neural network)



## 2. Method

+   关于SISR 的目标本文是这样描述的: The task of SISR is to estimate a HR image $I^{SR}$ given a LR image $I^{LR}$ downscaled from the corresponding original HR image $I^{HR}$. The downsampling operation is deterministic and known: to produce $I^{LR}$ from $I^{HR}$, we first convolve $I^{HR}$ using a Gaussian filter - thus simulating the camera's point spread function - then downsample the image by a factor of $r$. We will refer to $r$ as the upscaling ratio. In general, both $I^{LR}$ and $I^{HR}$ can have $C$ color cahnnels, thus they are represented as real-valued tensors of size $H\times W\times C$ and $rH\times rW\times C$, respectively.



