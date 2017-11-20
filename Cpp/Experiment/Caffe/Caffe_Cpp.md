# 使用 Caffe C++ API

## 2017 年 11 月 18 日

### 操作

+ 使用的是 caffe-v0.9

+ 将下面代码命名为 `demo.cpp`, 放在 `caffe/examples/lenet/` 目录下

  ```cpp
  #include <cuda_runtime.h>
  #include <fcntl.h>
  #include <google/protobuf/text_format.h>

  #include <cstring>

  #include "caffe/blob.hpp"
  #include "caffe/common.hpp"
  #include "caffe/net.hpp"
  #include "caffe/filler.hpp"
  #include "caffe/proto/caffe.pb.h"
  #include "caffe/util/io.hpp"
  #include "caffe/solver.hpp"

  #include <iostream>

  #include "caffe/layer.hpp"
  #include "caffe/vision_layers.hpp"
  #include "caffe/util/im2col.hpp"
  #include "caffe/filler.hpp"
  #include "caffe/util/math_functions.hpp"

  using namespace caffe;

  int main(int argc, char** argv) {
    Caffe::set_mode(Caffe::CPU);
    Caffe::set_phase(Caffe::TRAIN);

    Blob<float> input(1, 3, 5, 5);
    vector<Blob<float>*> bottom_vec;

    bottom_vec.push_back(&input);
    
    std::cout << bottom_vec.size() << std::endl;

    NetParameter net_param;
    ReadProtoFromTextFile("/home/ieric/Codes/caffe-v0.9/examples/lenet/lenet.prototxt", &net_param);
    std::cout << net_param.name() << std::endl;
    std::cout << net_param.input_size() << std::endl;
    std::cout << net_param.layers_size() << std::endl;
    std::cout << net_param.has_name() << std::endl;
    std::cout << net_param.input(0) << std::endl;
    
    std::cout << net_param.input_dim_size() << std::endl;
    std::cout << net_param.input_dim(0) << std::endl;
    std::cout << net_param.input_dim(1) << std::endl;
    std::cout << net_param.input_dim(2) << std::endl;
    std::cout << net_param.input_dim(3) << std::endl;

    const LayerConnection& layer_connect = net_param.layers(0);
    const LayerParameter& layer_param = net_param.layers(0).layer();
    
    std::cout << layer_connect.bottom(0) << std::endl;
    std::cout << layer_connect.top(0) << std::endl;
    std::cout << layer_param.name() << std::endl;
    std::cout << layer_param.type() << std::endl;
    std::cout << layer_param.num_output() << std::endl;
    std::cout << layer_param.pad() << std::endl;
    std::cout << layer_param.kernelsize() << std::endl;

    if (!layer_param.has_pool())
        std::cout << layer_param.pool() << std::endl;
    return 0;
  }
  ```

  在 `caffe/` 目录下的 Makefile 中最后面加上如下:

  ```bash
  .PHONY : run

  run :
  	./build/examples/lenet/demo.bin
  ```

  那么每次修改完程序之后, 使用:

  ```bash
  make examples
  ```

  编译程序, 然后使用

  ```bash
  make run
  ```

  来运行程序.