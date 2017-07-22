#include<iostream>
#include<string>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>


using namespace cv;


// Global Variables
const int NUMBER = 100;
const int DELAY = 5;

const int window_width = 900;
const int window_height = 600;
int x_1 = -window_width/2;
int x_2 = window_width*3/2;
int y_1 = -window_width/2;
int y_2 = window_width*3/2;

static Scalar randomColor( RNG& rng );
int Displaying_Random_Text(Mat image, const std::string& window_name, RNG rng);

int main(){

    int c;
    std::string window_name = "Drawing";
    
    RNG rng(0xFFFFFFFF);

    Mat image = Mat::zeros(window_height, window_width, CV_8UC3);
    imshow(window_name, image);
    waitKey(DELAY);

    c = Displaying_Random_Text(image, window_name, rng);
    if(c != 0) return 0;
    
    std::cout << c << std::endl;
    waitKey(0);
    return 0;
}


static Scalar randomColor( RNG& rng )
{
  int icolor = (unsigned) rng;
  return Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
}


int Displaying_Random_Text(Mat image, const std::string& window_name, RNG rng){

    int lineType = 8;

    for(int i = 1; i < NUMBER; i++){
        Point org;
        org.x = rng.uniform(x_1, x_2);
        org.y = rng.uniform(y_1, y_2);

        putText(image, "Testing text rendering", org, rng.uniform(0, 8),
                rng.uniform(0, 100) * 0.05 + 0.1, randomColor(rng), rng.uniform(1, 10), lineType);
        imshow(window_name, image);
        //if(waitKey(DELAY) >= 0) return -1;
    }
    return 0;
}
