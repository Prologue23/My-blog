#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include <stdint.h>
#include "main.h"
#include "stm32f1xx_hal.h"


void Chassis_Init();
void Rocker_uplift();
void Rocker_downlift();
void Foot_up();
void Foot_down();

#endif

