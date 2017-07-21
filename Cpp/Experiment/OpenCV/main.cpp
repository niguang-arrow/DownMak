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
