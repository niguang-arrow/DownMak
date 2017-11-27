# Chapter2 习题解答

## 2017 年 11 月 27 日

2.1 在 Unix 中有一个 w 命令, 这个命令与 who 有关, 运行这个命令, 并阅读他的联机文档, 找出它提供了哪些 who 没有提供的信息? 其中有哪些信息来自 utmp 这个文件? 这些信息各有什么含义? 其他信息来自哪里?

解答: 首先注意 who 和 w 命令功能的不同, 根据 man 手册, who 命令可以显示当前谁登录了系统, 但是 w 更强大一些, 不仅可以显示当前登录系统的用户, 还可以显示他们运行了什么程序(进程). 其中用户的登录信息可以在 `/var/run/utmp` 中查找到, 而对应的进程信息需要在 `/proc` 文件中找到.

2.3 做个实验, 把一个文件复制到 `/dev/tty`: `cp1 cp1.c /dev/tty`. 这时复制的目标文件是一个终端. 对终端的读写操作与对一个普通文件进行读写是一样的. 接下来做实验, 从终端读, 这是会从键盘读字符, 然后写入到文件中, 输入是以回车 + `<Ctrl + D>`  作为结束标志的.

解答: 

先给出编译的 Makefile:

```bash
CC = clang
CFLAGS = -Wall -ansi

NAME = cp
SRC = $(NAME).c
TAR = $(NAME).out

$(TAR) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $^


.PHONY : clean

clean :
	rm -rf *.out

.PHONY : run

run :
	./$(TAR)
```

将文件内容复制到终端: (输出文件使用 open)

```c
#include <stdio.h>
#include <fcntl.h>
#include <utmp.h>
#include <stdlib.h>
#include <unistd.h>


#define BUFFERSIZE 4096
#define FILEMODE 0644

int main(int argc, char *argv[]) {
    int in_fd, out_fd;
    int n_char;
    int buf[BUFFERSIZE];

    if (argc != 3) {
        fprintf(stderr, "usage: %s source destination\n", *argv);
        exit(1);
    }

    if ((in_fd = open(argv[1], O_RDONLY)) == -1) {
        perror(argv[1]);
        exit(1);
    }

    if ((out_fd = open(argv[2], O_WRONLY, FILEMODE)) == -1) {
        perror(argv[2]);
        exit(1);
    }

    while ((n_char = read(in_fd, &buf, BUFFERSIZE)) != 0) {
        if (write(out_fd, &buf, n_char) != n_char) {
            perror("Write Error");
            exit(1);
        }
    }

    if (close(in_fd) == -1 || close(out_fd) == -1) {
        perror("Close error");
        exit(1);
    }

    exit(EXIT_SUCCESS);
}

```

从终端读取字符写入到文件中: (输出文件使用 create)

```c
#include <stdio.h>
#include <fcntl.h>
#include <utmp.h>
#include <stdlib.h>
#include <unistd.h>


#define BUFFERSIZE 4096
#define FILEMODE 0644

int main(int argc, char *argv[]) {
    int in_fd, out_fd;
    int n_char;
    int buf[BUFFERSIZE];

    if (argc != 3) {
        fprintf(stderr, "usage: %s source destination\n", *argv);
        exit(1);
    }

    if ((in_fd = open(argv[1], O_RDONLY)) == -1) {
        perror(argv[1]);
        exit(1);
    }

    /*if ((out_fd = open(argv[2], O_WRONLY, FILEMODE)) == -1) {*/
        /*perror(argv[2]);*/
        /*exit(1);*/
    /*}*/
    if ((out_fd = creat(argv[2], FILEMODE)) == -1) {
        perror(argv[2]);
        exit(1);
    }

    while ((n_char = read(in_fd, &buf, BUFFERSIZE)) != 0) {
        if (write(out_fd, &buf, n_char) != n_char) {
            perror("Write Error");
            exit(1);
        }
    }

    if (close(in_fd) == -1 || close(out_fd) == -1) {
        perror("Close error");
        exit(1);
    }

    exit(EXIT_SUCCESS);
}

```



2.4 标准 C 函数如 fopen, getc, fclose, fgets 的实现包含内核级的缓冲. 它们用到了一个结构 FILE, 并以此为基础构造了类似 utmplib (前面的内容讲了)的中间层,在头文件中找到 FILE 的成员描述, 将其与 utmplib.c 中的变量做比较.

解答: 参看 https://code.woboq.org/userspace/glibc/libio/bits/types/FILE.h.html, 太复杂了, 不解释.



2.5 怎么来确定数据已经被写到磁盘上(不是被缓冲)? 采用缓冲技术, 内核会把要写入磁盘的数据放在缓冲区, 然后在它认为合适的时候写入磁盘. 阅读相应的联机帮助, 找到能够确定地把数据写入磁盘的方法.

解答: 查帮助的时候, 如果直接输入 `man write`, 是得不到 `write()` 函数相关的信息的, 而是某个名为 `write` 的程序. 此时就应该指定特定的章节, 比如 `man 2 write`, 就能找到了.

根据 man 文档中最后的 Notes, 要想写入到磁盘中, 唯一的方式是在将数据写入缓冲区后调用 **fsync** 函数.































