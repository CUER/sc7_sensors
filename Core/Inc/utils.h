/**
 * @file
 * @brief Useful macros and error-handling functions
*/
#ifndef UTILS_H
#define UTILS_H

#include <stm32l4xx_hal.h>
/**
 * @brief Null variable for compile-time use only, for example inside sizeof()
 * @param type Type of required variable
 * @return Null variable with type specified
*/
#define NULL_TYPE(type) (*((type*)0))

#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

void UTIL_Setup(UART_HandleTypeDef* uart_handle);

/**
 * @brief Error handler with support for printf-style error message
 * @note Disables interrupts and loops while printing error message and flashing LED
*/
void UTIL_Error(const char* err_msg_format, ...);

#endif /* UTILS_H */