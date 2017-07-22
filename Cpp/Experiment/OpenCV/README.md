# OpenCV

## 最简单的 opencv 程序

```cpp
#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>


using namespace cv;


int main(){

    Mat img = imread("lena.png");
    namedWindow("Display");
    imshow("Display", img);
    waitKey(0);
}
```

要编译该程序, 需要使用 

```cpp
g++ -Wall -std=c++0x -o main main.cpp -L/usr/local/lib 
           -lopencv_core -lopencv_highgui -lopencv_imgcodecs 
```

+ 对于 `putText()` 函数, 在 OpenCV3 中已经移入 imgproc 模块中 (以前在 core 模块中), 因此
要使用的话, 要使用 `#include<opencv2/imgproc/imgproc.hpp>`, 另外编译时加上 `-lopencv_imgproc` 选项.
