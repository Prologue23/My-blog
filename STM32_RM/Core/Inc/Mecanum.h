#ifndef __MECANUM_H__
#define __MECANUM_H__

#include "main.h"

void Mecanum_Init();
void Mecanum_SetDirection(uint8_t cmd);
void Set_Direction(void);
void Chassis_Control(uint8_t cmd, uint8_t gear);

#endif