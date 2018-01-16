# dataloader.py

## 2018 年 1 月 15 日

用于数据的载入. 主要定义了 `DataLoader` 这个接口, 当初我比较疑惑的是 `num_workers` 这个参数的意义, 现在就想着要将它搞清楚. 下面具体看看这个文件中的代码.

首先看 import 的内容:

```python
import torch
import torch.multiprocessing as multiprocessing
from .sampler import SequentialSampler, RandomSampler
import collections
import sys
import traceback

class ExceptionWrapper(object):
    "Wraps an exception plus traceback to communicate across threads"

    def __init__(self, exc_info):
        self.exc_type = exc_info[0]
        self.exc_msg = "".join(traceback.format_exception(*exc_info))
```



## DataLoader

这个类用于读取 `torch.utils.Dataset` 对象中的数据, 该对象定义在 `pytorch/torch/utils/dataset.py` 中, 大多数时候, 我们需要继承这个类从而定义自己的数据集对象, 只要实现 `__getitem__` 和 `__len__` 两个方法即可. `Dataset` 的定义如下:

```python
class Dataset(object):

    def __getitem__(self, index):
        raise NotImplementedError

    def __len__(self):
        raise NotImplementedError
```

之后, `DataLoader` 以 `Dataset` 作为参数, 从而读取 `Dataset` 中的数据, `DataLoader` 的定义如下:

```python
class DataLoader(object):
    """
    Data loader. Combines a dataset and a sampler, and provides
    single- or multi-process iterators over the dataset.
    """

    def __init__(self, dataset, batch_size=1, shuffle=False,
                 sampler=None, num_workers=0, collate_fn=default_collate):
        self.dataset     = dataset
        self.batch_size  = batch_size
        self.num_workers = num_workers
        self.collate_fn  = collate_fn

        if sampler is not None:
            self.sampler = sampler
        elif shuffle:
            self.sampler = RandomSampler(dataset)
        elif not shuffle:
            self.sampler = SequentialSampler(dataset)

    def __iter__(self):
        return DataLoaderIter(self)

    def __len__(self):
        return len(self.sampler)
```

在实际的应用场景中, 我们对 `DataLoader` 的使用是:

```python
for data, target in dataloader:
    # .....
```

其中 `(data, target)` 就是由 `DataLoaderIter` 返回的, 这个之后再来说明. 先介绍一下 `DataLoader` 的其他参数.

+   `batch_size`: mini-batch 的大小
+   `shuffle`: 是否需要 shuffle 数据, 默认为 False, 即顺序读取数据.
+   `sampler`: 默认为 `None`, 会自动选择 `SequentialSampler`, 即生成顺序的样本索引. 如果 `shuffle` 为 True, 那么会选择 `RandomSampler` 产生随机的索引.
+   `num_workers`: 进程的数量.
+   `collate_fn`: 默认为 `default_collate`, 将从 `Dataset` 中得到的数据转换为 Tensor.



## DataLoaderIter

它是 `DataLoader` 中实际干活的类, 用于不断的输出 batch.

+   初始化代码中获取了从 `DataLoader` 中得到的参数. 需要注意的是, 当 `self.num_workers` 不为 0 时, 会使用多进程对数据 `self.dataset` 进行处理, 其中 `self.index_queue` 保存着一个 batch 中各个元素在 `self.dataset` 中的索引, `self.data_queue` 用于保存这些索引对应着的元素. `self.collate_fn` 用于将数据转换为 Tensor.
+   `self.batches_outstanding` 和 `self.joined` 控制着进程的结束, 当 `self.batches_outstanding` 为 0 时, `self.joined` 为 True, 此时所有的进程已经将任务完成, 从而结束.
+   `_nextBatch` 用于返回下一个 batch 的索引.
+   `_putBatch` 根据 `self.samples_remaining` 的大小, 判断是否还有剩余的数据, 如果还有, 那么就将下一个 batch 加入到 `self.index_queue` 中.
+   调用 `next` 方法, 不断地从 `self.data_queue` 中获取数据.

