# Delving Deep into Rectifiers: Surpassing Human-Level Performance on ImageNet Classification

## 2017 年 5 月 11 日

### Delving Deep into Rectifiers: Surpassing Human-Level Performance on ImageNet Classification

文章地址: [https://arxiv.org/abs/1502.01852](https://arxiv.org/abs/1502.01852)

Kaiming He 大神的大作, 就不多解释了. 由于不做分类, 文章我只查找我感兴趣的内容. 

+   文章介绍了 PReLU, 其定义为: 

    $$f(y_i) = \left\{\begin{array}{ll}y_i, & \text{if $y_i > 0$ } \\ a_iy_i, & \text{if $y_i \leq 0$}\end{array}\right.$$

    ​

    该定义等价于 $f(y_i) = \max(0, y_i) + a_i\min(0, y_i)$. (软阈值也是这样写的)

+   Initialization of Filter Weights 

    +   本文提出的 weight initialization 是 $\sqrt{\frac{2}{f_{out} \times h \times w}}$ . Caffe 中有专门的定义: msra ([http://caffe.berkeleyvision.org/doxygen/classcaffe_1_1MSRAFiller.html](http://caffe.berkeleyvision.org/doxygen/classcaffe_1_1MSRAFiller.html)) (msra 的意思为 microsoft research aisa, 本文的公司就是微软)
    +   Glorot and Bengio[7] 提出的是 Xavier Initialization(该称呼见 [14]). 本文说到: Its derivation is based on the assumption that the activations are linear. This assumption is invalid for ReLU and PReLU.