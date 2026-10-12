#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // 1. 创建白色画布 (600x800)
    Mat canvas(600, 900, CV_8UC3, Scalar(255, 255, 255));
    
    // 2. 创建蓝色
    Mat blueRect(200, 900, CV_8UC3, Scalar(255,0, 0)); 
    
    // 3. 创建红色
    Mat redRect(200, 900, CV_8UC3, Scalar(0, 0, 255)); 
    
    // 5. 绘制蓝色矩形到画布
    blueRect.copyTo(canvas(Rect(0, 200, blueRect.cols, blueRect.rows)));
    
    // 6. 绘制红色圆形到画布
    redRect.copyTo(canvas(Rect(0, 400, redRect.cols, redRect.rows)));
    
    // 9. 显示结果
    namedWindow("Drawing with copyTo", WINDOW_AUTOSIZE);
    imshow("Drawing with copyTo", canvas);
    
    // 10. 保存结果
    imwrite("drawing_with_copyto.jpg", canvas);
    
    waitKey(0);
    return 0;
}