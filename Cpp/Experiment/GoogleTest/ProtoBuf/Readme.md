# Google Protocol Buffers

## 2017 年 11 月 17 日

从 Caffe 转过来的...

这里主要介绍一下编译的方法, Google 的教程对 ProtoBuf 介绍的非常详细. 详见 [Protocol Buffer Basics: C++](https://developers.google.com/protocol-buffers/docs/cpptutorial)

当创建好一个 `.proto` 文件, 比如上面链接中给出的名为 `addressbook.proto` 的例子, 之后我们使用:

```bash
protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/addressbook.proto
```

(比如可以设置 `$SRC_DIR` 为 `.` 当前目录), 此时会在 `$SRC_DIR` 目录下产生名为 `addressbook.proto.cc` 和 `addressbook.proto.h` 两个文件, 用于 C++ 程序中. 上面链接中还给出了如何读写 protobuf 格式文件的例子, 我这里给出编译代码的 Makefile 文件:

```bash
INCLUDE_DIR = .

CC = g++
CFLAGS = -Wall -std=c++0x -I$(INCLUDE_DIR)
CLINK = -lprotobuf

NAME = read
SRC = $(NAME).cpp addressbook.pb.cc
TAR = $(NAME).out

$(TAR) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

.PHONY : clean run

run :
	./$(TAR)

clean :
	rm -rf *.out
```