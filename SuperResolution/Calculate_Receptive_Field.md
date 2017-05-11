# C

## 2017 年 05 月 10 日

### 计算 Receptive Field 的大小

更为具体的信息请查阅:

[http://stackoverflow.com/questions/35582521/how-to-calculate-receptive-field-size](http://stackoverflow.com/questions/35582521/how-to-calculate-receptive-field-size)

```python
# Reference
# http://stackoverflow.com/questions/35582521/how-to-calculate-receptive-field-size
# Please refer to the above url for more detailed information.
# The equation of calculating receptive filed is listed in:
# http://shawnleezx.github.io/blog/2017/02/11/calculating-receptive-field-of-cnn/
# however in the following code, it's implemented in another way, but the result,
# I think it's the same.

# [filter size, stride, padding]

# convnet =[[11,4,0],[3,2,0],[5,1,2],[3,2,0],[3,1,1],[3,1,1],[3,1,1],[3,2,0],[6,1,0]]
# layer_name = ['conv1','pool1','conv2','pool2','conv3','conv4','conv5','pool5','fc6-conv']
# imsize = 227

# Used to calculate the SRCNN's receptive filed.
# SRCNN net
convnet = [[9, 1, 0], [1, 1, 0], [5, 1, 0]]
layer_name = ['conv1', 'conv2', 'conv3']
imsize = 33


def outFromIn(isz, layernum = 9, net = convnet):
    if layernum>len(net): layernum=len(net)

    totstride = 1
    insize = isz
    #for layerparams in net:
    for layer in range(layernum):
        fsize, stride, pad = net[layer]
        outsize = (insize - fsize + 2*pad) / stride + 1
        insize = outsize
        totstride = totstride * stride

    RFsize = isz - (outsize - 1) * totstride

    return outsize, totstride, RFsize

if __name__ == '__main__':

    print "layer output sizes given image = %dx%d" % (imsize, imsize)
    for i in range(len(convnet)):
        p = outFromIn(imsize,i+1)
        print "Layer Name = %s, Output size = %3d, Stride = % 3d, RF size = %3d" % (layer_name[i], p[0], p[1], p[2])
```

