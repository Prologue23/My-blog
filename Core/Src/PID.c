/**
  ******************************************************************************
  * @file    PID.c
  * @brief   RoboMaster C620 电调 PID 控制模块实现
  *
  * 依赖：
  *   - stm32f4xx_hal_can.h   (CAN 外设)
  *   - stm32f4xx_hal_cortex.h(SysTick / HAL_GetTick)
  *
  ******************************************************************************
  */

#include "PID.h"
#include <string.h>

/* ============================================================
 *  全局句柄（绑定外部 hcan1）
 * ============================================================ */

CAN_HandleTypeDef *C620_hcan = NULL;

/* ============================================================
 *  私有变量
 * ============================================================ */

static uint32_t s_ctrl_start_ms   = 0;
static int16_t  s_last_velocity   = 0;
static PID_State_t s_vel_pid_state;

#if C620_POS_LOOP_ENABLE
static PID_State_t s_pos_pid_state;
static float       s_last_vel_cmd = 0.0f;
#endif

/* ============================================================
 *  全局变量（PID.h 中 extern）
 * ============================================================ */

C620_Feedback_t C620_Feedback;
float           C620_VelocityTarget = 0.0f;
int16_t         C620_DebugCommand   = 0;
uint8_t         C620_CtrlReady      = 0;

#if C620_POS_LOOP_ENABLE
int32_t         C620_PosTarget      = 0;
#endif

/* ============================================================
 *  私有工具函数
 * ============================================================ */

/**
 * @brief 角度误差归一化到 [-res/2, +res/2]
 *         C620 编码器 14 位，res=16384 → 归一化到 [-8192, +8192]
 */
static int32_t NormalizeAngleError(int32_t error, uint32_t resolution)
{
    int32_t half = (int32_t)(resolution / 2u);
    if (error >  half) error -= (int32_t)resolution;
    if (error < -half) error += (int32_t)resolution;
    return error;
}

/**
 * @brief 通用位置式 PID（积分死区衰减、输出限幅）
 */
static float PID_Calculate(const PID_Params_t *params,
                           PID_State_t        *state,
                           int32_t             target,
                           int32_t             actual)
{
    int32_t error      = target - actual;
    int32_t error_diff = error - state->last_error;
    float   output     = 0.0f;

    if (error < -params->deadband || error > params->deadband)
    {
        state->integral += (float)error;
        if (state->integral >  params->i_limit)  state->integral =  params->i_limit;
        if (state->integral < -params->i_limit)  state->integral = -params->i_limit;
    }
    else
    {
        state->integral *= 0.94f;
    }

    output = params->kp * (float)error +
             params->ki * state->integral +
             params->kd * (float)error_diff;

    if (output >  params->output_max)
    {
        state->integral -= ((float)output - (float)params -> output_max)/params->ki;
    }
    else if (output < params -> output_min)
    {
        state->integral -= ((float)output - (float)params -> output_min)/params->ki;
    }  //防止积分过饱和

    if (output > (float) params->output_max) output = (float) params->output_max;
    if (output < (float) params->output_min) output = (float) params->output_min;

    state->last_error      = error;
    state->last_derivative = output;
    state->last_output     = output;

    return output;
}

/**
 * @brief 把命令打包进 C620 控制帧 0x200 的指定电机字段
 */
static void PackCommand(uint8_t *tx_buf, int16_t command, uint8_t motor_id)
{
    uint8_t offset = (uint8_t)C620_CMD_OFFSET(motor_id);
    tx_buf[offset]     = (uint8_t)((uint16_t)command >> 8);
    tx_buf[offset + 1] = (uint8_t)((uint16_t)command);
}

/**
 * @brief 向 CAN 发送帧（非阻塞，直接调 HAL_CAN_AddTxMessage）
 */
