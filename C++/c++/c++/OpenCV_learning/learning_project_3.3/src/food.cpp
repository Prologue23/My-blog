#include "food.h"
#include <random>
using namespace cv;
using namespace std;

Food::Food(int grid_size, int block_size) 
    : grid_size(grid_size), block_size(block_size) 
{
    position = Point(-1, -1); 
}

void Food::generate(const vector<Point>& snake_body) 
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, grid_size - 1);
    
    while (true) 
    {
        position = Point(dist(gen), dist(gen));
        
        
        bool onSnake = false;
        for (const auto& segment : snake_body) 
        {
            if (segment == position) 
            {
                onSnake = true;
                break;
            }
        }
        
        if (!onSnake) break;
    }
}

const Point& Food::getPosition() const 
{
    return position;
}