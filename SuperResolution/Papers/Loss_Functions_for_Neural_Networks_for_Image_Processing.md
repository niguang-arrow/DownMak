# Loss Functions for Neural Networks for Image Processing

## 2017 年 7 月 23 日

### Loss Functions for Neural Networks for Image Processing

文章作者: Hang Zhao, Orazio Gallo, Iuri Frosio, and Jan Kautz

文章地址: https://arxiv.org/abs/1511.08861

项目地址: [http://www.mit.edu/~hangzhao/](http://www.mit.edu/~hangzhao/)

文章代码: [https://github.com/NVlabs/PL4NN](https://github.com/NVlabs/PL4NN)



## 3. Loss Layers for image processing

### 3.1 The l1 error

$$
\mathcal{L}^{\ell_1} (P) = \frac{1}{N}\sum_{p\in P}\vert x(p) - y(p)\vert
$$

其中 $p$ 为像素的 index, $P$ 为 patch, $x(p)$ 与 $y(p)$ 分别为正在处理的 patch 的像素值以及 ground truth 的像素值. 反向传播的导数, 由于 $\partial\mathcal{L}^{\ell_1}(p)/\partial q = 0, \forall q\neq p$, 那么对于 patch 中的每一个像素 $p$, 有;
$$
\partial\mathcal{L}^{\ell_1}(p)/\partial x(p) = sign\left(x\left(p\right) - y\left(p\right)\right)
$$
代码如下:https://github.com/NVlabs/PL4NN/blob/master/src/loss.py

```python
import numpy as np
import scipy.signal
import caffe

class L2(caffe.Layer):
    "A loss layer that calculates Mean-Squared-Error loss"
    def setup(self, bottom, top):
        if len(bottom) != 2:
            raise Exception("Need two inputs to compute distance.")

    def reshape(self, bottom, top):
        # check input dimensions match
        if bottom[0].count != bottom[1].count:
            raise Exception("Inputs must have the same dimension.")
        # difference is shape of inputs
        self.diff = np.zeros_like(bottom[0].data, dtype=np.float32)
        # loss output is scalar
        top[0].reshape(1)

    def forward(self, bottom, top):
        self.diff[...] = bottom[0].data - bottom[1].data
        top[0].data[...] = np.sum(self.diff**2) / bottom[0].count / 2.

    def backward(self, top, propagate_down, bottom):
        for i in range(2):
            if not propagate_down[i]:
                continue
            if i == 0:
                sign = 1
            else:
                sign = -1
            bottom[i].diff[...] = sign * self.diff / bottom[i].count

class L1(caffe.Layer):
    "A loss layer that calculates Mean-Absolute-Error loss"
    def setup(self, bottom, top):
        if len(bottom) != 2:
            raise Exception("Need two inputs to compute distance.")

    def reshape(self, bottom, top):
        # check input dimensions match
        if bottom[0].count != bottom[1].count:
            raise Exception("Inputs must have the same dimension.")
        # difference is shape of inputs
        self.diff = np.zeros_like(bottom[0].data, dtype=np.float32)
        # loss output is scalar
        top[0].reshape(1)

    def forward(self, bottom, top):
        self.diff[...] = bottom[0].data - bottom[1].data
        top[0].data[...] = np.sum(np.abs(self.diff)) / bottom[0].count

    def backward(self, top, propagate_down, bottom):

	    # get the sign
        diff_sign = np.sign(self.diff)

        for i in range(2):
            if not propagate_down[i]:
                continue
            if i == 0:
                sign = 1
            else:
                sign = -1
            bottom[i].diff[...] = sign * diff_sign / bottom[i].count 
```

L1 Loss 对 $y(p)$ 进行求导时要注意还要带上一个负号, 所以最后有 `sign = -1`.

同时从代码中也可以看出, 对于 L1 loss 在 `0` 处求导, 结果就是 0, 因为此时 `diff_sign` 为 0.

### 3.2 SSIM

文章还有对 SSIM, MS-SSIM, MS-SSIM + $\ell_1$ 的分析, 相应的代码也可以在网站中找到.



## 一点想法

+   图像去噪可以不使用噪声大小先验
+   ​

### Phrase

+   poor performance
+   local minima


+   de facto [di:'fæktəu] standard   事实上的标准 (法语)
+   In the span of just a couple of years 在短短的几年时间里

### Vocabulary 

+   virtually  [ˈvəːtʃʊəli]   adv. 事实上，几乎；实质上
    -   The impact of the loss layer of neural networks, however, has not received much attention in the context of image processing: the default and virtually only choice is $\ell_2$.
+   exponentially  [ˌekspəʊˈnenʃəlɪ]  美 [,ɛkspo'nɛnʃəli]  adv. 以指数方式
    -   The popularity of neural networks in the field of computer vision has grown exponentially only recently, when deep learning boosted their performance in image recognition.
+   demosaicking  去马赛克