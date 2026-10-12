#include "PWM.h"


extern uint8_t command;
extern uint8_t task;
extern float angle[7];
extern float degree[7];
extern float duty[7];
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;


void PWM_SetDuty()
{
  for (int i = 0; i < 7; i++)
    {
      duty[i] = ( 10 * (float)angle[i] / 180.0 + 2.5 ) / 100.0 * 2000;
      if (i == 0)
      {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty[i]);
      }
      else if (i == 1)
      {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, duty[i]);
      }
      else if (i == 2)
      {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, duty[i]);
      }
      else if (i == 3)
      {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, duty[i]);
      }
      else if (i == 4)
      {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty[i]);
      }
      else if (i == 5)
      {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, duty[i]);
      }
      else if (i == 6)
      {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, duty[i]);
      }
    }
}


void PWM_SetAngle()
{
  for (int i = 0; i < 7; i++)
    {
      
      if (degree[i] > 180)
      {
        degree[i] = 180;
      }
      else if (degree[i] < 0)
      {
        degree[i] = 0;
      }
      angle[i] = degree[i];
    }
}


void PWM_Control()
{
    if (command == 0x01)
    {
        arm_Move_forward();
        command = 0x00;
    }
    else if (command == 0x02)
    {
        arm_Move_back();
        command = 0x00;
    }
    else if (command == 0x03)
    {
        arm_Move_left();
        command = 0x00;
    }
    else if (command == 0x04)
    {
        arm_Move_right();
        command = 0x00;
    }
    else if (command == 0x05)
    {
        arm_Move_up();
        command = 0x00;
    }
    else if (command == 0x06)
    {
        arm_Move_down();
        command = 0x00;
    }
    else if (command == 0x07)
    {
        arm_Claw_change();
        command = 0x00;
    }
      
    if (task == 0x01)
    {
        Rocker_uplift();
        task = 0x00;
    }
    else if (task == 0x02)
    {
        Rocker_downlift();
        task = 0x00;
    }
    else if (task == 0x03)
    {
        Foot_up();
        task = 0x00;
    }
    else if (task == 0x04)
    {
        Foot_down();
        task = 0x00;
    }
}