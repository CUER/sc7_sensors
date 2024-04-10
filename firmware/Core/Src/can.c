#include "can.h"

#include <assert.h>
#include <string.h>

#include "utils.h"

static CAN_HandleTypeDef* hcan;
static uint32_t tx_mailbox; // CAN Transmission Mailbox

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

    CHECK_HAL_RETURN(HAL_CAN_ConfigFilter(hcan, &sFilterConfig));
    CHECK_HAL_RETURN(HAL_CAN_Start(hcan));

    // Enables interrupts when receiving CAN messages
    CHECK_HAL_RETURN(HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING));
}

HAL_StatusTypeDef CAN_SendGPSTime(time_t* time) {
    const CAN_TxHeaderTypeDef can_header = {
        .StdId = CAN_ID_GPS_TIME,                           // ID of transmitter
        .IDE = CAN_ID_STD,                                  // Using standard ID
        .RTR = CAN_RTR_DATA,                                // Sending dataframe
        .DLC = sizeof(NULL_TYPE(CAN_GPS_Time_t).data),      // Length of data being sent in bytes
    };
    CAN_GPS_Time_t can_msg;
    can_msg.data = *time;

    return HAL_CAN_AddTxMessage(hcan, &can_header, can_msg.bytes, &tx_mailbox);
}

HAL_StatusTypeDef CAN_SendGPSPos(float* lattitude, float* longitude) {
    const CAN_TxHeaderTypeDef can_header = {
        .StdId = CAN_ID_GPS_COORD,                          // ID of transmitter
        .IDE = CAN_ID_STD,                                  // Using standard ID
        .RTR = CAN_RTR_DATA,                                // Sending dataframe
        .DLC = sizeof(NULL_TYPE(CAN_GPS_Coord_t).data),     // Length of data being sent in bytes
    };
    CAN_GPS_Coord_t can_msg;
    can_msg.data.lattitude = *lattitude;
    can_msg.data.longitude = *longitude;

    return HAL_CAN_AddTxMessage(hcan, &can_header, can_msg.bytes, &tx_mailbox);
}

HAL_StatusTypeDef CAN_SendGPSAltVel(float* altitude, float* speed) {
    const CAN_TxHeaderTypeDef can_header = {
        .StdId = CAN_ID_GPS_ALTVEL,                         // ID of transmitter
        .IDE = CAN_ID_STD,                                  // Using standard ID
        .RTR = CAN_RTR_DATA,                                // Sending dataframe
        .DLC = sizeof(NULL_TYPE(CAN_GPS_AltVel_t).data),    // Length of data being sent in bytes
    };
    CAN_GPS_AltVel_t can_msg;
    can_msg.data.altitude = *altitude;
    can_msg.data.speed = *speed;

    return HAL_CAN_AddTxMessage(hcan, &can_header, can_msg.bytes, &tx_mailbox);
}