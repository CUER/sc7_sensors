#include "bme280.h"

static I2C_HandleTypeDef *hi2c;

static HAL_StatusTypeDef I2C_Write_Data(uint8_t addr, uint8_t* data, uint16_t bytes) {
    return HAL_I2C_Mem_Write(hi2c, BME280_ADDR, addr, 1, data, bytes, I2C_DELAY_TIMEOUT);
}

static HAL_StatusTypeDef I2C_Read_Data(uint8_t addr, uint8_t* data, uint16_t bytes) {
    return HAL_I2C_Mem_Read(hi2c, BME280_ADDR, addr, 1, data, bytes, I2C_DELAY_TIMEOUT);
}

HAL_StatusTypeDef BME280_Connect(I2C_HandleTypeDef *hi2c_handle) {
    uint8_t buf[12];
    HAL_StatusTypeDef ret;

    hi2c = hi2c_handle;

    // Check ID contents
    ret = I2C_Read_Data(BME280_ID, buf, 1);
    if (ret != HAL_OK) return ret;
    if (buf[0] != BME280_CHIP_ID) return HAL_ERROR;

    // // Software reset
    buf[0] = 0xB6;  // Magic value from datasheet
    ret = I2C_Write_Data(BME280_RESET, buf, 1);

    return ret;
}
