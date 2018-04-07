# Readme

2018 年 4 月 7 日

## 使用 Boost::Python 来扩展 Python

+ [利用C++ Boost编写扩展Python模块](https://blog.csdn.net/kkk584520/article/details/40889691)
+ [简书: boost.python笔记](https://www.jianshu.com/p/0fee49c58caa) (更详细)
+ https://www.cvl.isy.liu.se/education/graduate/opencv/Lecture8_PythonWrappers.pdf 这个 ppt 比较拽, 暂时不愿看.
+ http://www.it1352.com/478187.html 这个代码非常详细, boost 和 numpy 以及 opencv 的结合.
+ [Exposing C++ OpenCV code to Python using Boost](http://www.ysagade.nl/2013/05/11/numpy-boost-python/)
+ [Github: NUMPY <=> OpenCV's cv::Mat converter](https://github.com/spillai/numpy-opencv-converter)
+ https://github.com/zmlhome/DownMak/tree/master/Cpp/Experiment/PyCAPI 我的笔记



## Makefile 的使用

注意它不是用来编译 Segmentation.cpp 的, Makefile 用来编译的代码没有传上来. 编译 Segmentation.cpp 的方式见下面, 另外, 额外的代码也没有传上来.



## 编译 Segmentation.cpp

使用下面的命令编译 Segmentation.cpp

```bash
clang++ -pthread -fPIC -shared -std=c++0x -Wall -I/home/ieric/.pyenv/versions/2.7.13/include/python2.7 -o test.so test.cpp -lboost_python `pkg-config --libs opencv` BackgroundSubtractorPAWCS.cpp BackgroundSubtractorLBSP.cpp LBSP.cpp
```

在当前目录下生成 `Segmentation.so` 文件, 在 Python 中使用:

**必须注意, 当前目录下需要有 baseImage.jpg** 光板图片.

```bash
>>> import Segmentation
>>> Segmentation.Seg("/home/ieric/Codes/Qt5/pcb/figs/2.jpg")
# 在当前目录中产生 result.jpg
```



## rectangle.m

传入 `result.jpg` 的路径, 然后元器件的位置, 为一个二维的矩阵, `[N, 4]`, 4 表示 `(x, y, w, h)`, N 一般为 6.

在 Python 中使用 mlabwrap 调用该函数.



## Python 运行 matlab 文件

使用 mlabwrap.

推荐看: 

+ [Stackoverflow : Run a MATLAB script from python + pass args](https://stackoverflow.com/questions/13311415/run-a-matlab-script-from-python-pass-args)
+ [Calling MATLAB .m-files and functions in Python script](https://stackoverflow.com/questions/13457751/calling-matlab-m-files-and-functions-in-python-script)
+ https://blog.csdn.net/houchaoqun_xmu/article/details/53948647



## mlabwrap 的安装

主要说下可能出现的问题:

+ `can't start matlab engine` : http://mlabwrap.sourceforge.net/#troubleshooting 官网可以参考. 结论是, 需要安装 csh : `sudo apt-get install csh`.

**发现即使出现下面的问题, 不解决也可以! 不用管 mex 的编译问题.**

+ `error while loading shared libraries: libeng.so` : [Matlab: configuring mex](http://www-h.eng.cam.ac.uk/help/tpl/programs/Matlab/configuringmex.html) 可以参考, 这个问题出现在我们使用 mex 编译 engdemo.c 文件时出现. 使用 `which mex` 查看使用的是那个 mex. 我的 matlab 安装在 `/usr/local/MATLAB/R2015b/bin/` 目录下, engdemo.c 在 `/usr/local/MATLAB/R2015b/extern/examples/` 目录下.

  要编译 engdemo.c, 首先要拷贝到自己的目录中, 否则会提示访问受限.

  ```bash
  cp /usr/local/MATLAB/R2015b/extern/examples/engdemo.c ~/Codes/
  cd ~/Codes
  chmod a+w engdemo.c 
  mex -f /usr/local/MATLAB/R2015b/bin/engopts.sh engdemo.c
  ```

  如果出现 `./engdemo: error while loading shared libraries`, 那么在 .bashrc 文件中加入如下命令:

  ```bash
  # matlab used for mlab(python)
  # if 'can't start matlab engine' occurred, install csh
  export PATH=/usr/local/MATLAB/R2015b/bin:$PATH
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/MATLAB/R2015b/bin/glnxa64:/usr/local/MATLAB/R2015b/sys/os/glnxa64
  ```

  **第二个 export 的内容如果不注释掉, clang++ 等就不能使用; 如果注释掉, 那么 engdemo 的这个问题不能解决...**

+ 其他可能出现的问题:

  + [ImportError: MATLAB/R2015b/bin/glnxa64/libssl.so.1.0.0: undefined symbol: EVP_idea_cbc](https://github.com/imatlab/imatlab/issues/3)

+ 有些价值的参考:

  [how do i compile and run a matlab engine program on unix linux mac](https://cn.mathworks.com/matlabcentral/answers/101334-how-do-i-compile-and-run-a-matlab-engine-program-on-unix-linux-mac)



## Qt 遇到的问题

### 使用 QPainter 画矩形

能在 `result.jpg` 上正确的画出矩形, 但是如果将结果缩放到 zoomArea 上时, 原图能正确缩放, 但是其中的矩形不能正确缩放, 猜想是此时矩形的坐标已经固定了. 目前的解法是先将画了矩形的图保存下来, 然后再重新载入.







