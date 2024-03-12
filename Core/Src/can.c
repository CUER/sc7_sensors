#include "can.h"

#include <assert.h>
#include <string.h>

#include "main.h"

static CAN_HandleTypeDef* hcan;
static uint32_t TxMailbox; // CAN Transmission Mailbox

void CAN_Start(CAN_HandleTypeDef *can_handle) {
    hcan = can_handle;
    CAN_FilterTypeDef  sFilterConfig;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    if(HAL_CAN_Start(hcan) != HAL_OK) {
        Error_Handler();
    }

    // Enables interrupts when receiving CAN messages
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }
}

HAL_StatusTypeDef CAN_SendGPSPos(float* lattitude, float* longitude) {
    const CAN_TxHeaderTypeDef TxHeader = {
    .StdId = CAN_ID_GPS_POS,    // ID of transmitter
    .IDE = CAN_ID_STD,          // Using standard ID
    .RTR = CAN_RTR_DATA,        // Sending dataframe
    .DLC = 8,                   // Length of data being sent in bytes
    };
    uint8_t TxData[8];
    static_assert(2 * sizeof(float) == 8);  // Check GPS Pos data holds two floats


    memcpy(TxData, lattitude, sizeof(float));
    memcpy(TxData + sizeof(float), longitude, sizeof(float));

    return HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox);
}