```python
class DataLoaderIter(object):
    "Iterates once over the DataLoader's dataset, as specified by the sampler"

    def __init__(self, loader):
        self.dataset = loader.dataset
        self.batch_size = loader.batch_size
        self.collate_fn = loader.collate_fn
        self.sampler = loader.sampler
        self.num_workers = loader.num_workers

        self.samples_remaining = len(self.sampler)
        self.sample_iter = iter(self.sampler)

        if self.num_workers:
            self.index_queue = multiprocessing.Queue()
            self.data_queue = multiprocessing.Queue()
            self.batches_outstanding = 0
            self.joined = False

            self.workers = [
                multiprocessing.Process(
                    target=_workerLoop,
                    args=(self.dataset, self.index_queue, self.data_queue, self.collate_fn))
                for i in range(self.num_workers)]

            for w in self.workers:
                w.daemon = True # ensure that the worker exits on process exit
                w.start()
                # prime the prefetch loop with exactly 1 batch per process
                # this ensures no deadlocks on the queues using the blocking queue API
                self._putBatch()

    def _nextBatch(self):
        # next(self.sample_iter) 一次产生一个索引, 所以 batch 中有 batch_size 个索引
        batch = [next(self.sample_iter) for x in range(min(self.samples_remaining, self.batch_size))]
        self.samples_remaining -= len(batch)
        return batch

    def _putBatch(self):
        if self.samples_remaining > 0:
            self.index_queue.put(self._nextBatch())
            self.batches_outstanding += 1

    def next(self):
        if self.num_workers:
            # multi-process loading
            if self.batches_outstanding:
                assert(not self.joined)
                # maintain at most len(workers)+1 outstanding batches
                # to avoid deadlocks in the queues, using the blocking queue API
                # TODO: add and use non-blocking queue API
                self._putBatch()
                assert(self.batches_outstanding <= len(self.workers) + 1)
                self.batches_outstanding -= 1
                data = self.data_queue.get()

                if isinstance(data, ExceptionWrapper):
                    raise data.exc_type(data.exc_msg)
                else:
                    return data
            else:
                self._joinWorkers()
                raise StopIteration()
        else:
            # single-process loading
            if self.samples_remaining:
                return _processBatch(self.dataset, self._nextBatch(), self.collate_fn)
            else:
                raise StopIteration()

    __next__ = next

    def __getstate__(self):
        # TODO: add limited pickling support for sharing an iterator
        # across multiple threads for HOGWILD.
        # Probably the best way to do this is by moving the sample pushing
        # to a separate thread and then just sharing the data queue
        # but signalling the end is tricky without a non-blocking API
        raise NotImplementedError("DataLoaderIterator cannot be pickled")

    def _joinWorkers(self):
        self.joined = True
        if self.num_workers:
            [self.index_queue.put(None) for x in self.workers]
            [x.join() for x in self.workers]

    def __del__(self):
        self._joinWorkers()
```



## workerLoop

多进程在 `DataLoaderIter` 对象初始化时就已经创建了, 入口函数是 `_workerLoop`, 定义如下.

+   `_workerLoop` 从 `self.index_queue` 中读取下一个 batch 的索引, 通过使用 `_processBatch` 方法获取索引对应的 data, 并通过 `collate_fn` 将 data 转换为 Tensor. 然后再将这些 `samples` 加入到 `self.data_queue` 中供 `next()` 方法使用.
+   `default_collate` 很多 `if`, 其中第一个 `if` 是用于处理一般的数据的, 后面几个 `if` 主要是用来处理 `target` 的. 但是最后一个 `collections.Iterable` 目前我想不到什么例子...

```python
def _processBatch(dataset, indices, collate_fn):
    samples = [dataset[idx] for idx in indices]

    samples = collate_fn(samples)
    return samples


def _workerLoop(dataset, index_queue, data_queue, collate_fn):
    while True:
        batch_indices = index_queue.get()

        if batch_indices is None:
            break

        try:
            samples = _processBatch(dataset, batch_indices, collate_fn)
        except Exception:
            data_queue.put(ExceptionWrapper(sys.exc_info()))
        else:
            data_queue.put(samples)

# default collate function, puts each data field into a
# tensor with outer dimension batchSize
def default_collate(batch):
    if torch.is_tensor(batch[0]):
        return torch.cat([t.view(1, *t.size()) for t in batch], 0)
    elif isinstance(batch[0], int):
        return torch.LongTensor(batch)
    elif isinstance(batch[0], float):
        return torch.DoubleTensor(batch)
    elif isinstance(batch[0], str):
        return batch
    elif isinstance(batch[0], collections.Iterable):
        # if each batch element is not a tensor, then it should be a tuple
        # of tensors; in that case we collate each element in the tuple
        transposed = zip(*batch)
        return [default_collate(samples) for samples in transposed]

    raise TypeError(("batch must contain tensors, numbers, or lists; found {}"
                     .format(type(batch[0]))))
```



## 例子

看完上面的代码我当然会想写个示例代码看看多进程处理数据对代码效率有多大的提升. 下面是例子, 注意, 我将 `torchvision/transforms.py` https://github.com/pytorch/vision/blob/v0.1.8/torchvision/transforms.py 中很多代码写在该示例代码中, 是因为随着 pytorch 的更新, 很多接口发生了点变化, 所以需要这般处理.

