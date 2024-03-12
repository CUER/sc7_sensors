#ifndef CAN_H
#define CAN_H

#include <stdint.h>

#include "stm32l4xx_hal.h"

#define CAN_ID_GPS_POS 0x550

void CAN_Start(CAN_HandleTypeDef *can_handle);

HAL_StatusTypeDef CAN_SendGPSPos(float* lattitude, float* longitude);

#endif /* CAN_H */