static void CAN_SendControlFrame(uint32_t std_id, uint8_t *data)
{
    CAN_TxHeaderTypeDef tx_header = {0};
    uint32_t tx_mailbox = 0;

    if (C620_hcan == NULL) return;

    HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_10);

    tx_header.StdId = std_id;
    tx_header.IDE   = CAN_ID_STD;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.DLC   = 8;

    if (HAL_CAN_AddTxMessage(C620_hcan, &tx_header, data, &tx_mailbox) != HAL_OK)
    {
        HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_11);
    }
}

/* ============================================================
 *  位置环专属（仅 C620_POS_LOOP_ENABLE = 1）
 * ============================================================ */

#if C620_POS_LOOP_ENABLE

/**
 * @brief 连续角度追踪（跨 0↔16383 跳变处理）
 */
static int32_t ContinuousPos(void)
{
    static int32_t angle_cont = 0;
    static uint16_t last_angle = 0;
    static uint8_t  angle_init = 0;
    int32_t raw   = (int32_t)C620_Feedback.angle_raw;
    int32_t delta;

    if (!angle_init)
    {
        angle_cont = raw;
        angle_init = 1;
    }
    else
    {
        delta = raw - (int32_t)last_angle;
        delta = NormalizeAngleError(delta, C620_ENCODER_RESOLUTION);
        angle_cont += delta;
    }
    last_angle = (uint16_t)raw;
    return angle_cont;
}

static float PositionLoop_Calculate(void)
{
    static const PID_Params_t pos_params = {
        .kp         = C620_POS_KP,
        .ki         = C620_POS_KI,
        .kd         = C620_POS_KD,
        .i_limit    = C620_POS_I_LIMIT,
        .output_max =  C620_POS_OUTPUT_MAX,
        .output_min = -C620_POS_OUTPUT_MAX,
        .deadband   = C620_POS_DEADBAND,
    };

    int32_t actual = ContinuousPos() % (int32_t)C620_ENCODER_RESOLUTION;
    int32_t err    = NormalizeAngleError(C620_PosTarget - actual,
                                         C620_ENCODER_RESOLUTION);
    float   out    = PID_Calculate(&pos_params, &s_pos_pid_state,
                                   C620_PosTarget, actual);

    if (out >  s_last_vel_cmd + C620_POS_RAMP_RPM_PER_MS)
        out =  s_last_vel_cmd + C620_POS_RAMP_RPM_PER_MS;
    if (out <  s_last_vel_cmd - C620_POS_RAMP_RPM_PER_MS)
        out =  s_last_vel_cmd - C620_POS_RAMP_RPM_PER_MS;
    s_last_vel_cmd = out;

    return out;
}

#endif /* C620_POS_LOOP_ENABLE */

/* ============================================================
 *  公开接口实现
 * ============================================================ */

/**
 * @brief 绑定 CAN 句柄。在 MX_CAN1_Init() 之后调用。
 */
void C620_BindCAN(CAN_HandleTypeDef *hcan)
{
    C620_hcan = hcan;
}

/**
 * @brief 配置电机 ID（通过 0x700 配置帧）
 *
 *        C620 电机 ID 配置帧格式：
 *          CAN ID 0x701 → 配置电机 1~4
 *          CAN ID 0x702 → 配置电机 5~8
 *          数据 Byte0: 目标电机 ID (1~4 发送到 0x701；5~8 发送到 0x702)
 *          Byte6-7:     校验 CRC-16
 *        （简化版：仅支持出厂默认 ID=1，若修改需自行计算 CRC）
 */
void C620_ConfigMotorID(uint8_t motor_id)
{
    uint8_t tx_buf[8] = {0};
    uint32_t config_id = (motor_id <= 4u) ? 0x701u : 0x702u;

    if (C620_hcan == NULL) return;

    motor_id -= 1u;
    tx_buf[0] = motor_id;
    tx_buf[1] = 0x00;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0x00;
    tx_buf[4] = 0x00;
    tx_buf[5] = 0x00;
    tx_buf[6] = 0x00;
    tx_buf[7] = 0x00;

    CAN_SendControlFrame(config_id, tx_buf);
}