```python
import os
from PIL import Image
from torch.utils.data import Dataset, DataLoader
import torch
import numpy as np
import numbers
import time
from functools import wraps
  
def fn_timer(function):
    @wraps(function)
    def function_timer(*args, **kwargs):
        t0 = time.time()
        result = function(*args, **kwargs)
        t1 = time.time()
        print ("Total time running %s: %s seconds" %
               (function.func_name, str(t1-t0))
              )
        return result
    return function_timer


IMG_EXTENSIONS = ['.jpg', '.png', '.ppm', '.jpeg', '.bmp']

def is_image_file(filename):
    filename_lower = filename.lower()
    return any(filename_lower.endswith(ext) for ext in IMG_EXTENSIONS)

def make_dataset(dir):
    images = []
    dir = os.path.expanduser(dir)
    for root, _, fnames in sorted(os.walk(dir)):
        for index, fname in enumerate(sorted(fnames)):
            if is_image_file(fname):
                path = os.path.join(root, fname)
                item = (path, index)
                images.append(item)
    return images

def default_loader(path):
    with open(path, 'rb') as f:
        with Image.open(f) as img:
            return img.convert('RGB')

class ImageFolder(Dataset):
    def __init__(self, root, transform=None, target_transform=None,
                 loader=default_loader):
        super(ImageFolder, self).__init__()
        self.root = root
        self.imgs = make_dataset(root)
        self.transform = transform
        self.target_transform = target_transform
        self.loader = default_loader


    def __getitem__(self, index):
        path, target = self.imgs[index]
        img = self.loader(path)
        if self.transform is not None:
            img = self.transform(img)
        if self.target_transform is not None:
            target = self.target_transform(target)

        return img, target


    def __len__(self):
        return len(self.imgs)


class ToTensor(object):
    """Converts a PIL.Image or numpy.ndarray (H x W x C) in the range
    [0, 255] to a torch.FloatTensor of shape (C x H x W) in the range [0.0, 1.0].
    """

    def __call__(self, pic):
        if isinstance(pic, np.ndarray):
            # handle numpy array
            img = torch.from_numpy(pic.transpose((2, 0, 1)))
            # backard compability
            return img.float().div(255)
        # handle PIL Image
        if pic.mode == 'I':
            img = torch.from_numpy(np.array(pic, np.int32, copy=False))
        elif pic.mode == 'I;16':
            img = torch.from_numpy(np.array(pic, np.int16, copy=False))
        else:
            img = torch.ByteTensor(torch.ByteStorage.from_buffer(pic.tobytes()))
        # PIL image mode: 1, L, P, I, F, RGB, YCbCr, RGBA, CMYK
        if pic.mode == 'YCbCr':
            nchannel = 3
        elif pic.mode == 'I;16':
            nchannel = 1
        else:
            nchannel = len(pic.mode)
        img = img.view(pic.size[1], pic.size[0], nchannel)
        # put it from HWC to CHW format
        # yikes, this transpose takes 80% of the loading time/CPU
        img = img.transpose(0, 1).transpose(0, 2).contiguous()
        if isinstance(img, torch.ByteTensor):
            return img.float().div(255)
        else:
            return img


class CenterCrop(object):
    """Crops the given PIL.Image at the center to have a region of
    the given size. size can be a tuple (target_height, target_width)
    or an integer, in which case the target will be of a square shape (size, size)
    """

    def __init__(self, size):
        if isinstance(size, numbers.Number):
            self.size = (int(size), int(size))
        else:
            self.size = size

    def __call__(self, img):
        w, h = img.size
        th, tw = self.size
        x1 = int(round((w - tw) / 2.))
        y1 = int(round((h - th) / 2.))
        return img.crop((x1, y1, x1 + tw, y1 + th))


class Compose(object):
    """Composes several transforms together.

    Args:
        transforms (List[Transform]): list of transforms to compose.

    Example:
        >>> transforms.Compose([
        >>>     transforms.CenterCrop(10),
        >>>     transforms.ToTensor(),
        >>> ])
    """

    def __init__(self, transforms):
        self.transforms = transforms

    def __call__(self, img):
        for t in self.transforms:
            img = t(img)
        return img



if __name__ == '__main__':
    root = '/home/ieric/DataSet/291/'
    trans = Compose([
        CenterCrop(96),
        ToTensor()
    ])
    dataset = ImageFolder(root, transform=trans)
    dataloader = DataLoader(dataset, batch_size=64, shuffle=False,
                    sampler=None, num_workers=20)

    dataiter = iter(dataloader)

    @fn_timer
    def myfun():
        return next(dataiter)

    myfun()
```

+   当 `num_workers=0` 时, ... 算了, 以后再说