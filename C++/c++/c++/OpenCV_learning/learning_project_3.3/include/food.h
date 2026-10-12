#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

class Food 
{
    public:
        Food(int grid_size, int block_size);
        void generate(const vector<Point>& snake_body);
        const Point& getPosition() const;

    private:
        Point position;
        int grid_size;
        int block_size;
};