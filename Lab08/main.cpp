#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>


using namespace std;
using namespace cv;

void createTrackbars(int &threshold1,int &threshold2, int &hmin, int &smin, int &vmin,int &hmax,int &smax,int &vmax ){

    namedWindow("trackbars", WINDOW_FREERATIO);
    createTrackbar("threshold1", "trackbars", &threshold1, 200);
    createTrackbar("threshold2", "trackbars", &threshold2, 200);

// get ~ red color

    createTrackbar("hmin", "trackbars", &hmin, 255);
    createTrackbar("hmax", "trackbars", &hmax, 255);
    createTrackbar("smin", "trackbars", &smin, 255);
    createTrackbar("smax", "trackbars", &smax, 255);
    createTrackbar("vmin", "trackbars", &vmin, 255);
    createTrackbar("vmax", "trackbars", &vmax, 255);
}
void drawContours(Mat mask,Mat img){
    vector<vector<Point>> contours;
    vector<vector<Point>> max_countours;

//    Wszystkie kontury
    findContours(mask,contours,RETR_LIST,CHAIN_APPROX_SIMPLE);
    sort(contours.begin(), contours.end(), [](vector<Point> firstContour,vector<Point> secondContour){
        double x = fabs(contourArea(Mat(firstContour)));
        double y = fabs(contourArea(Mat(secondContour)));
        return (x<y);
    });
    if(contours.size()>1){
        vector<Point> contour = contours.back();
        vector<Point> contour2 = contours[contours.size()-2];
        vector<vector<Point>> toDraw = {contour,contour2};
        drawContours(img,toDraw,-1,{255,0,255});
        Moments m = moments(contour, false);
        Point p = {(int) (m.m10 / m.m00), (int) (m.m01 / m.m00)};
        Point p_2 = {(int) ((m.m10 / m.m00) - 4), (int) ((m.m01 / m.m00) - 4)};
        auto m1 = moments(contour2, false);
        Point p1 = {(int) (m1.m10 / m1.m00), (int) (m1.m01 / m1.m00)};
        Point p1_2 = {(int) ((m1.m10 / m1.m00) - 4), (int) ((m1.m01 / m1.m00) - 4)};

        line(img, p, p1, Scalar(255, 255, 255), 2, LINE_8);
        line(img, p_2, p1_2, Scalar(0, 0, 0), 2, LINE_8);
    }
}
int main(int argc, char **argv)
{
    VideoCapture cap(1);
    Mat img,greyIMG,mask;
    int threshold1 = 50;
    int threshold2 = 150;
    int hmin = 95,smin = 137,vmin = 88;
    int hmax = 189,smax = 199,vmax = 154;
    createTrackbars(threshold1,threshold2,hmin,smin,vmin,hmax,smax,vmax);
    while(true){
        cap.read(img);


        cvtColor(img,greyIMG,COLOR_BGR2HSV);

        //search colors
        Scalar lower(hmin,smin,vmin);
        Scalar upper(hmax,smax,vmax);
        inRange(greyIMG,lower,upper,mask);

//        imshow("MASK 1",mask);
        Mat kernel = getStructuringElement(MORPH_ELLIPSE,{10,10});
        morphologyEx(mask,mask,MORPH_CLOSE,kernel);
        Canny(mask,mask,100,100);
//        imshow("MASK 2",mask);
        Mat kernel2 = getStructuringElement(MORPH_ELLIPSE,{50,50});
        morphologyEx(mask,mask,MORPH_CLOSE,kernel2);


        drawContours(mask,img);

        imshow("MAIN WINDOWS",img);
        imshow("MASK 3",mask);
        if (waitKey(1) == 27)
            break;
    }

    return 0;
}