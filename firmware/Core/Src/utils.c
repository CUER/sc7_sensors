#include "utils.h"

#include <stdarg.h>
#include <stdio.h>

#include "main.h"

static UART_HandleTypeDef* huart;

/**
 * @brief Get delay time to use when HAL_Delay is not enabled
 * @note use for (voltatile int i = 0; i < delay_time; i++) for a delay of ~1s
*/
#define NO_IRQ_DELAY_TIME (HAL_RCC_GetSysClockFreq() / 10)

void UTIL_Setup(UART_HandleTypeDef* uart_handle) {
    huart = uart_handle;
}

void UTIL_Error(const char* err_msg_format, ...) {
    // NOTE: interrupts disabled, HAL_Delay not longer functioning
    __disable_irq();

    HAL_UART_Abort(huart);
    uint32_t delay_time = NO_IRQ_DELAY_TIME;

    char uart_buf[200];
    va_list vl;
    va_start(vl, err_msg_format);
    int len = vsnprintf(uart_buf, sizeof(uart_buf), err_msg_format, vl);
    va_end(vl);

    while (1) {
        for (volatile uint32_t i = 0; i < delay_time; i++);

        // Toggle LED
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

        // Print error message
        HAL_UART_Transmit(huart, (uint8_t*)uart_buf, len, 100);
    }
}