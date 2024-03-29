#include "lsm6ds.h"

#define GRAVITY (9.80665F)
#define DPS_TO_RADS (0.017453293F)

static I2C_HandleTypeDef *hi2c;

static lsm6ds_accel_range_t accel_range;
static lsm6ds_gyro_range_t gyro_range;

static HAL_StatusTypeDef I2C_Write_Data(uint8_t addr, uint8_t* data, uint16_t bytes) {
    return HAL_I2C_Mem_Write(hi2c, LSM6DS_ADDR, addr, 1, data, bytes, I2C_DELAY_TIMEOUT);
    // HAL_StatusTypeDef ret;

    // addr |= (1 << 7);

    // ret = HAL_I2C_Master_Transmit(hi2c, LSM6DS_ADDR, &addr, 1, I2C_DELAY_TIMEOUT);
    // if (ret != HAL_OK) return ret;
    // ret = HAL_I2C_Master_Transmit(hi2c, LSM6DS_ADDR, data, bytes, I2C_DELAY_TIMEOUT);
    // return ret;
}

static HAL_StatusTypeDef I2C_Read_Data(uint8_t addr, uint8_t* data, uint16_t bytes) {
    return HAL_I2C_Mem_Read(hi2c, LSM6DS_ADDR, addr, 1, data, bytes, I2C_DELAY_TIMEOUT);
    // HAL_StatusTypeDef ret;

    // ret = HAL_I2C_Master_Transmit(hi2c, LSM6DS_ADDR, &addr, 1, I2C_DELAY_TIMEOUT);
    // if (ret != HAL_OK) return ret;
    // ret = HAL_I2C_Master_Receive(hi2c, LSM6DS_ADDR, data, bytes, I2C_DELAY_TIMEOUT);
    // return ret;
}

HAL_StatusTypeDef LSM6DS_Connect(I2C_HandleTypeDef *hi2c_handle) {
    uint8_t buf[12];
    HAL_StatusTypeDef ret;

    hi2c = hi2c_handle;

    // Check WHOAMI contents
    ret = I2C_Read_Data(LSM6DS_WHOAMI, buf, 1);
    if (ret != HAL_OK) return ret;
    if (buf[0] != LSM6DSO32_CHIP_ID) return HAL_ERROR;

    // Software reset
    ret = I2C_Read_Data(LSM6DS_CTRL3_C, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] |= 0x01;
    ret = I2C_Write_Data(LSM6DS_CTRL3_C, buf, 1);
    if (ret != HAL_OK) return ret;

    while ((buf[0] & 1) != 0) {
        ret = I2C_Read_Data(LSM6DS_CTRL3_C, buf, 1);
        if (ret != HAL_OK) return ret;
    }

    // Block data update before reading
    buf[0] |= 0x80;
    ret = I2C_Write_Data(LSM6DS_CTRL3_C, buf, 1);
    if (ret != HAL_OK) return ret;

    // Disable I3C interface
    ret = I2C_Read_Data(LSM6DS_CTRL9_XL, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] |= 0x02;
    ret = I2C_Write_Data(LSM6DS_CTRL9_XL, buf, 1);

    return ret;
}

HAL_StatusTypeDef LSM6DS_SetAccelDataRate(lsm6ds_data_rate_t rate) {
    HAL_StatusTypeDef ret;
    uint8_t buf[1];

    ret = I2C_Read_Data(LSM6DS_CTRL1_XL, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] &= ~(0b1111 << 4);
    buf[0] |= (rate << 4);

    ret = I2C_Write_Data(LSM6DS_CTRL1_XL, buf, 1);

    return ret;
}

HAL_StatusTypeDef LSM6DS_SetAccelRange(lsm6ds_accel_range_t range) {
    HAL_StatusTypeDef ret;
    uint8_t buf[1];

    ret = I2C_Read_Data(LSM6DS_CTRL1_XL, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] &= ~(0b11 << 2);
    buf[0] |= (range << 2);

    ret = I2C_Write_Data(LSM6DS_CTRL1_XL, buf, 1);
    if (ret != HAL_OK) accel_range = range;

    return ret;
}

