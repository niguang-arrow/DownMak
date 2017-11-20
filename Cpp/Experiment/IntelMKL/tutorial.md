# Intel Math Kernel Library

## 2017 年 11 月 17 日

### Tutorial

主要介绍一下这个库怎么链接.

mkl 的安装非常简单, 在 [https://software.intel.com/en-us/mkl](https://software.intel.com/en-us/mkl) 可以免费下载, 解压后可以看到有个带 `_GUI` 后缀的安装文件, 运行这个程序可以使用 GUI 界面安装. 所用配置请使用默认配置. 比如默认的安装路径在 `/opt/intel/mkl`.

我是因为 Caffe 才开始接触这个库的, 当时编译 Caffe 曾出现 `libmkl_rt.so` 无法链接的情况, 参看 [ImportError: libmkl_rt.so: cannot open shared object file](https://github.com/davisking/dlib/issues/587) 将问题解决的办法是: 在 `/etc/ld.so.conf.d` 目录创建一个 `mylibs.conf`(名字随便起), 将 `libmkl_rt.so` 所在的路径加到该文件中:

```bash
# libmkl.conf
/opt/intel/compilers_and_libraries_2018.0.128/linux/mkl/lib/ia32_lin
/opt/intel/compilers_and_libraries_2018.0.128/linux/mkl/lib/intel64_lin
```

就是上面两行了, 然后再运行 `sudo ldconfig` 命令即可.

### 继续

下面给出使用 mkl 编程的例子, 其实重点是如何连接 mkl 中的动态库, 下面这个例子在 [Multiplying Matrices Using dgemm](https://software.intel.com/en-us/mkl-tutorial-c-multiplying-matrices-using-dgemm#9CEED00C-1A85-4AC0-8AF8-BE2AFEF0E603), 目的是求两个矩阵的乘积.

```c
#define min(x,y) (((x) < (y)) ? (x) : (y))

#include <stdio.h>
#include <stdlib.h>
#include "mkl.h"

int main()
{
    double *A, *B, *C;
    int m, n, k, i, j;
    double alpha, beta;

    printf ("\n This example computes real matrix C=alpha*A*B+beta*C using \n"
            " Intel(R) MKL function dgemm, where A, B, and  C are matrices and \n"
            " alpha and beta are double precision scalars\n\n");

    m = 2000, k = 200, n = 1000;
    printf (" Initializing data for matrix multiplication C=A*B for matrix \n"
            " A(%ix%i) and matrix B(%ix%i)\n\n", m, k, k, n);
    alpha = 1.0; beta = 0.0;

    printf (" Allocating memory for matrices aligned on 64-byte boundary for better \n"
            " performance \n\n");
    A = (double *)mkl_malloc( m*k*sizeof( double ), 64 );
    B = (double *)mkl_malloc( k*n*sizeof( double ), 64 );
    C = (double *)mkl_malloc( m*n*sizeof( double ), 64 );
    if (A == NULL || B == NULL || C == NULL) {
      printf( "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
      mkl_free(A);
      mkl_free(B);
      mkl_free(C);
      return 1;
    }

    printf (" Intializing matrix data \n\n");
    for (i = 0; i < (m*k); i++) {
        A[i] = (double)(i+1);
    }

    for (i = 0; i < (k*n); i++) {
        B[i] = (double)(-i-1);
    }

    for (i = 0; i < (m*n); i++) {
        C[i] = 0.0;
    }

    printf (" Computing matrix product using Intel(R) MKL dgemm function via CBLAS interface \n\n");
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                m, n, k, alpha, A, k, B, n, beta, C, n);
    printf ("\n Computations completed.\n\n");

    printf (" Top left corner of matrix A: \n");
    for (i=0; i<min(m,6); i++) {
      for (j=0; j<min(k,6); j++) {
        printf ("%12.0f", A[j+i*k]);
      }
      printf ("\n");
    }

    printf ("\n Top left corner of matrix B: \n");
    for (i=0; i<min(k,6); i++) {
      for (j=0; j<min(n,6); j++) {
        printf ("%12.0f", B[j+i*n]);
      }
      printf ("\n");
    }
    
    printf ("\n Top left corner of matrix C: \n");
    for (i=0; i<min(m,6); i++) {
      for (j=0; j<min(n,6); j++) {
        printf ("%12.5G", C[j+i*n]);
      }
      printf ("\n");
    }

    printf ("\n Deallocating memory \n\n");
    mkl_free(A);
    mkl_free(B);
    mkl_free(C);

    printf (" Example completed. \n\n");
    return 0;
}
```

编译这个程序的 Makefile 如下:

```bash
MKL_DIR = /opt/intel/mkl
MKL_INCLUDE_DIR = $(MKL_DIR)/include
MKL_LIB_DIR = $(MKL_DIR)/lib $(MKL_DIR)/lib/intel64_lin

CC = gcc
CFLAGS = -Wall -I$(MKL_INCLUDE_DIR) -L$(foreach libdir,$(MKL_LIB_DIR),-L$(libdir))
CLINK = -lmkl_rt -lmkl_intel_thread -lmkl_core -lpthread


NAME = main
SRC = $(NAME).c
TAR = $(NAME).out


$(TAR) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

.PHONY : clean run

run :
	./$(TAR)

clean :
	rm -rf *.out
```

其中主要是要链接 `libmkl_rt.so`, 其他两个可以不需要.

编译程序后运行得到:

```c
 This example computes real matrix C=alpha*A*B+beta*C using                                                                                                                   
 Intel(R) MKL function dgemm, where A, B, and  C are matrices and                                                                                                             
 alpha and beta are double precision scalars                                                                                                                                  
                                                                                                                                                                              
 Initializing data for matrix multiplication C=A*B for matrix                                                                                                                 
 A(2000x200) and matrix B(200x1000)                                                                                                                                           
                                                                                                                                                                              
 Allocating memory for matrices aligned on 64-byte boundary for better 
 performance
 
 Intializing matrix data
 
 Computing matrix product using Intel(R) MKL dgemm function via CBLAS interface
 
 Computations completed.
   
 Top left corner of matrix A:
           1           2           3           4           5           6
         201         202         203         204         205         206
         401         402         403         404         405         406
         601         602         603         604         605         606
         801         802         803         804         805         806
        1001        1002        1003        1004        1005        1006

 Top left corner of matrix B:
          -1          -2          -3          -4          -5          -6
       -1001       -1002       -1003       -1004       -1005       -1006
       -2001       -2002       -2003       -2004       -2005       -2006
       -3001       -3002       -3003       -3004       -3005       -3006
       -4001       -4002       -4003       -4004       -4005       -4006
       -5001       -5002       -5003       -5004       -5005       -5006
            
 Top left corner of matrix C:
 -2.6666E+09 -2.6666E+09 -2.6667E+09 -2.6667E+09 -2.6667E+09 -2.6667E+09
 -6.6467E+09 -6.6467E+09 -6.6468E+09 -6.6468E+09 -6.6469E+09  -6.647E+09
 -1.0627E+10 -1.0627E+10 -1.0627E+10 -1.0627E+10 -1.0627E+10 -1.0627E+10
 -1.4607E+10 -1.4607E+10 -1.4607E+10 -1.4607E+10 -1.4607E+10 -1.4607E+10
 -1.8587E+10 -1.8587E+10 -1.8587E+10 -1.8587E+10 -1.8588E+10 -1.8588E+10
 -2.2567E+10 -2.2567E+10 -2.2567E+10 -2.2567E+10 -2.2568E+10 -2.2568E+10

 Deallocating memory

 Example completed.           
```

