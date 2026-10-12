#include "game.h"


int main() 
{
    Game game(800, 800, 40, 20); // 窗口600x600，20x20网格，每格30像素
    game.run();
    return 0;
}