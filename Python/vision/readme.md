# vision

## 2017 年 11 月 26 日

### Transforms

+   ToTensor

    +   将 PIL.Image 或 numpy.ndarray (H, W, C) 范围为 (0, 255) 转换为大小为 [C, H, W] 范围为 (0.0, 1.0) 的 FloatTensor.
    +   PIL.Image 对象有名为 `tobytes()` 的方法, 可以获得图像在内存中的 raw data, torch 使用 `torch.ByteStorage.from_buffer` 转换为 ByteStorage 对象:

    ```python
    # pic 为 PIL.Image 对象.
    img = torch.ByteTensor(torch.ByteStorage.from_buffer(pic.tobytes()))
    img = img.view(pic.size[1], pic.size[0], nchannel)
    # put it from HWC to CHW format
    img = img.transpose(0, 1).transpose(0, 2).contiguous()
    ```

    ​

+   ToPILImage

    +   将大小为 (C, H, W) 的 Tensor 或 (H, W, C) 的 numpy.ndarray 转换为 PIL.Image, 注意, numpy.ndarray 始终默认为 [0, 255] 范围.
    +   首先先判断是不是 FloatTensor, 如果是的话, 一方面要将范围从 [0.0, 1.0] 转化到 [0, 255], 另一方面, 要将浮点数转换为整型. 然后使用 `is_tensor` 判断是否是 tensor, 因为如果是 numpy.ndarray 的话, 那就不需要进行 transpose. 最后, 对于 Tensor 对象, 调用 numpy() 方法转换为 numpy.ndarray.

    ```python
    if isinstance(pic, torch.FloatTensor):
        pic = pic.mul(255).byte()
    if torch.is_tensor(pic):
    	npimg = np.transpose(pic.numpy(), (1, 2, 0))
    ```

    总之先转换为 numpy.ndarray, 最后使用 `Image.fromarray` 方法转换为 PIL.Image, 注意根据 numpy.ndarray 中 C 的大小选择合适的 mode.

    ```python
    Image.fromarray(npimg, mode=mode)
    ```

