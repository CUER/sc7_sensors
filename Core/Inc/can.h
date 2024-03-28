/**
 * @file
 * @brief Provides function for transmitting CAN messages
*/
#ifndef CAN_H
#define CAN_H

#include <stdint.h>

#include "stm32l4xx_hal.h"

#include "can_defs.h"

void CAN_Start(CAN_HandleTypeDef *can_handle);

HAL_StatusTypeDef CAN_SendGPSTime(GPS_time_t* time);

HAL_StatusTypeDef CAN_SendGPSPos(float* lattitude, float* longitude);

HAL_StatusTypeDef CAN_SendGPSAltVel(float* altitude, float* speed);

#endif /* CAN_H */