#ifndef CAN_H
#define CAN_H

#include "stm32f4xx_hal.h"
#include <stdint.h>


extern CAN_HandleTypeDef hcan1;



//标准库CAN接口
void CAN_Config(void);

HAL_StatusTypeDef Init_Filter(CAN_HandleTypeDef *hcan, uint32_t filter_id,
                              uint32_t filter_mask, uint32_t filter_bank,
                              uint32_t filter_mode, uint32_t filter_scale);

HAL_StatusTypeDef C620_CAN_SendMessage(CAN_HandleTypeDef *hcan, uint32_t ID,
                                       uint8_t *DATA, uint8_t LEN);

uint8_t C620_CAN_ReceiveMessage(CAN_HandleTypeDef *hcan, uint32_t *ID,
                                uint8_t *DATA, uint8_t *LEN);

//阻塞接收
void C620_CAN_Receive(uint32_t *ID, uint8_t *DATA, uint8_t *LEN);


//反馈帧解析
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);  //取报文



#endif /* CAN_H */