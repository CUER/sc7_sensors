#include "lsm6ds.h"

static HAL_StatusTypeDef I2C_Write_Data(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t* data, uint16_t bytes) {
    HAL_StatusTypeDef ret;

    ret = HAL_I2C_Master_Transmit(hi2c, LSM6DS_ADDR, &addr, 1, I2C_DELAY_TIMEOUT);
    if (ret != HAL_OK) return ret;
    ret = HAL_I2C_Master_Transmit(hi2c, LSM6DS_ADDR, data, bytes, I2C_DELAY_TIMEOUT);
    return ret;
}

static HAL_StatusTypeDef I2C_Read_Data(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t* data, uint16_t bytes) {
    HAL_StatusTypeDef ret;

    ret = HAL_I2C_Master_Transmit(hi2c, LSM6DS_ADDR, &addr, 1, I2C_DELAY_TIMEOUT);
    if (ret != HAL_OK) return ret;
    ret = HAL_I2C_Master_Receive(hi2c, LSM6DS_ADDR, data, bytes, I2C_DELAY_TIMEOUT);
    return ret;
}

HAL_StatusTypeDef LSM6DS_Connect(I2C_HandleTypeDef *hi2c) {
    uint8_t buf[12];
    HAL_StatusTypeDef ret;

    // Check WHOAMI contents
    ret = I2C_Read_Data(hi2c, LSM6DS_WHOAMI, buf, 1);
    if (ret != HAL_OK) return ret;
    if (buf[0] != LSM6DSO32_CHIP_ID) return HAL_ERROR;

    // Software reset
    ret = I2C_Read_Data(hi2c, LSM6DS_CTRL3_C, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] |= 0x01;
    ret = I2C_Write_Data(hi2c, LSM6DS_CTRL3_C, buf, 1);

    while ((buf[0] & 1) != 0) {
        ret = I2C_Read_Data(hi2c, LSM6DS_CTRL3_C, buf, 1);
        if (ret != HAL_OK) return ret;
    }

    // Block data update before reading
    buf[0] |= 0x80;
    ret = I2C_Write_Data(hi2c, LSM6DS_CTRL3_C, buf, 1);
    if (ret != HAL_OK) return ret;

    // Disable I3C interface
    ret = I2C_Read_Data(hi2c, LSM6DS_CTRL9_XL, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] |= 0x02;
    ret = I2C_Write_Data(hi2c, LSM6DS_CTRL9_XL, buf, 1);

    return ret;
}

HAL_StatusTypeDef LSM6DS_SetAccelDataRate(I2C_HandleTypeDef *hi2c, lsm6ds_data_rate_t rate) {
    HAL_StatusTypeDef ret;
    uint8_t buf[1];

    ret = I2C_Read_Data(hi2c, LSM6DS_CTRL1_XL, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] &= ~(0b1111 << 4);
    buf[0] |= (rate << 4);

    ret = I2C_Write_Data(hi2c, LSM6DS_CTRL1_XL, buf, 1);

    return ret;
}

HAL_StatusTypeDef LSM6DS_SetAccelRange(I2C_HandleTypeDef *hi2c, lsm6ds_accel_range_t range) {
    HAL_StatusTypeDef ret;
    uint8_t buf[1];

    ret = I2C_Read_Data(hi2c, LSM6DS_CTRL1_XL, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] &= ~(0b11 << 2);
    buf[0] |= (range << 2);
    
    ret = I2C_Write_Data(hi2c, LSM6DS_CTRL1_XL, buf, 1);

    return ret;
}

HAL_StatusTypeDef LSM6DS_SetGyroDataRate(I2C_HandleTypeDef *hi2c, lsm6ds_data_rate_t rate) {
    HAL_StatusTypeDef ret;
    uint8_t buf[1];

    ret = I2C_Read_Data(hi2c, LSM6DS_CTRL2_G, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] &= ~(0b1111 << 4);
    buf[0] |= (rate << 4);

    ret = I2C_Write_Data(hi2c, LSM6DS_CTRL2_G, buf, 1);

    return ret;
}

HAL_StatusTypeDef LSM6DS_SetGyroRange(I2C_HandleTypeDef *hi2c, lsm6ds_gyro_range_t range) {
    HAL_StatusTypeDef ret;
    uint8_t buf[1];

    ret = I2C_Read_Data(hi2c, LSM6DS_CTRL2_G, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] &= ~(0b111 < 1);
    buf[0] |= (range << 1);

    ret = I2C_Write_Data(hi2c, LSM6DS_CTRL2_G, buf, 1);

    return ret;
}

HAL_StatusTypeDef LSM6DS_GetTemp(I2C_HandleTypeDef *hi2c, float *result) {
    HAL_StatusTypeDef ret;
    uint8_t buf[2];

    ret = I2C_Read_Data(hi2c, LSM6DS_OUT_TEMP_L, buf, 2);
    if (ret != HAL_OK) return ret;

    int16_t rawTemp = buf[1] << 8 | buf[0];
    *result = (rawTemp / 256.0) + 25.0;

    return HAL_OK;
}