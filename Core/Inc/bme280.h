#ifndef BME280_H
#define BME280_H

#include <stdint.h>

#include "stm32l4xx_hal.h"

#define I2C_DELAY_TIMEOUT HAL_MAX_DELAY
#define BME280_ADDR (0x77 << 1)
#define BME280_CHIP_ID 0x60

#define BME280_ID 0xD0
#define BME280_RESET 0xE0
#define BME280_STATUS 0xF3

HAL_StatusTypeDef BME280_Connect(I2C_HandleTypeDef *hi2c);

#endif /* BME280_H */