HAL_StatusTypeDef LSM6DS_SetGyroDataRate(lsm6ds_data_rate_t rate) {
    HAL_StatusTypeDef ret;
    uint8_t buf[1];

    ret = I2C_Read_Data(LSM6DS_CTRL2_G, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] &= ~(0b1111 << 4);
    buf[0] |= (rate << 4);

    ret = I2C_Write_Data(LSM6DS_CTRL2_G, buf, 1);

    return ret;
}

HAL_StatusTypeDef LSM6DS_SetGyroRange(lsm6ds_gyro_range_t range) {
    HAL_StatusTypeDef ret;
    uint8_t buf[1];

    ret = I2C_Read_Data(LSM6DS_CTRL2_G, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] &= ~(0b111 << 1);
    buf[0] |= (range << 1);

    ret = I2C_Write_Data(LSM6DS_CTRL2_G, buf, 1);
    if (ret != HAL_OK) gyro_range = range;

    return ret;
}

HAL_StatusTypeDef LSM6DS_GetTemp(float *result) {
    HAL_StatusTypeDef ret;
    uint8_t buf[2];

    ret = I2C_Read_Data(LSM6DS_OUT_TEMP_L, buf, 2);
    if (ret != HAL_OK) return ret;

    int16_t rawTemp = buf[1] << 8 | buf[0];
    *result = (rawTemp / 256.0) + 25.0;

    return HAL_OK;
}

HAL_StatusTypeDef LSM6DS_GetAccel(lsm6ds_data_t *result) {
    HAL_StatusTypeDef ret;
    uint8_t buf[6];

    ret = I2C_Read_Data(LSM6DS_OUTX_L_A, buf, 6);
    if (ret != HAL_OK) return ret;

    int16_t accel_x = buf[1] << 8 | buf[0];
    int16_t accel_y = buf[3] << 8 | buf[2];
    int16_t accel_z = buf[5] << 8 | buf[4];

    float accel_scale = 1; // range is in milli-g
    if (accel_range == LSM6DSO32_ACCEL_RANGE_32_G)
        accel_scale = 0.976;
    if (accel_range == LSM6DSO32_ACCEL_RANGE_16_G)
        accel_scale = 0.488;
    if (accel_range == LSM6DSO32_ACCEL_RANGE_8_G)
        accel_scale = 0.244;
    if (accel_range == LSM6DSO32_ACCEL_RANGE_4_G)
        accel_scale = 0.122;
    (*result).x = accel_x * accel_scale * GRAVITY / 1000;
    (*result).y = accel_y * accel_scale * GRAVITY / 1000;
    (*result).z = accel_z * accel_scale * GRAVITY / 1000;

    return HAL_OK;
}

HAL_StatusTypeDef LSM6DS_GetGyro(lsm6ds_data_t *result) {
    HAL_StatusTypeDef ret;
    uint8_t buf[6];

    ret = I2C_Read_Data(LSM6DS_OUTX_L_G, buf, 6);
    if (ret != HAL_OK) return ret;

    int16_t gyro_x = buf[1] << 8 | buf[0];
    int16_t gyro_y = buf[3] << 8 | buf[2];
    int16_t gyro_z = buf[5] << 8 | buf[4];

    float gyro_scale = 1; // range is in milli-dps
    if (gyro_range == LSM6DS_GYRO_RANGE_2000_DPS)
        gyro_scale = 70.0;
    if (gyro_range == LSM6DS_GYRO_RANGE_1000_DPS)
        gyro_scale = 35.0;
    if (gyro_range == LSM6DS_GYRO_RANGE_500_DPS)
        gyro_scale = 17.50;
    if (gyro_range == LSM6DS_GYRO_RANGE_250_DPS)
        gyro_scale = 8.75;
    if (gyro_range == LSM6DS_GYRO_RANGE_125_DPS)
        gyro_scale = 4.375;
    (*result).x = gyro_x * gyro_scale * DPS_TO_RADS / 1000;
    (*result).y = gyro_y * gyro_scale * DPS_TO_RADS / 1000;
    (*result).z = gyro_z * gyro_scale * DPS_TO_RADS / 1000;

    return HAL_OK;
}
