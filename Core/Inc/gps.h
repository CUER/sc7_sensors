#ifndef GPS_H
#define GPS_H

#include <stdint.h>

#include "stm32l4xx_hal.h"

HAL_StatusTypeDef GPS_Connect(UART_HandleTypeDef *huart_handle);

HAL_StatusTypeDef GPS_SendRxData(UART_HandleTypeDef *output_huart_handle);

HAL_StatusTypeDef GPS_UARTRxCpltHandler();

#endif /* GPS_H */