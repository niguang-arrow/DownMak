# Run mnist example in Tiny-dnn examples

## Run With GCC

If you want to compile successfully `train.cpp` in mnist example, you must
use the following command:

```bash
g++ -std=c++11 -I../.. -o main -Wl,--no-as-needed train.cpp -lpthread
```

Reference: [https://stackoverflow.com/questions/19463602/compiling-multithread-code-with-g](https://stackoverflow.com/questions/19463602/compiling-multithread-code-with-g)

Or you'll get the error: `Enable multithreading to use std::thread: Operation not permitted Aborted (core dumped).`



## Tiny-dnn v0.0.1 test

```bash
git clone https://github.com/tiny-dnn/tiny-dnn
cd tiny-dnn
git checkout v0.0.1
cd test
ls 
	picotest
	test.cpp
	testhelper.h
	test_network.h
	test_convolutional_layer.h
    ...
    
# 打开 test.cpp, 除了 #include "test_network.h" 外其他的 include 都
# 注释掉, 目前只先只测试 network

# 首先注意到该目录下有个 picotest 文件夹, 但是里面没有文件, 所以需要找到相应的文件
rm -rf picotest
git clone https://github.com/nyanp/picotest

# 之后编译 test.cpp, 注意 -Wl,--no-as-needed 和 -lpthread 都是必须的
g++ -Wall -std=c++0x -I../ -o main -Wl,--no-as-needed test.cpp  -lpthread

# 运行
./main

network:[ PASSED ] 
1 tests success.
```

+   另外, 我写了一个 Makefile 用于编译 test.cpp:

```bash
# Makefile
CC = g++
COMPILE_FLAGS = -Wall -std=c++0x -I../
CFLAGS = -Wl,--no-as-needed -lpthread

SRC = test.cpp

# $@ 表示 : 左边的所有内容, $^ 表示 : 右边的内容, 
# $< 表示 : 右边的第一个内容
# .PHONY 表示虚假的 target, 对于 clean, install 等最好使用它
# 使用 make run 运行 main 可执行文件
main : $(SRC)
	$(CC) $(COMPILE_FLAGS) -o $@ $^ $(CFLAGS)

.PHONY : clean

clean :
	rm -rf main

.PHONY : run

run :
	./main
```

关于 .PHONY 的解释看 https://stackoverflow.com/a/3574387 中的回答.