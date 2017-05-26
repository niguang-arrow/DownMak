# Deep Networks for Image Super-Resolution with Sparse Prior

## 2017 年 5 月 25 日

### Deep Networks for Image Super-Resolution with Sparse Prior

文章主页: [http://www.ifp.illinois.edu/~dingliu2/iccv15/](http://www.ifp.illinois.edu/~dingliu2/iccv15/)

下载地址: [https://arxiv.org/abs/1507.08905](https://arxiv.org/abs/1507.08905)

注意这是发表在 2015 年 ICCV 上的会议文章, 作者在 2016 年将相关的工作发表在 TIP 上, 名字为: **Robust Single Image Super-Resolution via Deep Networks with Sparse Prior**, 可以从下面的地址下载.[http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=7466062](http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=7466062) 

## 注

这篇文章看完了, 但是当时没有及时做笔记, 每几天就忘得差不多了, 网络结构也没记住....真不知道我在干什么...因此做笔记应该及时. 这里把我认为重要的东西先记下, 比如作者对基于 SC 方法的 SR 进行了简介, 我觉得写得非常棒. 以后我相信还会回顾这篇文章的内容的, 因为它在 CNN 中考虑了 Sparse Prior; 同时要深刻理解这篇文章的内容, 应该抽时间看看代码.

#### Related Work

+   Image SR Using Sparse Coding

    +   基于稀疏表示的 SR 方法是对 LR 图像到 HR 图像的变换进行建模. 更为精确的描述在文章的 2.1 节详细说明了, 如下:

        >   The sparse representation based SR method[36] **models** the transform from each local patch $y\in\mathbb{R}^{m_y}$ in the bicubic-upscaled LR image to the corresponding patch $x\in\mathbb{R}^{m_x}$ in the HR image. The dimension $m_y$ is not necessarily the same as $m_x$ when image features other than raw pixel is used to represent patch $y$. It is assumed that the LR(HR) patch $y$($x$) can be represented with respect to an overcomplete dictionary $D_y$($D_x$) using some sparse linear coefficients $\alpha_y$($\alpha_x$)$\in\mathbb{R}^{n}$, which  are known as sparse code. Since the degradation process from $x$ to $y$ is nearly linear, the patch pair can share the same sparse code $\alpha_y = \alpha_x = \alpha$ if the dictionaries $D_y$ and $D_x$ are defined properly. Therefore, for an input LR patch $y$, the HR patch can be recovered as $$x = D_x\alpha, \text{  } s.t. \text{  } \alpha = \min_{z}\Vert y - D_y z\Vert_2^2 + \lambda\Vert z\Vert_1$$, where $\Vert\cdot\Vert_1$ denotes the $\ell_1$ norm which is convex and sparcity-inducing, and $\lambda$ is a regularization coefficient. The dictionary pair $(D_y, D_x)$ can be learned alternatively with the inference of training patches' sparse codes in their joint space[36] or through bi-level optimization[35].

    +   也就是要注意到对 HR 图像做 downsampling(degradation) 生成 LR 图像这个过程是线性的, 比如使用 Gaussian 或 Bicubic kernel 进行处理(应该就是卷积了...). 所以如果对 HR 以及 LR 对应的字典好好选择的话, 那么可以认为各自对应的稀疏表示系数相等.

    +   上面的 [35] 和 [36] 都是 Jianchao Yang 的文章:

        +   [35] J. Yang, Z.Wang, Z. Lin, S. Cohen, and T. Huang. Coupled dictionary training for image super-resolution. IEEE Transactions on Image Processing, 21(8):3467–3478, 2012. 1, 2, 4, 5, 6, 7, 8
        +   [36] J. Yang, J. Wright, T. Huang, and Y. Ma. Image super- resolution via sparse representation. IEEE Transactions on Image Processing, 19(11):1–8, 2010. 1, 2, 3

    +   sparsity-inducing  稀疏诱导: $\Vert\cdot\Vert_1$ denotes the $\ell_1$ norm which is convex and sparcity-inducing, and $\lambda$ is a regularization coefficient.

#### phrase

+   It is assumed that 


+   unless otherwise stated  除非另有说明
+   be attributed to         归因于

#### vocabulary

+   degradation [,degrə'deɪʃ(ə)n]  n. 退化, 降格, 降级
    +   Since the degradation process from $x$(HR) to $y$(LR) is nearly linear, ...


+   overshadow  [əʊvə'ʃædəʊ] vt 使失色, 使阴暗, 夺取...的光彩
    +   For image super-resolution, serveral models based on deep neural networks have been recently proposed and attained superior performance that *overshadows* all previous handcrafted models.
+   outperform   vt. 胜过, 做得比...好