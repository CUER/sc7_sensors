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

void TrimRead(void);
void BME280_WakeUP(void);
void BME280_Measure (void);

// Oversampling definitions
#define OSRS_OFF    	0x00
#define OSRS_1      	0x01
#define OSRS_2      	0x02
#define OSRS_4      	0x03
#define OSRS_8      	0x04
#define OSRS_16     	0x05

// MODE Definitions
#define MODE_SLEEP      0x00
#define MODE_FORCED     0x01
#define MODE_NORMAL     0x03

// Standby Time
#define T_SB_0p5    	0x00
#define T_SB_62p5   	0x01
#define T_SB_125    	0x02
#define T_SB_250    	0x03
#define T_SB_500    	0x04
#define T_SB_1000   	0x05
#define T_SB_10     	0x06
#define T_SB_20     	0x07

// IIR Filter Coefficients
#define IIR_OFF     	0x00
#define IIR_2       	0x01
#define IIR_4       	0x02
#define IIR_8       	0x03
#define IIR_16      	0x04

// REGISTERS DEFINITIONS
#define ID_REG      	0xD0
#define RESET_REG  		0xE0
#define CTRL_HUM_REG    0xF2
#define STATUS_REG      0xF3
#define CTRL_MEAS_REG   0xF4
#define CONFIG_REG      0xF5
#define PRESS_MSB_REG   0xF7


#endif /* BME280_H */