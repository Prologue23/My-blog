#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    
    Mat canvas(600, 1200, CV_8UC3, Scalar(125, 36, 0));
    
    Mat WhiteRect(200, 1200, CV_8UC3, Scalar(255, 255, 255)); 
    Mat whiteRect(600, 200, CV_8UC3, Scalar(255, 255, 255)); 
    
    Mat redRect(120, 1200, CV_8UC3, Scalar(43, 20, 207)); 
    Mat RedRect(600, 120, CV_8UC3, Scalar(43, 20, 207)); 
    
    line(canvas, Point(-200, -100), Point(1400, 700), Scalar(255, 255, 255), 120);
    line(canvas, Point(-200, 700), Point(1400, -100), Scalar(255, 255, 255), 120);

    line(canvas, Point(-21, 12), Point(579, 312), Scalar(43, 20, 207), 40);
    line(canvas, Point(21, 612), Point(621, 312), Scalar(43, 20, 207), 40);
    line(canvas, Point(1239, 598), Point(619, 288), Scalar(43, 20, 207), 40);
    line(canvas, Point(1191, -12), Point(591, 288), Scalar(43, 20, 207), 40);

    WhiteRect.copyTo(canvas(Rect(0, 200, WhiteRect.cols, WhiteRect.rows)));
    whiteRect.copyTo(canvas(Rect(500, 0, whiteRect.cols, whiteRect.rows)));
    
    redRect.copyTo(canvas(Rect(0, 240, redRect.cols, redRect.rows)));
    RedRect.copyTo(canvas(Rect(540, 0, RedRect.cols, RedRect.rows)));
    
    // 9. 显示结果
    namedWindow("Drawing with copyTo", WINDOW_AUTOSIZE);
    imshow("Drawing with copyTo", canvas);
    
    // 10. 保存结果
    imwrite("drawing_with_copyto.jpg", canvas);
    
    waitKey(0);
    return 0;
}