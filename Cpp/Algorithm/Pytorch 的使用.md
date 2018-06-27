# Pytorch 的使用

2018 年 6 月 26 日

### 类似 repmat 的操作

实际上 numpy 中提供了 `np.tile` 和 matlab 中的 repmat 功能相同:

```python
a = np.arange(4).reshape(1, -1)
a = np.tile(a, (4, 1))

array([[0, 1, 2, 3],
       [0, 1, 2, 3],
       [0, 1, 2, 3],
       [0, 1, 2, 3]])

b = torch.from_numpy(a)
```

