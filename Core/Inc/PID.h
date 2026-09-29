#ifndef __PID_H__
#define __PID_H__

#include "stdint.h"

#define MOTOR_ID        1
#define CAN_CMD_ID      0x200
#define CAN_FEEDBACK_ID(motor_id) (0x200 + (motor_id))

typedef struct {
    uint16_t angle;
    int16_t  speed;
    int16_t  current;
    uint8_t  temp;
} MotorInfo;

void Motor_Init(void);
void SpeedTask(void);
void PositionTask(void);
void PositionStepTask(void);
void UpdateFeedback(const MotorInfo *fb);
void UpdateRoundCounter(void);

#endif
