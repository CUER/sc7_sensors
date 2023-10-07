#ifndef PROTO_UTILS_H
#define PROTO_UTILS_H

#include <stm32l4xx_hal.h>

#include <pb_encode.h>
#include <imu.pb.h>

#define UART_DELAY_TIMEOUT HAL_MAX_DELAY

void Proto_Setup(UART_HandleTypeDef* uart_handle);

HAL_StatusTypeDef Proto_SendIMUData(imu_data_t* imu_data);

#endif /* PROTO_UTILS_H */