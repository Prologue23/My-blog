#include "snake.h"
using namespace cv;
using namespace std;

Snake::Snake(int grid_size, int block_size) 
    : grid_size(grid_size), block_size(block_size), dir(RIGHT) 
    {
    body.push_back(Point(5, grid_size/2));
    body.push_back(Point(4, grid_size/2));
    body.push_back(Point(3, grid_size/2));
    }

void Snake::move() 
{
    
    Point new_head = body[0];
    
    switch(dir) 
    {
        case UP:    new_head.y--; break;
        case DOWN:  new_head.y++; break;
        case LEFT:  new_head.x--; break;
        case RIGHT: new_head.x++; break;
    }
    
    for (int i = body.size() - 1; i > 0; --i) 
    {
        body[i] = body[i-1];
    }
    body[0] = new_head;
}

void Snake::grow() 
{
    body.push_back(body.back());
}

void Snake::changeDirection(Direction new_dir) 
{
    
    if ((dir == UP && new_dir != DOWN) ||
        (dir == DOWN && new_dir != UP) ||
        (dir == LEFT && new_dir != RIGHT) ||
        (dir == RIGHT && new_dir != LEFT)) 
    {
        dir = new_dir;
    }
}

bool Snake::checkCollision(int width, int height) const 
{
    return body[0].x < 0 || body[0].x >= width || 
           body[0].y < 0 || body[0].y >= height;
}

bool Snake::checkSelfCollision() const 
{
    for (size_t i = 1; i < body.size(); ++i) 
    {
        if (body[0] == body[i]) return true;
    }
    return false;
}

bool Snake::checkFoodCollision(const cv::Point& food_pos) const 
{
    return body[0] == food_pos;
}

const vector<Point>& Snake::getBody() const 
{
    return body;
}

Point Snake::getHead() const 
{
    return body[0];
}