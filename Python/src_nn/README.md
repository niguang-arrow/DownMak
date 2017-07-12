# 注意事项

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
