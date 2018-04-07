#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include "BackgroundSubtractorPAWCS.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
using namespace boost::python;
using namespace cv;
using namespace std;

// "/home/ieric/Codes/Qt5/pcb/figs/1.jpg"
void Seg(char path[]) {
	cv::Mat oCurrInputFrameInit, oCurrSegmMask, oCurrReconstrBGImg;
	oCurrInputFrameInit = imread("baseImage.jpg");

	Rect ROI(403, 452, 1476, 698);
	Mat oCurrInputFrame_i(oCurrInputFrameInit, ROI);
	Mat oCurrInputFrame;
	oCurrInputFrame_i.copyTo(oCurrInputFrame);

    oCurrSegmMask.create(oCurrInputFrame.size(),CV_8UC1);
    oCurrReconstrBGImg.create(oCurrInputFrame.size(),oCurrInputFrame.type());
    cv::Mat oSequenceROI(oCurrInputFrame.size(),CV_8UC1,cv::Scalar_<uchar>(255)); // for optimal results, pass a constrained ROI to the algorithm (ex: for CDnet, use ROI.bmp)

    BackgroundSubtractorPAWCS oBGSAlg;
    oBGSAlg.initialize(oCurrInputFrame,oSequenceROI);
    for(int k=0; k < 9;++k) {

		char filename_i[128];
		sprintf(filename_i, "%s", path);
		Mat oCurrInputFrame_i = imread(filename_i);
		if (oCurrInputFrame_i.empty())
			break;
		
		Mat oCurrInputFrame_c(oCurrInputFrame_i, ROI);
		Mat oCurrInputFrame;
		oCurrInputFrame_c.copyTo(oCurrInputFrame);

        oBGSAlg.apply(oCurrInputFrame,oCurrSegmMask,double(k<=100)); //		
        oBGSAlg.getBackgroundImage(oCurrReconstrBGImg);
    }
    char filename[1024];
    sprintf(filename, "%s", "result.jpg");
    cv::imwrite(filename, oCurrSegmMask);
}

BOOST_PYTHON_MODULE(Segmentation)
{
    def("Seg", Seg);
}