+   Normalize

    +   总结一下, 细节见下方: mean 或 std 中的值会分别被每个通道处理.

    ```python
    class Normalize(object):
        """Given mean: (R, G, B) and std: (R, G, B),
        will normalize each channel of the torch.*Tensor, i.e.
        channel = (channel - mean) / std
        """

        def __init__(self, mean, std):
            self.mean = mean
            self.std = std

        def __call__(self, tensor):
            # TODO: make efficient
            for t, m, s in zip(tensor, self.mean, self.std):
                t.sub_(m).div_(s)
            return tensor
    ```

    mean 或 std, 可以是大小为 (3L,) 的 Tensor, 分别表示 (R, G, B) 的均值, 比如下面的例子: 注意到, a 中的第一列(从 0 到 24) 分别减去的是 1, 第二列(从 1 到 25) 分别减去的是 2, 第三列分别减去 3. 要知道 a 表示的意义可是 (C, H, W), 可以把 (3L,) 理解为 (1L, 3L), 这样, 由于 a 是 (3, 3, 3), 那么根据 torch 的 broadcast 规则 [Broadcasting semantics](http://pytorch.org/docs/master/notes/broadcasting.html#broadcasting-semantics), torch.Tensor((1, 2, 3)) 会被扩展为 torch.Tensor([[1, 2, 3], [1, 2, 3], [1, 2, 3]]), 然后再将前面的结果, 扩展 3 次.

    ```python
    a = torch.arange(0, 27).view(3, 3, 3)
    print torch.Tensor((1, 2, 3)).size()
    print a
    print a.sub_(torch.Tensor((1, 2, 3)))

    # 结果:
    (3L,)
    (0 ,.,.) = 
       0   1   2
       3   4   5
       6   7   8

    (1 ,.,.) = 
       9  10  11
      12  13  14
      15  16  17

    (2 ,.,.) = 
      18  19  20
      21  22  23
      24  25  26
    [torch.FloatTensor of size 3x3x3]


    (0 ,.,.) = 
      -1  -1  -1
       2   2   2
       5   5   5

    (1 ,.,.) = 
       8   8   8
      11  11  11
      14  14  14

    (2 ,.,.) = 
      17  17  17
      20  20  20
      23  23  23
    [torch.FloatTensor of size 3x3x3]
    ```

+   Scale

    +   对 PIL.Image 对象进行处理, 将该对象缩放成指定的大小. 方法是使用默认的 Image.BILINEAR 的双线性插值. 输出会保持原图像的高宽比:

    ```python
    return img.resize((ow, oh), self.interpolation)
    ```

    +   另一方面, 指定的图像大小 size 会成为处理后的图像的最短的边, 如果原图像某一边正好等于 size, 那么就不做 scale 处理. (注意 resize 中是 (ow, oh))

    ```python
    if (w <= h and w == self.size) or (h <= w and h == self.size):
        return img
    if w < h:
        ow = self.size
        oh = int(self.size * h / w)
        return img.resize((ow, oh), self.interpolation)
    else:
        oh = self.size
        ow = int(self.size * w / h)
        return img.resize((ow, oh), self.interpolation)
    ```

+   CenterCrop

    +   对 PIL.Image 对象进行处理, 取出中心部分的内容:

    ```python
    def __call__(self, img):
        w, h = img.size
        th, tw = self.size
        x1 = int(round((w - tw) / 2.))
        y1 = int(round((h - th) / 2.))
        return img.crop((x1, y1, x1 + tw, y1 + th))
    ```

+   Pad

    +   对 PIL.Image 进行 Pad, 可以指定 fill 的值, 默认是 0, 使用了 PIL.ImageOps

    ```python
    class Pad(object):
        """Pads the given PIL.Image on all sides with the given "pad" value"""

        def __init__(self, padding, fill=0):
            assert isinstance(padding, numbers.Number)
            assert isinstance(fill, numbers.Number) or isinstance(fill, str) or isinstance(fill, tuple)
            self.padding = padding
            self.fill = fill

        def __call__(self, img):
            return ImageOps.expand(img, border=self.padding, fill=self.fill)
    ```

+   Lambda

    使用 Lambda 对 PIL.Image 进行变换

    ```python
    class Lambda(object):
        """Applies a lambda as a transform."""

        def __init__(self, lambd):
            assert isinstance(lambd, types.LambdaType)
            self.lambd = lambd

        def __call__(self, img):
            return self.lambd(img)
    ```

+   RandomCrop

    对 PIL.Image 进行 RandomCrop, 可以看到, 能先对 Image 进行 padding. 之后从 (0, w - tw) 但范围内随机选取一个值作为坐标, 再 Crop

    ```python
    class RandomCrop(object):
        """Crops the given PIL.Image at a random location to have a region of
        the given size. size can be a tuple (target_height, target_width)
        or an integer, in which case the target will be of a square shape (size, size)
        """

        def __init__(self, size, padding=0):
            if isinstance(size, numbers.Number):
                self.size = (int(size), int(size))
            else:
                self.size = size
            self.padding = padding

        def __call__(self, img):
            if self.padding > 0:
                img = ImageOps.expand(img, border=self.padding, fill=0)

            w, h = img.size
            th, tw = self.size
            if w == tw and h == th:
                return img

            x1 = random.randint(0, w - tw)
            y1 = random.randint(0, h - th)
            return img.crop((x1, y1, x1 + tw, y1 + th))
    ```



+   RandomHorizontalFlip

    根据概率是否小于 0.5, 将 Image 图像进行左右翻转.

    ```python
    class RandomHorizontalFlip(object):
        """Randomly horizontally flips the given PIL.Image with a probability of 0.5
        """

        def __call__(self, img):
            if random.random() < 0.5:
                return img.transpose(Image.FLIP_LEFT_RIGHT)
            return img
    ```

+   RandomSizedCrop

    +   先将图像进行缩放到指定的大小, 然后再进行 Crop. (这是在尝试随机方法 10 次后采取的办法), 而如果 attmpt 中有一次成功, 那么:
    +   首先将 PIL.Image 对象变成原图像在 (0.08, 1.0) 范围内随机的大小, 并且使得图像的 aspect ratio 也在 (3/4, 4/3) 范围内随机变化, 变化完成后要保证变化后的 w 或 h 要同时比原图像的 w 和 h 小, 最后对图像进行 crop 以及 resize 为指定的 size.

    ```python
    class RandomSizedCrop(object):
        """Random crop the given PIL.Image to a random size of (0.08 to 1.0) of the original size
        and and a random aspect ratio of 3/4 to 4/3 of the original aspect ratio
        This is popularly used to train the Inception networks
        size: size of the smaller edge
        interpolation: Default: PIL.Image.BILINEAR
        """

        def __init__(self, size, interpolation=Image.BILINEAR):
            self.size = size
            self.interpolation = interpolation

        def __call__(self, img):
            for attempt in range(10):
                area = img.size[0] * img.size[1]
                target_area = random.uniform(0.08, 1.0) * area
                aspect_ratio = random.uniform(3. / 4, 4. / 3)

                w = int(round(math.sqrt(target_area * aspect_ratio)))
                h = int(round(math.sqrt(target_area / aspect_ratio)))

                if random.random() < 0.5:
                    w, h = h, w

                if w <= img.size[0] and h <= img.size[1]:
                    x1 = random.randint(0, img.size[0] - w)
                    y1 = random.randint(0, img.size[1] - h)

                    img = img.crop((x1, y1, x1 + w, y1 + h))
                    assert(img.size == (w, h))

                    return img.resize((self.size, self.size), self.interpolation)

            # Fallback
            scale = Scale(self.size, interpolation=self.interpolation)
            crop = CenterCrop(self.size)
            return crop(scale(img))
    ```

    ​