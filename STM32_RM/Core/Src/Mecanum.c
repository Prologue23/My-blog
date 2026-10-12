#include "Mecanum.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;


static const GPIO_TypeDef*  IN1_PORT[4]      = {AIN1_GPIO_Port, BIN1_GPIO_Port, CIN1_GPIO_Port, DIN1_GPIO_Port};
static const GPIO_TypeDef*  IN2_PORT[4]      = {AIN2_GPIO_Port, BIN2_GPIO_Port, CIN2_GPIO_Port, DIN2_GPIO_Port};
static const uint16_t       IN1_PIN[4]       = {AIN1_Pin, BIN1_Pin, CIN1_Pin, DIN1_Pin};
static const uint16_t       IN2_PIN[4]       = {AIN2_Pin, BIN2_Pin, CIN2_Pin, DIN2_Pin};
static const uint32_t       PWM_CH[4]        = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4};

int wheel_direction[4];
static int16_t  speed = 0;
static uint8_t  last_cmd = 0;
static uint32_t last_msg_time = 0;

#define SPEED_LIMIT_GEAR0    50
#define SPEED_LIMIT_GEAR1    255


void Mecanum_Init()
{
    for (int i = 0; i < 4; i++)
    {
        wheel_direction[i] = 0;
    }
}

void Mecanum_SetDirection(uint8_t cmd)
{
    switch(cmd)
    {
        case 0x01:
            wheel_direction[0] =  1; 
            wheel_direction[1] =  1;
            wheel_direction[2] =  1; 
            wheel_direction[3] =  1;
            break;
        case 0x02:
            wheel_direction[0] = -1; 
            wheel_direction[1] = -1;
            wheel_direction[2] = -1; 
            wheel_direction[3] = -1;
            break;
        case 0x03:
            wheel_direction[0] =  1; 
            wheel_direction[1] = -1;
            wheel_direction[2] = -1; 
            wheel_direction[3] =  1;
            break;
        case 0x04:
            wheel_direction[0] = -1; 
            wheel_direction[1] =  1;
            wheel_direction[2] =  1; 
            wheel_direction[3] = -1;
            break;
        case 0x05:
            wheel_direction[0] = -1; 
            wheel_direction[1] =  1;
            wheel_direction[2] = -1; 
            wheel_direction[3] =  1;
            break;
        case 0x06:
            wheel_direction[0] =  1; 
            wheel_direction[1] = -1;
            wheel_direction[2] =  1; 
            wheel_direction[3] = -1;
            break;
    }
}



void Set_Direction(void)
{
    for (int i = 0; i < 4; i++)
    {
        if (wheel_direction[i] > 0)
        {
            HAL_GPIO_WritePin(IN1_PORT[i], IN1_PIN[i], GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN2_PORT[i], IN2_PIN[i], GPIO_PIN_RESET);
        }
        else if (wheel_direction[i] < 0)
        {
            HAL_GPIO_WritePin(IN1_PORT[i], IN1_PIN[i], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN2_PORT[i], IN2_PIN[i], GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(IN1_PORT[i], IN1_PIN[i], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN2_PORT[i], IN2_PIN[i], GPIO_PIN_RESET);
        }
    }
}


void Chassis_Control(uint8_t cmd, uint8_t gear)
{
    int16_t max_speed = (gear == 1) ? SPEED_LIMIT_GEAR1 : SPEED_LIMIT_GEAR0;

    if (HAL_GetTick() - last_msg_time > 500)
    {
        speed = 0;
    }

    if (cmd != last_cmd)
    {
        speed = 0;
        Mecanum_SetDirection(cmd);
        last_cmd = cmd;
    }

    if (speed < max_speed)
    {
        speed += 5;
        if (speed > max_speed)
            speed = max_speed;
    }

    uint16_t pwm = (uint16_t)((float)speed / 255.0 * 99);
    for (int i = 0; i < 4; i++)
    {
        __HAL_TIM_SET_COMPARE(&htim1, PWM_CH[i], pwm);
    }

    Set_Direction();
    last_msg_time = HAL_GetTick();
}