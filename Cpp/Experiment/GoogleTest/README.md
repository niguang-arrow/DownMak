# Google Test

## 2017 年 9 月 28 日

### 编译

+   从 https://github.com/google/googletest 下载源文件

    ```bash
    cd ~/Programs
    git clone https://github.com/google/googletest
    cd googletest/googletest
    mkdir build
    cd build
    cmake -Dgtest_build_samples=ON ..
    make
    ```

+   实际上 (Ubuntu Kylin) 已经下载好了 googletest 以及 googlemock, 

    ```bash
    sudo apt-get install libgtest-dev
    ```

    可以在 `/usr/local/include` 中找到 gtest 文件夹以及 `/usr/local/lib` 中找到 `libgtest.a` 以及 `libgtest_main.a` 文件.

+   如果没有自带的话, 只要将第一步中编译成功的文件拷贝到相应的位置即可.

    ```bash
    # 当前在 ${GTEST_DIR} 目录, 即 ~/Programs/googletest/googletest
    sudo cp -r include/gtest /usr/local/include
    cd build
    sudo cp libgtest.a libgtest_main.a -t /usr/local/lib
    # 注意这里是 libgtest.a 和 libgtest_main.a 两个静态库, 如果想
    # 弄成动态库, 可以看 ${GTEST_DIR} 下的 README.md
    ```

### 测试

+   进入 `${GTEST_DIR}/samples/`:

    ```bash
    cd ~/Programs/googletest/googletest/samples

    # 方法一, 注意顺序, 以及 -lpthread 必须有, 否则链接出错
    g++ -Wall -std=c++0x -o main sample1_unittest.cc sample1.cc -lgtest_main -lgtest -lpthread

    # 方法二
    g++ -Wall -std=c++0x -c sample1.cc
    g++ -Wall -std=c++0x -c sample1_unittest.cc
    g++ -Wall -std=c++0x -o main sample1_unittest.o sample1.o -lgtest_main -lgtest -lpthread

    # 测试 
    ./main
    ```

    ​