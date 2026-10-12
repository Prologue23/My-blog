#ifndef __PWM_H__
#define __PWM_H__

#include "arm.h"
#include "stdint.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "Chassis.h"

void PWM_SetDuty();
void PWM_SetAngle();
void PWM_Control();

#endif