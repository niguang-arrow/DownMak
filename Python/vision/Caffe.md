# Caffe

## 2017 年 7 月 9 日

[https://github.com/nitnelave/pycaffe_tutorial/blob/master/02%20Network%20manipulation.ipynb](https://github.com/nitnelave/pycaffe_tutorial/blob/master/02%20Network%20manipulation.ipynb)

+ 套路:

  ```python
  # 当使用 caffe.Net 载入模型的时候, 会输出大量的关于模型的信息,
  # 如果不想显示这些信息, 可以使用如下两条命令抑制,
  # 这两条命令要用在 import caffe 之前使用
  import os
  os.environ["GLOG_minloglevel"] = '2'

  import caffe
  from caffe import layers
  import numpy as np

  # for reproducible results
  caffe.set_random_seed(0)
  np.random.seed(0)

  # GPU
  USE_GPU = True
  if USE_GPU:
      caffe.set_device(0)
      caffe.set_mode_gpu()
  else:
      caffe.set_mode_cpu()
  ```

### Loading network

```python
net = caffe.Net('/home/ieric/Programs/caffe/examples/mnist/lenet.prototxt', caffe.TRAIN)
```

### Inspecting the network

+ 查看网络每一层的名字, 使用 `net._layer_names` , 其类型为 `<class 'caffe._caffe.StringVec'>`, 它输出所有的名字, 和生成器类似.

+ 查看网络每一层, 使用 `net.layers`

  ```python
  print("Network layers:")
  for name, layer in zip(net._layer_names, net.layers):
      print("{:<7}: {:17s}({} blobs)".format(name, layer.type, len(layer.blobs)))
  ```

  结果为:

  ```python
  data   : Input            (0 blobs)
  conv1  : Convolution      (2 blobs)
  pool1  : Pooling          (0 blobs)
  conv2  : Convolution      (2 blobs)
  pool2  : Pooling          (0 blobs)
  ip1    : InnerProduct     (2 blobs)
  relu1  : ReLU             (0 blobs)
  ip2    : InnerProduct     (2 blobs)
  prob   : Softmax          (0 blobs)
  ```

  **等下要注意区分 `layer.blobs` 和下面展示的 `net.blobs` 的区别**: `layer.blobs` 存储了网络的参数, 比如 weight 与 bias; 而 `net.blobs` 存储了网络每一层的结果. 网络的参数还可以使用 `net.params` 访问, 比如访问 `ip1` 的网络参数: 

  ```python
  len(net.params['ip1'])
  # 结果为 2
  ```

  网络参数可以像 numpy 数组那样修改.

+ 网络的 blobs, 是一个 OrderedDict, 

  >  These blobs carry the data between the layers

  因此, 

  ```python
  print("Blobs:")
  for name, blob in net.blobs.iteritems():
      print("{:<5}:  {}".format(name, blob.data.shape))
  ```

  结果是:

  ```python
  data : (64, 1, 28, 28)
  conv1: (64, 20, 24, 24)
  pool1: (64, 20, 12, 12)
  conv2: (64, 50, 8, 8)
  pool2: (64, 50, 4, 4)
  ip1  : (64, 500)
  ip2  : (64, 10)
  prob : (64, 10)
  ```

  The blobs provide access to:

  - `num`: the number of elements (usually `batch_size`)
  - `channels`, `height`, `width`: the dimension of a sample
  - `shape`: a tuple with `(num, channels, height, width)`
  - `count`: `num * channels * height * width`
  - `data`: the data stored in the blob (see forward propagation)
  - `diff`: the computed gradient for the blob (see backward propagation)

  **Be aware that when getting the `data` or `diff` of a blob, it is a reference, or pointer, to the actual memory, so anything changing it will change all current references. To get a snapshot of the value that will not change, copy it with `blob.data.copy()`.**

+ 查看网络的 `top_names` 与 `bottom_names`, 了解网络每层之间的连接关系, 可以使用 `net.top_names` 与 `net.bottom_names`:

  ```python
  print net.top_names

  # 结果为
  OrderedDict([('data', ['data']), 
               ('conv1', ['conv1']), 
               ('pool1', ['pool1']), 
               ('conv2', ['conv2']), 
               ('pool2', ['pool2']), 
               ('ip1', ['ip1']), 
               ('relu1', ['ip1']),  # relu 的输出仍为 ip1 说明是 in-place 处理
               ('ip2', ['ip2']), 
               ('prob', ['prob'])])
  ```

  那么如果要访问 `data` 层的 top_names, 可以使用 `net.top_names['data']`, 得到的结果是一个 list: `['data']`, 原因是某些层的输出(或者 top_names)可能不止一个, 比如

  有的可能为 `['data', 'label']`.

+ 还可以使用 `net.inputs` 与 `net.outputs` 访问网络的输入输出. 注意 `net.outputs` 指的是某输出不为其余层的输入, 比如最后的 loss.

### Forward propogation

+ **We cannot assign directly the field `data`, since it is a C++ field. However, we can set its contents, like so:** (此要点需注意)

  ```python
  # net.blobs['data'] 为 data 层的结果, 其有属性 shape, 见前面的笔记
  batch = np.random.randn(*net.blobs['data'].shape) 

  # 对 data 属性, 即输入数据进行赋值, 但如果直接使用
  # net.blobs['data'].data = batch 则会报错: 
  # AttributeError: can't set attribute, 虽然 net.blobs['data'].data 为
  # <numpy.ndarray>, 但如果给 .data 直接赋值的话, 则是给 net.blobs['data']
  # 的属性赋值, 看来这是不允许的.
  # 注意省略号的使用.
  net.blobs['data'].data[...] = batch
  ```

+ 由于最后的结果使用 `.data` 可以得到, 让我找到了相似的感觉.

+ 前向传播直接使用 `net.forward()`

+ `net.forward()` 还支持部分前向传播, 通过指定 `start` 与 `end`:

  ```python
  res = net.forward(start="mnist", end="conv1")
  ```

  ​

  ​