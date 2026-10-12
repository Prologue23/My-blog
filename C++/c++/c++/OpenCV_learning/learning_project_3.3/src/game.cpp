#include "game.h"
#include <iostream>
#include <cmath>
using namespace cv;
using namespace std;

Game::Game(int width, int height, int grid_size, int block_size)
    : width(width), height(height), grid_size(grid_size), 
      block_size(block_size), running(true), score(0),
      snake(grid_size, block_size), food(grid_size, block_size) 
{
    canvas = Mat(height, width, CV_8UC3, Scalar(15, 15, 15));
    food.generate(snake.getBody());
}

void Game::run() 
{
    while (running) 
    {
        processInput();
        update();
        render();
    }
}

void Game::processInput() 
{
    int key = waitKey(300-(20*log(score+1)));
    
    switch(key) 
    {
        case 'w':case 82: snake.changeDirection(Snake::UP); break;
        case 's':case 84: snake.changeDirection(Snake::DOWN); break;
        case 'a':case 81: snake.changeDirection(Snake::LEFT); break;
        case 'd':case 83: snake.changeDirection(Snake::RIGHT); break;
        case 'r': reset(); break;
        case 27: running = false; break; 
        case 'q': running = false; break;
    }
}

void Game::update() 
{
    snake.move();
    
    
    if (snake.checkCollision(grid_size, grid_size) || snake.checkSelfCollision()) 
    {
        reset();
        return;
    }
    
    
    if (snake.checkFoodCollision(food.getPosition())) 
    {
        snake.grow();
        food.generate(snake.getBody());
        score++;
    }
}

void Game::render() 
{
    
    canvas.setTo(Scalar(15, 15, 15));
    
    
    for (int i = 0; i <= grid_size; ++i)
    {
        line(canvas, 
                 Point(0, i * block_size), 
                 Point(width, i * block_size), 
                 Scalar(40, 40, 40));
        line(canvas, 
                 Point(i * block_size, 0), 
                 Point(i * block_size, height), 
                 Scalar(40, 40, 40));
    }
    
    
    rectangle(canvas,
                 Rect(food.getPosition().x * block_size, 
                        food.getPosition().y * block_size,
                        block_size, block_size),
                 Scalar(0, 0, 255), 
                 FILLED);
    
    
    const auto& body = snake.getBody();
    for (size_t i = 0; i < body.size(); ++i) 
    {
        Scalar color = (i == 0) ? Scalar(0, 255, 0) : Scalar(0, 180, 0);
        rectangle(canvas,
                Rect(body[i].x * block_size, 
                    body[i].y * block_size,
                    block_size, block_size),
                    color,
                    FILLED);
    }
    
    
    string score_text = "Score: " + to_string(score);
    putText(canvas, score_text, Point(10, 30),
               FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 255, 255), 2);
    
    imshow("Elegant Snake Game", canvas);
}

void Game::reset() 
{
    snake = Snake(grid_size, block_size);
    food.generate(snake.getBody());
    score = 0;
}