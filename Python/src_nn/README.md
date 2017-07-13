# 注意事项

## 2017 年 7 月 13 日

+   使用 LapSRN 得到的结果 (scale 为 4);

    ```bash
    			   lapsrn     bicubic
    butterfly:    27.54dB     22.10dB
    woman:        30.45dB     26.47dB
    head:         32.85dB     31.62dB
    baby:         33.58dB     31.79dB
    bird:         33.81dB     30.18dB
    Average:      31.65dB     28.43dB
    ```

+   对 vdsr 中的代码进行修改, 使用 Set5 中的 .mat 数据, 可以得到最后的结果为: (scale 为 4)

    ```bash
    				vdsr      bicubic
    butterfly:    27.21dB     22.10dB
    woman:        30.04dB     26.47dB
    head:         32.83dB     31.62dB
    baby:         33.47dB     31.79dB
    bird:         33.72dB     30.18dB
    Average:      31.45dB     28.43dB
    ```

+ 使用 `caffe-vdsr` 中的 `Demo_SR_Conv.m` 测试 Set5 数据集使用 `VDSR_Official.mat` 进行超分辨率的 PSNR 值, 其中 scale 为 3:

```bash
woman_GT.bmp: 32.35dB
head_GT.bmp: 33.98dB
butterfly_GT.bmp: 29.95dB
bird_GT.bmp:
baby_GT.bmp:
Average: 
```

最后两个结果似乎不正确.

+ 在 MATLAB 上测试 Set5 数据集使用 Bicubic 算法进行超分辨率得到的 PSNR 值, 其中 scale 为 3:

```bash
woman_GT.bmp: 28.57dB
head_GT.bmp: 32.91dB
butterfly_GT.bmp: 24.04dB
bird_GT.bmp: 32.59dB
baby_GT.bmp: 33.93dB
Average: 30.41dB
```

测试代码:

+ `test.m`

```matlab
close all;
clear all;

up_scale = 3;
shave = 1;

im_gt = imread('./Data/Set5/baby_GT.bmp');

im_gt = modcrop(im_gt,up_scale);
im_l = imresize(im_gt,1/up_scale,'bicubic');
im_gt = double(im_gt);
im_l  = double(im_l) / 255.0;

[H,W,C] = size(im_l);
if C == 3
    im_l_ycbcr = rgb2ycbcr(im_l);
else
    im_l_ycbcr = im_l;
end
im_l_y = im_l_ycbcr(:,:,1);

im_h_ycbcr = imresize(im_l_ycbcr,up_scale,'bicubic');
if C == 3
    im_b = ycbcr2rgb(im_h_ycbcr) * 255.0;
else
    im_b = im_y_cbcr * 255.0;
end

if shave == 1;
    shave_border = round(up_scale);
else
    shave_border = 0;
end
bi_psnr = compute_psnr(im_b,im_gt,shave_border);
fprintf('bi_psnr: %f dB\n',bi_psnr);
```

+ `compute_psnr.m`

```matlab
function psnr = compute_psnr(im1,im2,shave_border)
if size(im1, 3) == 3,
    im1 = rgb2ycbcr(im1);
    im1 = im1(:, :, 1);
end

if size(im2, 3) == 3,
    im2 = rgb2ycbcr(im2);
    im2 = im2(:, :, 1);
end

imdff = double(im1) - double(im2);
if shave_border > 0
    imdff = shave(imdff,[shave_border,shave_border]);
end
imdff = imdff(:);
im1 = shave(im1,[shave_border,shave_border]);
im2 = shave(im2,[shave_border,shave_border]);
rmse = sqrt(mean(imdff.^2));
psnr = 20*log10(255/rmse);
```

+ `modcrop.m`

```matlab
function imgs = modcrop(imgs, modulo)
if size(imgs,3)==1
    sz = size(imgs);
    sz = sz - mod(sz, modulo);
    imgs = imgs(1:sz(1), 1:sz(2));
else
    tmpsz = size(imgs);
    sz = tmpsz(1:2);
    sz = sz - mod(sz, modulo);
    imgs = imgs(1:sz(1), 1:sz(2),:);
end
```

+ `shave.m`

```matlab
function I = shave(I, border)
I = I(1+border(1):end-border(1), ...
      1+border(2):end-border(2), :, :);
```

## 2017 年 7 月 11 日

+ `test.py`: `crop_size = calculate_valid_crop_size` 中的 `max` 应该改为 `min`
+ 输出图像在有的地方有黑点, 原因是最后输出的 `pred` 需要进行 clip, 使得像素的大小在
  `[0, 255]` 之间, 所以我使用了:

```python
pred[pred < 0.0] = 0.0
pred[pred > 1.0] = 1.0
```

+ 注意最后合成 RGB 图像时, `Cb` 和 `Cr` 必须是经过两次 `Bicubic` 算法处理过的, 不能直接
  使用原图的 `Cb` 与 `Cr`. 因此增加了 `bicubic_transform`
