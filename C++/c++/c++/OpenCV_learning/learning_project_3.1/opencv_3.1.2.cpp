#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main()
{
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        cerr << "错误：无法打开摄像头" << endl;
        return -1;
    }
    
    const int numFrames =100;
    Mat frame;
    
    int actualFrames = 0;
    int frameCount = 0;
    double startTick = static_cast<double>(getTickCount());
    double fps = 0;

    namedWindow("Camera",WINDOW_AUTOSIZE);
    
    while(true)
    {
        cap >> frame;

        if(frame.empty())
        {
            cout << "完" << endl;
            break;
        }

        frameCount++;

        double nowTick = static_cast<double>(getTickCount());
        double elapsed = (nowTick - startTick) / getTickFrequency(); // 秒

        if (elapsed >= 1.0) {
            fps = frameCount / elapsed;
            frameCount = 0;
            startTick = nowTick;
        }

        string fpsText = "fps" + to_string(fps);
        double fontScale = 1.0;
        int thickness = 2;
        Scalar color(255,255,255);
        Point org(10,30);

        putText(frame,fpsText,org,FONT_HERSHEY_SIMPLEX,fontScale,color,thickness);
    

        imshow("Prologue",frame);

        int key = waitKey(30);
        if(key == 27)
        {
            cout << "退出" << endl;
            break;
        }
    }
    
    cap.release();
    destroyAllWindows();
    return 0;
}