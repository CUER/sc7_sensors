/**
 * @file
 * @brief Useful macros and error-handling functions
*/
#ifndef UTILS_H
#define UTILS_H

#include "stm32l4xx_hal.h"

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

/**
 * @brief If not HAL_OK, raises error with file and line number
 * @param return_val HAL_StatusTypeDef to check
*/
#define CHECK_HAL_RETURN(return_val) \
    do { \
        HAL_StatusTypeDef cached_ret_val = (return_val); \
        if (cached_ret_val != HAL_OK) { \
            UTIL_Error("%s:%i: HAL failed with error code %i\r\n" \
            , __FILE__, __LINE__, cached_ret_val); \
        } \
    } while (0)

#endif /* UTILS_H */