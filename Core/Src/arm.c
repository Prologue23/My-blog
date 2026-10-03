#include "arm.h"
#include "math.h"

//ARM_INIT  初始化空间角度
#define YAW_INIT            90   
#define PITCH_1_INIT        0
#define PITCH_2_INIT        0
#define PITCH_3_INIT        0
#define CLAW_INIT           0

//空间位置初始化
#define RHO_INIT            0
#define HEI_INIT            0


//ARM_CHANGE
#define YAW_CHANGE          8
#define CLAW_CHANGE         180

//ARM_LENTH
#define ARM_LENTH_1         100
#define ARM_LENTH_2         100

//空间变化
#define STEP                3


#define PI 3.14159265358979323846



//空间位置
int last_Rho        =       0;   
int last_Height     =       0;


/*
ρ=L1 * ​cosθ1 + L2 * ​cos(θ1 + θ2)
z=L1 * ​sinθ1 + L2 * ​sin(θ1 + θ2)
*/


float angle[5] = {0,0,0,0,0}; //顺序为yaw，pitch_1，pitch_2，pitch_3，claw  描述的是舵机角度
float degree[5] = {0,0,0,0,0}; //顺序为yaw，pitch_1，pitch_2，pitch_3，claw 描述的是实际空间角



void arm_Init(void)
{
    degree[0]   = YAW_INIT;
    degree[1]   = PITCH_1_INIT;
    degree[2]   = PITCH_2_INIT;
    degree[3]   = PITCH_3_INIT;
    degree[4]   = CLAW_INIT;
    last_Rho    = RHO_INIT;
    last_Height = HEI_INIT;
}

void arm_Move_up(void)
{
    last_Height += STEP;
    arm_Move_to(last_Rho, last_Height);
}

void arm_Move_down(void)
{
    last_Height -= STEP;
    arm_Move_to(last_Rho, last_Height);
}

void arm_Move_left(void)
{
    degree[0] += YAW_CHANGE;
}

void arm_Move_right(void)
{
    degree[0] -= YAW_CHANGE;
}

void arm_Move_forward(void)
{
    last_Rho += STEP;
    arm_Move_to(last_Rho, last_Height);
}

void arm_Move_back(void)
{
    last_Rho -= STEP;
    arm_Move_to(last_Rho, last_Height);
}

void arm_Claw_change(void)
{
    if (degree[4] <= 10)
    {
        degree[4] = 180;
    }
    else if (degree[4] >= 170)
    {
        degree[4] = 0;
    }
}

void arm_Move_to(int rho, int height)
{
    float xw = rho;
    float zw = height;
    float R  = sqrtf(xw * xw + zw * zw);
    if (R > (ARM_LENTH_1 + ARM_LENTH_2) || R < fabsf(ARM_LENTH_1 - ARM_LENTH_2)) {
        // The target position is unreachable
        return;
    }
    float angle_2 = acosf((R * R - ARM_LENTH_1 * ARM_LENTH_1 - ARM_LENTH_2 * ARM_LENTH_2) / (2 * ARM_LENTH_1 * ARM_LENTH_2));
    float angle_1_1 = atan2f(zw, xw) - atan2f(ARM_LENTH_2 * sinf(angle_2), ARM_LENTH_1 + ARM_LENTH_2 * cosf(angle_2));
    float angle_1_2 = atan2f(zw, xw) - atan2f(-ARM_LENTH_2 * sinf(angle_2), ARM_LENTH_1 + ARM_LENTH_2 * cosf(angle_2));
    if ((fabsf(degree[1] - angle_1_1 * 180 / PI) + fabsf(degree[2] - angle_2 * 180 / PI)) 
    <   (fabsf(degree[1] - angle_1_2 * 180 / PI) + fabsf(degree[2] - -angle_2 * 180 / PI))) 
    {
        degree[1] = angle_1_1 * 180 / PI;
        degree[2] = angle_2 * 180 / PI;
        degree[3] = - (degree[1] + degree[2]);
    } 
    else 
    {
        degree[1] = angle_1_2 * 180 / PI;
        degree[2] = -angle_2 * 180 / PI;
        degree[3] = - (degree[1] + degree[2]);
    }
}
