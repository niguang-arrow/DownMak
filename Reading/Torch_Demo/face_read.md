# Train face detector

## Jun 25, 2017

## data.lua

+ Download data and uncompress data

```lua
if not paths.dirp('face-dataset') then
    os.execute('cd ' .. train_dir)
    os.execute('wget ' .. www)
    os.execute('unzip ' .. tar)
end
```

+ load dataset from disk

```lua
local imagesAll = torch.Tensor(41267, 3, 32, 32)
local labelsAll = torch.Tensor(41267)

-- classes: Global val
classes = {'face', 'backg'}

-- load faces and background
for f=0, 28033 do
    imagesAll[f+1] = image.load('face-dataset........')
    labelsAll[f+1] = 2 -- 2 = background
end
```

+ Shuffle dataset: get shuffled indices in this variable

```lua
local labelsShuffle = torch.randperm((#labelsAll)[1])
local portionTrain = 0.8
local trsize = torch.floor(labelsShuffle:size(1) * portionTrain)
local tesize = labelsShuffle:size(1) - trsize
```

+ Create train set and test set

```lua
trainData = {
    data = torch.Tensor(trsize, 1, 32, 32),
    labels = torch.Tensor(trsize),
    size = function() return trsize end
}
```

+ use `clone()`

```lua
for i=1, trsize do
    trainData.data[i] = imagesAll[labelsShuffle[i]][1]:clone()
    trainData.labels[i] = labelsAll[labelsShuffle[i]]
end
```

+ preprocess data

    + faces and bg are already YUV in the dataset, so no need to convert; However, you can use
    `image.rgb2yuv(trainData.data[i])` to convert the colorspace from RGB -> YUV.
    + preprocessing requires a floating point representation (the original data is stored
    on bytes). So there needs a conversion: `dst = src:type('torch.TypeTensor')`, there are
    also some shortcuts like `float()`, `double()`, `cuda()` ...
    + preprocessing tricks for natural images:
        + images are mapped into YUV space, to separate luminance information from color information
        + the luminance channel (Y) is locally normalized, using a contrastive
        normalization operator: for each neighborhood, defined by a Gaussian
        kernel, the mean is suppressed, and the standard deviation is normalized to one.
        + color channels are normalized globally, across the entire dataset;
        as a result, each color component has 0-mean and 1-norm across the dataset.
