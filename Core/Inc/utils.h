#ifndef UTILS_H
#define UTILS_H

#include <stm32l4xx_hal.h>

void UTIL_Setup(UART_HandleTypeDef* uart_handle);

void UTIL_Error(const char* err_msg_format, ...);

#endif /* UTILS_H */