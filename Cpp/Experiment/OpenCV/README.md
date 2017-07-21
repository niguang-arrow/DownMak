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
