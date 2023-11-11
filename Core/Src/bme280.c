#include "bme280.h"

static I2C_HandleTypeDef *hi2c;

static HAL_StatusTypeDef I2C_Write_Data(uint8_t addr, uint8_t* data, uint16_t bytes) {
    return HAL_I2C_Mem_Write(hi2c, BME280_ADDRESS, addr, 1, data, bytes, I2C_DELAY_TIMEOUT);
}

static HAL_StatusTypeDef I2C_Read_Data(uint8_t addr, uint8_t* data, uint16_t bytes) {
    return HAL_I2C_Mem_Read(hi2c, BME280_ADDRESS, addr, 1, data, bytes, I2C_DELAY_TIMEOUT);
}
int16_t kt1, kt2, kt3;

HAL_StatusTypeDef BME280_Connect(I2C_HandleTypeDef *hi2c_handle) {
    uint8_t buf[12];
    HAL_StatusTypeDef ret;

    hi2c = hi2c_handle;

    // Check ID contents
    ret = I2C_Read_Data(BME280_REGISTER_CHIPID, buf, 1);
    // if (ret != HAL_OK) return ret;
    if (buf[0] != 0x60) return HAL_ERROR;

    // Software reset
    buf[0] = 0xB6;  // Magic value from datasheet
    ret = I2C_Write_Data(BME280_REGISTER_SOFTRESET, buf, 1);

    return ret;
}

HAL_StatusTypeDef BME280_GetTemp(float *result) {
    HAL_StatusTypeDef ret;
    int32_t v1, v2;
    uint8_t buf[1];

    ret = I2C_Read_Data(BME280_REGISTER_TEMPDATA, buf, 1);

    *result = (float)(buf[0]);
	return HAL_OK;
}
