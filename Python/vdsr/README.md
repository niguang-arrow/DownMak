# VDSR

## 2017 年 7 月 26 日

分为三步来训练 VDSR 网络, 分别是下面的 "准备工作" 和 "训练网络", "测试网络", 按顺序执行, 保你无忧.

## 准备工作

### 预处理

+   下载代码: https://github.com/twtygqyy/pytorch-vdsr
    +   删除其中的 `Set5/` 文件夹, 以及 `data/train.h5` 文件
+   下载代码: https://github.com/huangzehao/caffe-vdsr
    +   参照作者给出的 ReadMe, 对数据进行处理;
    +   首先下载 291 数据集 http://cv.snu.ac.kr/research/VDSR/train_data.zip (点击直接下载)
    +   解压后应该会得到 `291` 这个目录, 包含了 291 张图片, 将该目录拷贝到 `caffe-vdsr/Train` 目录下, 打开 MATLAB, 运行 `data_aug.m` 程序增加数据, 输出的目录应该是 `291-aug/`.
+   将 `caffe-vdsr/291-aug/` 目录复制到 `pytorch-vdsr/data/` 目录中, 注意到 `pytorch-vdsr/data/` 目录中有 `generate_train.m` 文件, 打开 MATLAB, 将 `generate_train.m` 文件中的第三行代码改为 `folder = './291-aug';`, 然后运行该文件, 即可在当前目录下生成 `train.h5` 文件. 其中训练样本的大小为 `41x41x1x1235584`.

### 修改训练代码

+   进入到 `pytorch-vdsr` 目录中, 打开 main.py 文件, 使用 `%s/\r//` 消除 `^M`
+   运行 main.py 代码时, 需要注意将 `--threads` 的大小设置为 1, 如果使用默认值会报错.

### 修改测试代码

+   进入到 https://github.com/twtygqyy/pytorch-LapSRN 中, 将其中的 `Set5/` 文件夹复制到 `pytorch-vdsr/` 目录下;


+   创建 `vdsr_test.py` 文件, 将如下内容拷贝到该文件中:

    ```python
    import argparse
    import torch
    from torch.autograd import Variable
    import numpy as np
    import time
    import math
    import scipy.io as sio
    import matplotlib.pyplot as plt

    parser = argparse.ArgumentParser(description="PyTorch VDSR Test")
    parser.add_argument("--cuda", action="store_true", help="use cuda?")
    parser.add_argument("--model", default="model/model_epoch_50.pth", type=str, help="model path")
    parser.add_argument("--image", default="butterfly_GT", type=str, help="image name")
    parser.add_argument("--scale", default=4, type=int, help="scale factor, Default: 4")


    def PSNR(pred, gt, shave_border=0):
        height, width = pred.shape[:2]
        pred = pred[shave_border:height - shave_border, shave_border:width - shave_border]
        gt = gt[shave_border:height - shave_border, shave_border:width - shave_border]
        imdff = pred - gt
        rmse = math.sqrt(np.mean(imdff ** 2))
        if rmse == 0:
            return 100
        return 20 * math.log10(255.0 / rmse)


    opt = parser.parse_args()
    cuda = opt.cuda

    if cuda and not torch.cuda.is_available():
        raise Exception("No GPU found, please run without --cuda")

    model = torch.load(opt.model)["model"]

    im_gt_y = sio.loadmat("Set5/" + opt.image + ".mat")['im_gt_y']
    im_b_y = sio.loadmat("Set5/" + opt.image + ".mat")['im_b_y']

    im_gt_y = im_gt_y.astype(float)
    im_b_y = im_b_y.astype(float)

    psnr_bicubic = PSNR(im_gt_y, im_b_y, shave_border=opt.scale)

    im_input = im_b_y/255.

    im_input = Variable(torch.from_numpy(im_input).float()).view(1, -1, im_input.shape[0], im_input.shape[1])

    if cuda:
        model = model.cuda()
        im_input = im_input.cuda()

    start_time = time.time()
    out = model(im_input)
    elapsed_time = time.time() - start_time

    out = out.cpu()

    im_h_y = out.data[0].numpy().astype(np.float32).squeeze()

    im_h_y = im_h_y * 255.
    im_h_y[im_h_y < 0] = 0
    im_h_y[im_h_y > 255.] = 255.

    psnr_predicted = PSNR(im_gt_y, im_h_y, shave_border=opt.scale)

    print("Scale=", opt.scale)
    print("PSNR_predicted=", psnr_predicted)
    print("PSNR_bicubic=", psnr_bicubic)
    print("It takes {}s for processing".format(elapsed_time))

    # fig = plt.figure()
    # ax = plt.subplot("131")
    # ax.imshow(im_gt_y, cmap='gray')
    # ax.set_title("GT")

    # ax = plt.subplot("132")
    # ax.imshow(im_b_y, cmap='gray')
    # ax.set_title("Input(Bicubic)")

    # ax = plt.subplot("133")
    # ax.imshow(im_h_y, cmap='gray')
    # ax.set_title("Output(LapSRN)")
    # plt.show()
    ```


+   运行 `python vdsr_test.py` 可以得到最终的 psnr 对比为:

    ```bash
    ('Scale=', 4)
    ('PSNR_predicted=', 27.210109584212265)
    ('PSNR_bicubic=', 22.0999883845094)
    ```

+   由此可知作者原来给出的模型训练得非常好, 应将其保存起来, 于是创建 `GT_model` 文件夹, `mv model/model_epoch_50.pth GT_model/`.

## 训练网络

```bash
python main.py --cuda --threads 1
```

## 测试网络

```bash
python test.py --cuda --model model/model_epoch_1.pth
```

## 结果

对 vdsr 中的代码进行修改, 使用 Set5 中的 .mat 数据, 可以得到最后的结果为: (scale 为 4)

```bash
               vdsr      bicubic
baby:         33.47dB     31.79dB
bird:         33.72dB     30.18dB
butterfly:    27.21dB     22.10dB
head:         32.83dB     31.62dB
woman:        30.04dB     26.47dB
Average:      31.45dB     28.43dB
```

作为对比, 给出 VDSR 论文作者的结果:

```bash
               vdsr      bicubic
baby:         33.41dB     31.79dB
bird:         33.54dB     30.18dB
butterfly:    27.28dB     22.10dB
head:         32.70dB     31.62dB
woman:        29.81dB     26.47dB
Average:      31.35dB     28.43dB
```
