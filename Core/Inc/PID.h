/**
  ******************************************************************************
  * @file    PID.h
  * @brief   RoboMaster C620 电调 · 速度单环 / 可选位置环 PID 控制模块
  *
  * 适用硬件：STM32F427 / STM32F407 RoboMaster 开发板 + 大疆 C620 电调
  * 通信方式：CAN1 · 1Mbps
  * 编码器：14 位（0 ~ 16383）
  * 控制模式：电流模式（默认）±16384，电压模式 ±6000
  *
  * 宏开关说明：
  *   C620_POS_LOOP_ENABLE       (0/1)  位置环总开关，默认 0 = 纯速度单环
  *   C620_INNER_LOOP_TEST_ENABLE (0/1) 内环固定目标测试模式
  *   C620_CTRL_MODE              (0/1) 0=电压控制 1=电流控制（默认）
  *
  * 使用流程（参考 main.c）：
  *   1. MX_CAN1_Init() → 配置 CAN
  *   2. C620_ConfigMotorID(1)  → 给电机 1 分配 CAN ID 0x201
  *   3. C620_ConfigFilter()    → 配 CAN 过滤器接收 0x201~0x208
  *   4. HAL_CAN_Start(&hcan1)
  *   5. HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING)
  *   6. C620_Init()            → 模块初始化
  *   7. C620_Control_Start()   → 启动控制
  *   8. TIM_CH_Start(1kHz)     → 启动 1kHz 定时器，回调里调 C620_CAN_1kHz_Task()
  *   9. HAL_CAN_RxFifo0MsgPendingCallback() → 解析反馈帧 → C620_UpdateFeedback()
  *  10. 主循环/上层逻辑 → C620_SetVelocityTarget(RPM)
  *
  ******************************************************************************
  */

#ifndef PID_H
#define PID_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
 *  宏开关 & 硬件常量（大疆 C620 真实协议值）
 * ============================================================ */

#ifndef C620_POS_LOOP_ENABLE
#define C620_POS_LOOP_ENABLE        0
#endif

#ifndef C620_INNER_LOOP_TEST_ENABLE
#define C620_INNER_LOOP_TEST_ENABLE 0
#endif

/* ---------- 电调硬件常量 ---------- */
#define C620_MOTOR_COUNT            8u          /* 电调支持 8 个电机              */
#define C620_MOTOR_ID_1             1u          /* 当前模块控制的电机编号（1~8）  */
#define C620_ENCODER_RESOLUTION     16384u      /* C620 编码器：14 位，0~16383   */
#define C620_ENABLE_ZERO_TIME_MS    500u        /* 启动后发零帧时间，使电调自检   */

/* ---------- CAN 帧 ID ---------- */
#define C620_CTRL_FRAME_ID          0x200u      /* 控制帧（发送）：统一 0x200     */
#define C620_FEEDBACK_FRAME_ID_BASE 0x200u      /* 反馈帧基地址 0x200，M1=0x201  */
#define C620_CONFIG_FRAME_ID        0x700u      /* 电机 ID 配置帧                */

/* ---------- 控制模式 ---------- */
#define C620_CTRL_MODE_VOLTAGE      0   /* 电压模式 CAN ID 0x1FF/0x2FF，±6000 */
#define C620_CTRL_MODE_CURRENT      1   /* 电流模式 CAN ID 0x200，±16384     */
#ifndef C620_CTRL_MODE
#define C620_CTRL_MODE              C620_CTRL_MODE_CURRENT
#endif

#if C620_CTRL_MODE == C620_CTRL_MODE_VOLTAGE
    #define C620_CMD_RAW_MAX        6000        /* 电压模式满输出               */
#else
    #define C620_CMD_RAW_MAX        16384       /* 电流模式满输出（14 位）       */
#endif

/* ---------- 电机命令在控制帧中的字节偏移 ---------- */
#define C620_CMD_OFFSET(motor_id)   ((((motor_id) - 1u) % 4u) * 2u)

/* ---------- 反馈帧 ID 计算宏 ---------- */
#define C620_FEEDBACK_FRAME_ID(motor_id) \
    (C620_FEEDBACK_FRAME_ID_BASE + (motor_id))

/* ---------- 内环测试参数 ---------- */
#if C620_INNER_LOOP_TEST_ENABLE
#define C620_INNER_TEST_TARGET_RPM  300
#endif

/* ============================================================
 *  PID 参数宏（给合理起始值，调参时修改）
 * ============================================================ */

/* 速度环 */
#define C620_VEL_KP                 1.2f
#define C620_VEL_KI                 0.5f
#define C620_VEL_KD                 0.0f
#define C620_VEL_I_LIMIT            500.0f
#define C620_VEL_OUTPUT_MAX         (C620_CMD_RAW_MAX / 3)
#define C620_VEL_DEADBAND           10

#define constant_target_rpm         3000.0f  // 固定目标转速（RPM）

/* 位置环（仅 C620_POS_LOOP_ENABLE = 1） */
#if C620_POS_LOOP_ENABLE
#define C620_POS_KP                 8.0f
#define C620_POS_KI                 0.0f
#define C620_POS_KD                 0.1f
#define C620_POS_I_LIMIT            2000.0f
#define C620_POS_OUTPUT_MAX         600
#define C620_POS_DEADBAND           16
#define C620_POS_RAMP_RPM_PER_MS    5.0f
#endif

