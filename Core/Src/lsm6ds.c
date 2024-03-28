#include "lsm6ds.h"

#define GRAVITY (9.80665F)
#define DPS_TO_RADS (0.017453293F)

static I2C_HandleTypeDef* hi2c;

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

    // Write datarate to accelerometer control register 1
    ret = I2C_Read_Data(LSM6DS_CTRL1_XL, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] &= ~(0b1111 << 4);
    buf[0] |= (rate << 4);

    ret = I2C_Write_Data(LSM6DS_CTRL1_XL, buf, 1);

    // Write datarate to FIFO control register 3
    // FIFO only enabled when FIFO control register 4 is also written to
    ret = I2C_Read_Data(LSM6DS_FIFO_CTRL3, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] &= ~0b1111;
    buf[0] |= rate;

    ret = I2C_Write_Data(LSM6DS_FIFO_CTRL3, buf, 1);

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

    // Write datarate to gyro control register 1
    ret = I2C_Read_Data(LSM6DS_CTRL2_G, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] &= ~(0b1111 << 4);
    buf[0] |= (rate << 4);

    ret = I2C_Write_Data(LSM6DS_CTRL2_G, buf, 1);

    // Write datarate to FIFO control register 3
    // FIFO only enabled when FIFO control register 4 is also written to
    ret = I2C_Read_Data(LSM6DS_FIFO_CTRL3, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] &= ~(0b1111 << 4);
    buf[0] |= (rate << 4);

    ret = I2C_Write_Data(LSM6DS_FIFO_CTRL3, buf, 1);

    return ret;
}

HAL_StatusTypeDef LSM6DS_SetGyroRange(lsm6ds_gyro_range_t range) {
    HAL_StatusTypeDef ret;
    uint8_t buf[1];

    ret = I2C_Read_Data(LSM6DS_CTRL2_G, buf, 1);
    if (ret != HAL_OK) return ret;

    buf[0] &= ~(0b111 < 1);
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
    *result = (rawTemp / 256.0f) + 25.0f;

    return HAL_OK;
}

static void LSM6DS_DecodeAccel(uint8_t raw_bytes[6], lsm6ds_data_t* result) {
    int16_t accel_x = raw_bytes[1] << 8 | raw_bytes[0];
    int16_t accel_y = raw_bytes[3] << 8 | raw_bytes[2];
    int16_t accel_z = raw_bytes[5] << 8 | raw_bytes[4];

    float accel_scale;
    switch (accel_range) {
        // Using mg/LSB values from datasheet: Table 3. Mechanical characteristics
        case LSM6DSO32_ACCEL_RANGE_32_G:
            accel_scale = 0.976f * (GRAVITY / 1000);
            break;
        case LSM6DSO32_ACCEL_RANGE_16_G:
            accel_scale = 0.488f * (GRAVITY / 1000);
            break;
        case LSM6DSO32_ACCEL_RANGE_8_G:
            accel_scale = 0.244f * (GRAVITY / 1000);
            break;
        case LSM6DSO32_ACCEL_RANGE_4_G:
            accel_scale = 0.122f * (GRAVITY / 1000);
            break;
    }

    result->x = accel_x * accel_scale;
    result->y = accel_y * accel_scale;
    result->z = accel_z * accel_scale;
}

HAL_StatusTypeDef LSM6DS_GetAccel(lsm6ds_data_t *result) {
    HAL_StatusTypeDef ret;
    uint8_t buf[6];

    ret = I2C_Read_Data(LSM6DS_OUTX_L_A, buf, 6);
    if (ret != HAL_OK) return ret;

    LSM6DS_DecodeAccel(buf, result);

    return HAL_OK;
}

static void LSM6DS_DecodeGyro(uint8_t raw_bytes[6], lsm6ds_data_t* result) {
    int16_t gyro_x = raw_bytes[1] << 8 | raw_bytes[0];
    int16_t gyro_y = raw_bytes[3] << 8 | raw_bytes[2];
    int16_t gyro_z = raw_bytes[5] << 8 | raw_bytes[4];

    float gyro_scale;
    switch (gyro_range) {
        // Using mdps/LSB values from datasheet: Table 3. Mechanical characteristics
        case LSM6DS_GYRO_RANGE_2000_DPS:
            gyro_scale = 70.0f * (DPS_TO_RADS / 1000);
            break;
        case LSM6DS_GYRO_RANGE_1000_DPS:
            gyro_scale = 35.0f * (DPS_TO_RADS / 1000);
            break;
        case LSM6DS_GYRO_RANGE_500_DPS:
            gyro_scale = 17.50f * (DPS_TO_RADS / 1000);
            break;
        case LSM6DS_GYRO_RANGE_250_DPS:
            gyro_scale = 8.75f * (DPS_TO_RADS / 1000);
            break;
        case LSM6DS_GYRO_RANGE_125_DPS:
            gyro_scale = 4.375f * (DPS_TO_RADS / 1000);
            break;
    }

    result->x = gyro_x * gyro_scale;
    result->y = gyro_y * gyro_scale;
    result->z = gyro_z * gyro_scale;
}

