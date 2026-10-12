#ifndef PWM_H
#define PWM_H

#include "stdint.h"

void PWM_Init(uint32_t frequency);
void PWM_SetDutyCycle(uint8_t channel, uint8_t dutyCycle);
void PWM_Start(uint8_t channel);
void PWM_Stop(uint8_t channel);

#endif // PWM_H