/* ============================================================
 *  类型定义
 * ============================================================ */

/**
 * @brief C620 电机反馈数据（对应 CAN 反馈帧 0x201~0x208）
 */
typedef struct
{
    uint16_t angle_raw;      /* 编码器原始角度 0~16383              */
    int16_t  speed_rpm;      /* 电调直接反馈的转速（RPM）           */
    int16_t  current_raw;    /* 电流原始采样值                       */
    uint8_t  temperature;    /* 电机温度（℃）                       */
    int16_t  speed_filtered; /* 一阶低通滤波后的转速                 */
    uint8_t  initialized;    /* 反馈是否已完成首次初始化             */
} C620_Feedback_t;

/**
 * @brief PID 参数结构体
 */
typedef struct
{
    float    kp;
    float    ki;
    float    kd;
    float    i_limit;
    int16_t  output_max;
    int16_t  output_min;
    int16_t  deadband;
} PID_Params_t;

/**
 * @brief PID 运行时状态
 */
typedef struct
{
    float    integral;
    float    last_derivative;
    int32_t  last_error;
    float    last_output;
} PID_State_t;

/* ============================================================
 *  对外可见全局变量（谨慎访问）
 * ============================================================ */

extern CAN_HandleTypeDef *C620_hcan;        /* 指向全局 hcan1 指针 */
extern C620_Feedback_t    C620_Feedback;
extern float              C620_VelocityTarget;
extern int16_t            C620_DebugCommand;
extern uint8_t            C620_CtrlReady;

#if C620_POS_LOOP_ENABLE
extern int32_t            C620_PosTarget;
#endif

/* ============================================================
 *  公开接口
 * ============================================================ */

/**
 * @brief  绑定 CAN 句柄。调用本函数后模块才能发 CAN 帧。
 *         建议在 MX_CAN1_Init() 之后立即调用。
 * @param  hcan : HAL CAN 句柄指针（通常是 &hcan1）
 * @retval None
 */
void C620_BindCAN(CAN_HandleTypeDef *hcan);

/**
 * @brief  配置电机 ID（通过 CAN 配置帧 0x700）
 *         C620 电调出厂默认 ID 为 1，如需修改调用此函数。
 *         注意：电机 1~4 发送到 CAN ID 0x701，电机 5~8 发送到 0x702。
 * @param  motor_id : 目标电机 ID（1~8）
 * @retval None
 */
void C620_ConfigMotorID(uint8_t motor_id);

/**
 * @brief  配置 CAN 过滤器，接收 C620 反馈帧 0x201~0x208
 * @retval HAL_OK / HAL_ERROR
 */
HAL_StatusTypeDef C620_ConfigFilter(void);

/**
 * @brief  模块初始化：清零状态、初始化 PID 参数
 *         必须在 C620_BindCAN() 之后调用
 * @retval None
 */
void C620_Init(void);

/**
 * @brief  启动控制输出：记录启动时刻、清零积分
 *         启动后前 C620_ENABLE_ZERO_TIME_MS ms 发零帧使电调自检
 * @retval None
 */
void C620_Control_Start(void);

/**
 * @brief  设置速度环目标
 * @param  target_rpm : 目标转速（RPM）
 * @retval None
 */
void C620_SetVelocityTarget(float target_rpm);

#if C620_POS_LOOP_ENABLE
/**
 * @brief  设置位置环目标（仅启用位置环时可用）
 * @param  target_raw : 目标编码器原始角度（0~16383）
 * @retval None
 */
void C620_SetPositionTarget(int32_t target_raw);
#endif

/**
 * @brief  CAN 接收中断解析反馈帧后调用此函数写入反馈数据
 *         对应 CAN 帧 ID 0x201~0x208，8 字节数据格式：
 *           Byte0-1: Angle[13:0]  (uint16 大端)
 *           Byte2-3: Speed[15:0] (int16 大端，RPM)
 *           Byte4-5: Current[15:0](int16 大端)
 *           Byte6  : Temperature  (uint8, ℃)
 *           Byte7  : Reserved
 *
 *         解析示例（放在 HAL_CAN_RxFifo0MsgPendingCallback 中）：
 *           if (rx_header.StdId == C620_FEEDBACK_FRAME_ID(C620_MOTOR_ID_1)) {
 *               uint16_t angle = (uint16_t)(data[0] << 8 | data[1]);
 *               int16_t  speed = (int16_t)(data[2] << 8 | data[3]);
 *               int16_t  curr  = (int16_t)(data[4] << 8 | data[5]);
 *               uint8_t  temp  = data[6];
 *               C620_UpdateFeedback(angle, speed, curr, temp);
 *           }
 *
 * @param  angle_raw   : 编码器原始角度
 * @param  speed_rpm   : 转速（RPM）
 * @param  current_raw : 电流
 * @param  temperature : 温度（℃）
 * @retval None
 */
void C620_UpdateFeedback(uint16_t angle_raw, int16_t speed_rpm,
                         int16_t current_raw, uint8_t temperature);

/**
 * @brief  1kHz 周期任务。必须在 1ms 定时器中断或调度器中调用。
 *         内部顺序：
 *           启动零帧 → 检查反馈 → 位置环（可选）→ 速度环 → CAN 发帧
 * @retval None
 */
void C620_CAN_1kHz_Task(void);

#ifdef __cplusplus
}
#endif

#endif /* PID_H */