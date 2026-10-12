#pragma once
#include "snake.h"
#include "food.h"
using namespace cv;
using namespace std;

class Game {
public:
    Game(int width = 600, int height = 600, int grid_size = 20, int block_size = 30);
    void run();

private:
    void processInput();
    void update();
    void render();
    void reset();

    Mat canvas;
    int width;
    int height;
    int grid_size;
    int block_size;
    bool running;
    int score;
    Snake snake;
    Food food;
};