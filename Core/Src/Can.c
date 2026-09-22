#include "Can.h"
#include "PID.h"
#include "stm32f4xx_hal_can.h"
#include <stdint.h>

void CAN_Config(void) {
    // Configure CAN peripheral
}



HAL_StatusTypeDef Init_Filter(CAN_HandleTypeDef *hcan, uint32_t filter_id, uint32_t filter_mask, uint32_t filter_bank, uint32_t filter_mode, uint32_t filter_scale) 
{
    CAN_FilterTypeDef CAN_FilterInitStructure;
    CAN_FilterInitStructure.FilterBank = filter_bank;
    CAN_FilterInitStructure.FilterScale = filter_scale;
    CAN_FilterInitStructure.FilterMode = filter_mode;
    CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    CAN_FilterInitStructure.FilterIdHigh = (filter_id << 5) >> 16; 
    CAN_FilterInitStructure.FilterIdLow = (filter_id << 5) & 0xFFFF; 
    CAN_FilterInitStructure.FilterMaskIdHigh = (filter_mask << 5) >> 16; 
    CAN_FilterInitStructure.FilterMaskIdLow = (filter_mask << 5) & 0xFFFF; 
    CAN_FilterInitStructure.FilterActivation = ENABLE;

    if (HAL_CAN_ConfigFilter(hcan, &CAN_FilterInitStructure) != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}



HAL_StatusTypeDef C620_CAN_SendMessage(CAN_HandleTypeDef *hcan, uint32_t ID, uint8_t *DATA, uint8_t LEN)
{
    CAN_TxHeaderTypeDef TxMessage;
    TxMessage.StdId = ID;
    //TxMessage.ExtId = ID;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = LEN;
    uint32_t TxMailbox;
    return HAL_CAN_AddTxMessage(hcan, &TxMessage, DATA, &TxMailbox);
}

uint8_t C620_CAN_ReceiveMessage(CAN_HandleTypeDef *hcan, uint32_t *ID, uint8_t *DATA, uint8_t *LEN)
{
    CAN_RxHeaderTypeDef RxMessage;
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxMessage, DATA) == HAL_OK)
    {
        *ID = RxMessage.StdId;
        //*ID = RxMessage.ExtId;
        *LEN = RxMessage.DLC;
        return 1; // Message received
    }
    return 0; // No message received
}


void C620_CAN_Receive(uint32_t *ID, uint8_t *DATA, uint8_t *LEN)
{
    CAN_RxHeaderTypeDef RxMessage;
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxMessage, DATA);
    if (RxMessage.IDE == CAN_ID_STD) 
    {
        *ID = RxMessage.StdId;
    }
    else 
    {
        *ID = RxMessage.ExtId;
    }
    if (RxMessage.RTR == CAN_RTR_DATA) 
    {
        *LEN = RxMessage.DLC;
    }
    else 
    {
        // Remote frame
    }
}



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header = {0};
    uint8_t rx_data[8] = {0};

    if (hcan->Instance == CAN1)
    {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data) != HAL_OK)
            return;

        if (rx_header.StdId == CAN_FEEDBACK_ID(MOTOR_ID))
        {
            MotorInfo fb;
            fb.angle   = (uint16_t)((rx_data[0] << 8) | rx_data[1]);
            fb.speed   = (int16_t)((rx_data[2] << 8) | rx_data[3]);
            fb.current = (int16_t)((rx_data[4] << 8) | rx_data[5]);
            fb.temp    = rx_data[6];

            UpdateFeedback(&fb);
        }

        HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
    }
}