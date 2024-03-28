#ifndef UTILS_H
#define UTILS_H

#include <stm32l4xx_hal.h>

#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

void UTIL_Setup(UART_HandleTypeDef* uart_handle);

void UTIL_Error(const char* err_msg_format, ...);

#endif /* UTILS_H */