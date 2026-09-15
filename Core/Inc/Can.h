#ifndef CAN_H
#define CAN_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

/* ---- 外部全局句柄（定义在 main.c 中）---- */
extern CAN_HandleTypeDef hcan1;

/* ---- 旧版 CAN 接口（保留兼容，PID 模块已直接使用 HAL API）---- */
void CAN_Config(void);

HAL_StatusTypeDef Init_Filter(CAN_HandleTypeDef *hcan, uint32_t filter_id,
                              uint32_t filter_mask, uint32_t filter_bank,
                              uint32_t filter_mode, uint32_t filter_scale);

HAL_StatusTypeDef C620_CAN_SendMessage(CAN_HandleTypeDef *hcan, uint32_t ID,
                                       uint8_t *DATA, uint8_t LEN);

uint8_t C620_CAN_ReceiveMessage(CAN_HandleTypeDef *hcan, uint32_t *ID,
                                uint8_t *DATA, uint8_t *LEN);

/**
 * @brief 阻塞式接收（已被 PID 模块的中断回调替代，保留仅供调试）
 */
void C620_CAN_Receive(uint32_t *ID, uint8_t *DATA, uint8_t *LEN);

#endif /* CAN_H */