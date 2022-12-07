#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
using namespace std;
using namespace cv;
int main( int argc, char** argv )
{
    using namespace cv;
    VideoCapture vc(1);
    if (!vc.isOpened() ) return 1;
    cv::Mat frame;

    cv::namedWindow("Window", WINDOW_AUTOSIZE); // Create a window for display.
    int i = 0;
    while (waitKey(10) != 27) {
        vc >> frame;
        std::cout << "frame info: " << frame.rows << " " << frame.cols << std::endl;
        cv::imshow("Frame", frame);

    }
    return 0;
}