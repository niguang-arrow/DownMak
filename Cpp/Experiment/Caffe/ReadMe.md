# Caffe Convert Data

## 2017 年 11 月 21 日

### Convert image files

这里谈一下使用 Caffe 的 C++ API 处理图像的方法, 下面的例子中使用的就是参考 Caffe 自带的 `convert_imagenet.cpp` 文件, 但是经我处理后, 成了 `convert_image.cpp` 用于图像文件夹的处理.

#### Prerequisites

如果要成功编译下面的例子, 需要安装必要的库, 如果能在 Ubuntu 上成功安装 Caffe, 那么下面的例子就能正常编译. 参照 [http://caffe.berkeleyvision.org/installation.html](http://caffe.berkeleyvision.org/installation.html) 将 Caffe 安装好. Caffe 本身便依赖大量的第三方库, 比如:

+   Google 的全家桶: leveldb, protobuf, glog 等
+   Boost 库
+   OpenCV 库
+   Intel MKL 或者 Atlas

具体安装方式参考上面的链接, 另外, 编译 Caffe 时, 如果没有 GPU 而是在 CPU 下编译的话, 需要在 `Makefile.config` 中设定 `CPU_ONLY := 1`.

#### Source files

+   下面先看一下当前我的目录下, 有哪些文件:

    ```bash
    caffe/  # caffe 中的 include 文件
    Data/   # 图像资源, 里面可以放置子文件夹, 仿造 Imagenet 的目录结构,
            # 由于是实验, 我只放了一张图像, 即 Data/Figures/1.png
    leveldb/  # leveldb 文件的根目录
    convert_image.cpp  # 这就是源文件了
    train.txt  # 里面记录图像资源的位置, 以及相应的 label
    libcaffe.a  # caffe 编译后得到的库文件
    Makefile   # 用于编译 convert_image.cpp
    ```

#### Detail

下面详细介绍上面的各个文件.

+   `caffe/`: Caffe 的所有头文件

+   `Data/`: 保存所有的图像, `Data/` 是根目录, 里面可以有很多子目录, 然后每个子目录中是具体的图像, 比如 `Data/Figures/1.png`

+   `leveldb/`: 保存 leveldb 文件

+   `train.txt`: 保存图像的位置信息以及 label, 比如:

    ```bash
    # train.txt
    # 不用加上根目录
    Figures/1.png  7
    ```

+   `libcaffe.a` 编译 Caffe 后就可以得到这个文件.

+   下面是重点: `convert_image.cpp`:

    ```cpp
    // convert_image.cpp
    #include <iostream>
    #include <fstream>
    #include <algorithm>
    #include <string>
    #include <leveldb/db.h>
    #include <leveldb/write_batch.h>


    #include "caffe/proto/caffe.pb.h"
    #include "caffe/util/io.hpp"


    using namespace caffe;
    using std::pair;
    using std::string;
    using std::cout;
    using std::endl;

    int main(int argc, char** argv) {
        if (argc < 4) {
            printf("Convert a set of images to the leveldb format used\n"
                    "as input for Caffe.\n"
                    "Usage:\n"
                    "    convert_imageset ROOTFOLDER/ LISTFILE DB_NAME"
                    " RANDOM_SHUFFLE_DATA[0 or 1]\n"
                    "The ImageNet dataset for the training demo is at\n"
                    "    http://www.image-net.org/download-images\n");
            return 0;
        }
        std::ifstream infile(argv[2]); // argv[2] 就是 train.txt
        std::vector<std::pair<string, int> > lines;
        string filename;
        int label;
        while (infile >> filename >> label) { // 将 train.txt 中的每一行读入到 lines 中
            lines.push_back(std::make_pair(filename, label));
        }
        if (argc == 5 && argv[4][0] == '1') { // 最后一个参数我们不指定
            // randomly shuffle data
            std::random_shuffle(lines.begin(), lines.end());
        }

        leveldb::DB* db;
        leveldb::Options options;
        options.error_if_exists = true;
        options.create_if_missing = true;
        options.write_buffer_size = 268435456;
        cout << "Opening leveldb " << argv[3] << endl;
        leveldb::Status status = leveldb::DB::Open( // 创建 leveldb 文件, 保存到 argv[3]: leveldb/train_image_level 中
                options, argv[3], &db);

        string root_folder(argv[1]); // argv[1] 为图像的根目录, 即 Data/
        Datum datum;
        int count = 0;
        const int maxKeyLength = 30; // 原来的代码是 256, 由于实验只有一张图像, 所以改小了, 如果处理 imagenet, 文件数量多, 就改回来.
        char key_cstr[maxKeyLength];
        leveldb::WriteBatch* batch = new leveldb::WriteBatch();
        int data_size;
        bool data_size_initialized = false;

        for (int line_id = 0; line_id < lines.size(); ++line_id) {
            if (!ReadImageToDatum(root_folder + lines[line_id].first, lines[line_id].second,
                        &datum)) {  // 将图像一张一张读入到 Datum 中
                continue;
            };
            if (!data_size_initialized) {
                data_size = datum.channels() * datum.height() * datum.width();
                data_size_initialized = true;
            } else {
                const string& data = datum.data();
            }
            // sequential
            snprintf(key_cstr, maxKeyLength, "%08d_%s", line_id, lines[line_id].first.c_str());
            string value;
            // get the value
          	// 这里是我加的, 可以看到 1.png 这张图像的某些信息
            cout << "Data channels: " << datum.channels() << "\n"
                << "Width: " << datum.width() << "\n"
                << "height: " << datum.height() << "\n"
                << "label: " << datum.label() << endl;

            datum.SerializeToString(&value);
            batch->Put(string(key_cstr), value);
            if (++count % 1000 == 0) {
                db->Write(leveldb::WriteOptions(), batch);
                delete batch;
                batch = new leveldb::WriteBatch();
            }
        }
        // write the last batch
        if (count % 1000 != 0) {
            db->Write(leveldb::WriteOptions(), batch);
        }

        delete batch;
        delete db;
        return 0;
    }
    ```

+   下面是第二个重点, 编译这个文件, 给出 Makefile:

    ```bash
    CC = clang++
    CFLAGS = -Wall -std=c++0x -I./caffe -I. -DCPU_ONLY
    CLINK = -lprotobuf -lglog -lleveldb -lopencv_core -lopencv_highgui \
    		-lopencv_imgproc -lopencv_imgcodecs -lboost_system libcaffe.a


    NAME = convert_image
    SRC = $(NAME).cpp
    TAR = $(NAME).out


    $(TAR) : $(SRC)
    	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)


    .PHONY : clean run

    clean :
    	rm -rf $(TAR)


    run :
    	./$(TAR) Data/ train.txt ./leveldb/train_image_leveldb
    ```

    +   `CC` 可以换成 g++
    +   注意 `CFLAGS` 中需要指定 `-I./caffe`, 否则 `caffe/` 中有些头文件没法找到其他头文件.
    +   还需要注意, 由于目前我的 Caffe 是在 CPU 下编译的, 需要使用 `-DCPU_ONLY`, 否则会因为找不到 `cublas_v2.h` 而报错(没装 cuda)
    +   在 `CLINK` 中, 需要指定 `protobuf`, `glog`, `leveldb`, `opencv` 等库, `boost_system`, 以及最后的 `libcaffe.a`
    +   最后在 `run` 下, 指定程序的参数即可.

+   比如我成功编译并运行后得到:

    ```bash
    Opening leveldb ./leveldb/train_image_leveldb
    Data channels: 3
    Width: 593
    height: 476
    label: 7
    ```

    ​