/**
 * @brief 配置 CAN 过滤器，接收 C620 反馈帧 0x201 ~ 0x208
 *
 *        过滤器配置策略：
 *          - 过滤器 Bank 0，Mask 模式，32-bit 配置
 *          - Filter ID  = 0x00002000  (std_id << 5 = 0x200 << 5)
 *          - Filter Mask= 0xFFFFFE00  (bit 0~7 忽略，bit 11~31 全匹配)
 *          - 这样会接收 0x200 ~ 0x2FF 所有帧
 *          - 也可以精确匹配 0x201~0x208：Mask=0xFFFFFE00
 *            配合 Filter ID=0x00002020 或直接设 Filter ID=0x00002000 都可以
 */
HAL_StatusTypeDef C620_ConfigFilter(void)
{
    CAN_FilterTypeDef sFilterConfig = {0};

    if (C620_hcan == NULL) return HAL_ERROR;

    sFilterConfig.FilterBank           = 0;
    sFilterConfig.FilterMode           = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale          = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh         = 0x0000;
    sFilterConfig.FilterIdLow          = 0x0000;
    sFilterConfig.FilterMaskIdHigh     = 0x0000;
    sFilterConfig.FilterMaskIdLow      = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    sFilterConfig.FilterActivation     = ENABLE;

    return HAL_CAN_ConfigFilter(C620_hcan, &sFilterConfig);
}

/**
 * @brief 模块初始化：清零所有状态
 */
void C620_Init(void)
{
    memset(&C620_Feedback, 0, sizeof(C620_Feedback));
    memset(&s_vel_pid_state, 0, sizeof(s_vel_pid_state));
#if C620_POS_LOOP_ENABLE
    memset(&s_pos_pid_state, 0, sizeof(s_pos_pid_state));
    s_last_vel_cmd = 0.0f;
#endif
    s_ctrl_start_ms      = 0;
    s_last_velocity      = 0;
    C620_VelocityTarget  = 0.0f;
    C620_DebugCommand    = 0;
    C620_CtrlReady       = 0;
#if C620_POS_LOOP_ENABLE
    C620_PosTarget       = 0;
#endif
}

/**
 * @brief 启动控制输出
 */
void C620_Control_Start(void)
{
    s_ctrl_start_ms = HAL_GetTick();
    memset(&s_vel_pid_state, 0, sizeof(s_vel_pid_state));
#if C620_POS_LOOP_ENABLE
    memset(&s_pos_pid_state, 0, sizeof(s_pos_pid_state));
#endif
    C620_CtrlReady = 1;
}

/**
 * @brief 设置速度环目标
 */
void C620_SetVelocityTarget(float target_rpm)
{
    C620_VelocityTarget = target_rpm;
}

#if C620_POS_LOOP_ENABLE
/**
 * @brief 设置位置环目标（归一化到 0~16383）
 */
void C620_SetPositionTarget(int32_t target_raw)
{
    target_raw %= (int32_t)C620_ENCODER_RESOLUTION;
    if (target_raw < 0) target_raw += (int32_t)C620_ENCODER_RESOLUTION;
    C620_PosTarget = target_raw;
}
#endif

/**
 * @brief 写入反馈数据（CAN 接收回调中调用）
 */
void C620_UpdateFeedback(uint16_t angle_raw, int16_t speed_rpm,
                         int16_t current_raw, uint8_t temperature)
{
    C620_Feedback.angle_raw   = angle_raw;
    C620_Feedback.speed_rpm   = speed_rpm;
    C620_Feedback.current_raw = current_raw;
    C620_Feedback.temperature = temperature;

    if (!C620_Feedback.initialized)
    {
        C620_Feedback.initialized    = 1;
        C620_Feedback.speed_filtered = speed_rpm;
        return;
    }

    C620_Feedback.speed_filtered =
        (int16_t)(0.7f * (float)C620_Feedback.speed_filtered +
                  0.3f * (float)speed_rpm);
}