HAL_StatusTypeDef LSM6DS_GetGyro(lsm6ds_data_t *result) {
    HAL_StatusTypeDef ret;
    uint8_t buf[6];

    ret = I2C_Read_Data(LSM6DS_OUTX_L_G, buf, 6);
    if (ret != HAL_OK) return ret;

    LSM6DS_DecodeGyro(buf, result);

    return HAL_OK;
}

HAL_StatusTypeDef LSM6DS_SetupFifo() {
    HAL_StatusTypeDef ret;
    uint8_t buf[1];

    // No watermark
    buf[0] = 0x00;
    ret = I2C_Write_Data(LSM6DS_FIFO_CTRL1, buf, 1);
    if (ret != HAL_OK) return ret;

    // No compression
    ret = I2C_Write_Data(LSM6DS_FIFO_CTRL2, buf, 1);
    if (ret != HAL_OK) return ret;

    // LSM6DS_FIFO_CTRL3 updated in SetDataRate functions

    // Enable FIFO mode
    buf[0] = 0b1;
    ret = I2C_Write_Data(LSM6DS_FIFO_CTRL4, buf, 1);
    if (ret != HAL_OK) return ret;

    return HAL_OK;
}

HAL_StatusTypeDef LSM6DS_ReadFifoRaw(uint16_t max_samples, uint16_t* samples_read, uint8_t* samples) {
    HAL_StatusTypeDef ret;
    uint8_t buf[2];

    ret = I2C_Read_Data(LSM6DS_FIFO_STATUS1, buf, 2);
    if (ret != HAL_OK) return ret;
    uint16_t samples_available =  ((buf[1] & 0b11) << 8) | buf[0];
    *samples_read = (samples_available < max_samples) ? samples_available : max_samples;

    if (*samples_read == 0) return HAL_OK;  // Do not try and read 0 bytes
    ret = I2C_Read_Data(LSM6DS_FIFO_DATA_OUT_TAG, samples, 7 * (*samples_read));
    if (ret != HAL_OK) return ret;

    return HAL_OK;
}

HAL_StatusTypeDef LSM6DS_ReadFifo(imu_data_t* imu_data) {
    HAL_StatusTypeDef ret;
    uint16_t max_accel_samples = sizeof(imu_data->accel_data) / sizeof(imu_data->accel_data[0]);
    uint16_t max_gyro_samples = sizeof(imu_data->gyro_data) / sizeof(imu_data->gyro_data[0]);
    uint16_t max_samples = (max_accel_samples < max_gyro_samples) ? max_accel_samples : max_gyro_samples;
    uint8_t buffer[max_samples * 7];
    uint16_t no_samples;

    ret = LSM6DS_ReadFifoRaw(max_samples, &no_samples, buffer);
    if (ret != HAL_OK) return ret;

    for (uint16_t i = 0; i < no_samples; i++) {
        lsm6ds_data_t data;
        uint8_t* current_sample = &buffer[7 * i];

        lsm6ds_fifo_tag_t tag = (current_sample[0] & 0xF8) >> 3;

        switch (tag) {
            case LSM6DS_FIFO_TAG_ACCEL:
                LSM6DS_DecodeAccel(&current_sample[1], &data);

                imu_data->accel_data[imu_data->accel_data_count].x = data.x;
                imu_data->accel_data[imu_data->accel_data_count].y = data.y;
                imu_data->accel_data[imu_data->accel_data_count].z = data.z;
                imu_data->accel_data_count++;
                break;

            case LSM6DS_FIFO_TAG_GYRO:
                LSM6DS_DecodeGyro(&current_sample[1], &data);

                imu_data->gyro_data[imu_data->gyro_data_count].x = data.x;
                imu_data->gyro_data[imu_data->gyro_data_count].y = data.y;
                imu_data->gyro_data[imu_data->gyro_data_count].z = data.z;
                imu_data->gyro_data_count++;
                break;

            case LSM6DS_FIFO_TAG_TEMP:
                break;
            
            default:
                return HAL_ERROR;
        }
    }

    return HAL_OK;
}
