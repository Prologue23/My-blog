#include "PID.h"
#include "Can.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <string.h>

//PID
#define KP              5.0f
#define KI              0.005f
#define KD              0.5f
#define TARGET_RPM      1000
#define MAX_OUT         16384
#define MAX_INTEGRAL    6000

#define POS_KP          2.3f
#define POS_KI          0.0f
#define POS_KD          50.0f
#define MAX_SPEED       3000
#define POS_INTEGRAL_MAX 1000.0f

#define STEP_INTERVAL_MS  500
#define STEP_ANGLE        1365

//CAN
volatile MotorInfo          feedback;
static CAN_HandleTypeDef    *can;
static volatile float       integral;
static float                last_error;
static float                pos_integral;
static float                pos_last_error;
static uint32_t             start_ms;
uint32_t                    target_angle;
static uint32_t             step_timer;

//Fire

uint8_t Enable = 0;
uint8_t Fire = 0;
uint8_t step_counter = 0;


void Motor_Init(void)
{
    CAN_FilterTypeDef f = {0};

    can = &hcan1;

    f.FilterBank           = 0;
    f.FilterMode           = CAN_FILTERMODE_IDMASK;
    f.FilterScale          = CAN_FILTERSCALE_32BIT;
    f.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    f.FilterActivation     = ENABLE;
    HAL_CAN_ConfigFilter(can, &f);

    HAL_CAN_Start(can);
    HAL_CAN_ActivateNotification(can, CAN_IT_RX_FIFO0_MSG_PENDING);

    memset(&feedback, 0, sizeof(feedback));
    integral        = 0;
    last_error      = 0;
    pos_integral    = 0;
    pos_last_error  = 0;
    start_ms        = HAL_GetTick();
    target_angle    = 0;
    step_counter    = 0;
    step_timer      = HAL_GetTick();
}

void UpdateFeedback(const MotorInfo *fb)
{
    feedback = *fb;
}

static void PackCmd(uint8_t *buf, int16_t current, uint8_t id)
{
    uint8_t off = (id - 1) * 2;
    buf[off]     = (current >> 8) & 0xFF;
    buf[off + 1] = current & 0xFF;
}

static void RunSpeedPID(float target_rpm)
{
    uint8_t tx[8] = {0};
    float error, out;

    if (HAL_GetTick() - start_ms < 500) {
        C620_CAN_SendMessage(can, CAN_CMD_ID, tx, 8);
        return;
    }

    if(Enable == 0)
    {
        C620_CAN_SendMessage(can, CAN_CMD_ID, tx, 8);
        return;
    }


    error = target_rpm - feedback.speed;

    if (feedback.speed > 300 || feedback.speed < -300) {
        if (error > 200 || error < -200)
            integral += error;
        else
            integral += 0.2f * error;
    }

    if (integral > MAX_INTEGRAL)
        integral = MAX_INTEGRAL;
    else if (integral < -MAX_INTEGRAL)
        integral = -MAX_INTEGRAL;

    out = KP * error + KI * integral + KD * (error - last_error);

    if (out > MAX_OUT) {
        integral -= (out - MAX_OUT) / KI;
        out = MAX_OUT;
    } else if (out < -MAX_OUT) {
        integral -= (out + MAX_OUT) / KI;
        out = -MAX_OUT;
    }

    last_error = error;

    PackCmd(tx, (int16_t)out, MOTOR_ID);
    C620_CAN_SendMessage(can, CAN_CMD_ID, tx, 8);
}

void SpeedTask(void)
{
    RunSpeedPID(TARGET_RPM);
}

void PositionTask(void)
{
    float pos_error, speed_target, pos_out;
    int32_t diff;

    diff = (int32_t)target_angle - (int32_t)(feedback.angle & 0x1FFF);
    if (diff > 4096)
        diff -= 8192;
    else if (diff < -4096)
        diff += 8192;
    pos_error = (float)diff;

    pos_integral += pos_error;
    if (pos_integral > POS_INTEGRAL_MAX)
        pos_integral = POS_INTEGRAL_MAX;
    else if (pos_integral < -POS_INTEGRAL_MAX)
        pos_integral = -POS_INTEGRAL_MAX;

    pos_out = POS_KP * pos_error + POS_KI * pos_integral + POS_KD * (pos_error - pos_last_error);
    pos_last_error = pos_error;

    speed_target = pos_out;
    if (speed_target > MAX_SPEED)
        speed_target = MAX_SPEED;
    else if (speed_target < -MAX_SPEED)
        speed_target = -MAX_SPEED;

    RunSpeedPID(speed_target);
}

void PositionStepTask(void)
{
    if (Enable == 1)
    {
        if (Fire != 0)
        {
            if (step_counter < Fire)
            {
                if (HAL_GetTick() - step_timer >= STEP_INTERVAL_MS)
                {
                    step_timer = HAL_GetTick();
                    target_angle += STEP_ANGLE;
                    if (target_angle >= 8192.0f)
                        target_angle -= 8192.0f;
                    step_counter++;
                }
            }
            else
            {
                step_counter = 0;
                Fire = 0;
            }

        }
    }
}