/**
 * @brief 1kHz 周期控制任务（放在 1ms 定时器中断里）
 *
 *        速度环 PID 参数：
 *          Kp  = C620_VEL_KP
 *          Ki  = C620_VEL_KI（大误差全增益，小误差 0.2× 减小超调）
 *          Kd  = C620_VEL_KD（用速度差分近似加速度）
 *          输出限幅 = ±(C620_CMD_RAW_MAX / 3)
 *
 *        C620 电流模式命令范围 ±16384，电压模式 ±6000。
 *        建议 PID 输出限幅先设为满功率的 1/3，调稳后逐步放大。
 */
void C620_CAN_1kHz_Task(void)
{
    uint8_t tx_buf[8] = {0};
    int16_t command   = 0;
    float   velocity_target;

    static const PID_Params_t vel_params = {
        .kp         = C620_VEL_KP,
        .ki         = C620_VEL_KI,
        .kd         = C620_VEL_KD,
        .i_limit    = C620_VEL_I_LIMIT,
        .output_max =  C620_VEL_OUTPUT_MAX,
        .output_min = -C620_VEL_OUTPUT_MAX,
        .deadband   = C620_VEL_DEADBAND,
    };

    if (!C620_CtrlReady || C620_hcan == NULL) return;

    /* ---- 启动前 500ms：发零帧使电调自检 ---- */
    if (HAL_GetTick() - s_ctrl_start_ms < C620_ENABLE_ZERO_TIME_MS)
    {
        CAN_SendControlFrame(C620_CTRL_FRAME_ID, tx_buf);
        return;
    }

    /* ---- 反馈未就绪 ---- */
    if (!C620_Feedback.initialized) return;

    /* ---- 确定速度目标 ---- */
#if C620_INNER_LOOP_TEST_ENABLE
    velocity_target = (float)C620_INNER_TEST_TARGET_RPM;
#elif C620_POS_LOOP_ENABLE
    velocity_target = PositionLoop_Calculate();
#else
    velocity_target = C620_VelocityTarget;
#endif

    /* ---- 速度环 PID ---- */
    {
        int32_t velocity_error =
            (int32_t)velocity_target - (int32_t)C620_Feedback.speed_filtered;
        float vel_output;

        /* 大误差积分增益 ×1，小误差 ×0.2 减小超调 */
        if (velocity_error < -300 || velocity_error > 300)
            s_vel_pid_state.integral += (float)velocity_error;
        else
            s_vel_pid_state.integral += (float)velocity_error * 0.2f;

        if (s_vel_pid_state.integral >  vel_params.i_limit)
            s_vel_pid_state.integral  =  vel_params.i_limit;
        if (s_vel_pid_state.integral < -vel_params.i_limit)
            s_vel_pid_state.integral  = -vel_params.i_limit;

        /* Kd 项：用速度差分近似加速度 */
        vel_output = vel_params.kp * (float)velocity_error -
                     vel_params.kd * (float)((int32_t)velocity_target -
                                             (int32_t)s_last_velocity) +
                     vel_params.ki * s_vel_pid_state.integral;

        if (vel_output >  (float) vel_params.output_max)
            vel_output =  (float) vel_params.output_max;
        if (vel_output <  (float) vel_params.output_min)
            vel_output =  (float) vel_params.output_min;

        command = (int16_t)vel_output;
        C620_DebugCommand = command;
        s_last_velocity   = C620_Feedback.speed_rpm;
    }

    /* ---- 打包并发送 CAN 控制帧 0x200 ---- */
    PackCommand(tx_buf, command, C620_MOTOR_ID_1);
    CAN_SendControlFrame(C620_CTRL_FRAME_ID, tx_buf);
}