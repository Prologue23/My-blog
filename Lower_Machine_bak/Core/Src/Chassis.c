#include "Chassis.h"
#include "main.h"

#define ROCKER_STEP     10
#define FOOT_STEP       10


extern float angle[7]; //顺序为rocker，foot  描述的是舵机角度


void Chassis_Init()
{
    angle[6] = 0;
    angle[7] = 0;
}


void Rocker_uplift()
{
    angle[6] += ROCKER_STEP;
}


void Rocker_downlift()
{
    angle[6] -= ROCKER_STEP;
}


void Foot_up()
{
    angle[7] += FOOT_STEP;
}

void Foot_down()
{
    angle[7] -= FOOT_STEP;
}


