
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
using namespace std;
using namespace cv;
int main( int argc, char** argv )
{
    VideoCapture vc(0);
    cv::namedWindow("Okienko Prawie Inteligentne", WINDOW_AUTOSIZE);
    cv::waitKey(0);
    return 0;
}