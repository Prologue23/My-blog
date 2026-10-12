#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;

class Snake 
{
    public:
        enum Direction { UP, DOWN, LEFT, RIGHT };

        Snake(int grid_size, int block_size);
        void move();
        void grow();
        void changeDirection(Direction new_dir);
        bool checkCollision(int width, int height) const;
        bool checkSelfCollision() const;
        bool checkFoodCollision(const Point& food_pos) const;
        const vector<Point>& getBody() const;
        Point getHead() const;

    private:
        vector<cv::Point> body;
        Direction dir;
        int grid_size;
        int block_size;
};