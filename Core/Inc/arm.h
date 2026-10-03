#ifndef __ARM_H__
#define __ARM_H__

#include <stdint.h>
#include "main.h"
#include "stm32f1xx_hal.h"

void arm_Init(void);
void arm_Move_up(void);
void arm_Move_down(void);
void arm_Move_left(void);
void arm_Move_right(void);
void arm_Move_forward(void);
void arm_Move_back(void);
void arm_Claw_change(void);
void arm_Move_to(int rho, int height);

#endif /* __ARM_H__ */