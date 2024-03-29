#ifndef LSM6DS_H
#define LSM6DS_H

#include <stdint.h>

#include "stm32l4xx_hal.h"

#define I2C_DELAY_TIMEOUT HAL_MAX_DELAY
#define LSM6DS_ADDR (0x6A << 1)
#define LSM6DSO32_CHIP_ID 0x6C

#define LSM6DS_WHOAMI 0x0F
#define LSM6DS_CTRL3_C 0x12
#define LSM6DS_CTRL9_XL 0x18

#define LSM6DS_CTRL1_XL 0x10
#define LSM6DS_CTRL2_G 0x11
#define LSM6DS_STATUS_REG 0x1E
#define LSM6DS_OUT_TEMP_L 0x20
#define LSM6DS_OUTX_L_G 0x22
#define LSM6DS_OUTX_L_A 0x28

typedef enum {
  LSM6DS_RATE_SHUTDOWN,
  LSM6DS_RATE_12_5_HZ,
  LSM6DS_RATE_26_HZ,
  LSM6DS_RATE_52_HZ,
  LSM6DS_RATE_104_HZ,
  LSM6DS_RATE_208_HZ,
  LSM6DS_RATE_416_HZ,
  LSM6DS_RATE_833_HZ,
  LSM6DS_RATE_1_66K_HZ,
  LSM6DS_RATE_3_33K_HZ,
  LSM6DS_RATE_6_66K_HZ,
} lsm6ds_data_rate_t;

typedef enum {
  LSM6DSO32_ACCEL_RANGE_4_G,
  LSM6DSO32_ACCEL_RANGE_32_G,
  LSM6DSO32_ACCEL_RANGE_8_G,
  LSM6DSO32_ACCEL_RANGE_16_G
} lsm6ds_accel_range_t;

typedef enum {
  LSM6DS_GYRO_RANGE_125_DPS = 0b001,
  LSM6DS_GYRO_RANGE_250_DPS = 0b000,
  LSM6DS_GYRO_RANGE_500_DPS = 0b010,
  LSM6DS_GYRO_RANGE_1000_DPS = 0b100,
  LSM6DS_GYRO_RANGE_2000_DPS = 0b110
} lsm6ds_gyro_range_t;

typedef struct {
  lsm6ds_data_rate_t accel_data_rate;
  lsm6ds_accel_range_t accel_range;
  lsm6ds_data_rate_t gyro_data_rate;
  lsm6ds_gyro_range_t gyro_range;
} lsm6ds_config_t;

typedef struct {
  float x;
  float y;
  float z;
} lsm6ds_data_t;

HAL_StatusTypeDef LSM6DS_Connect(I2C_HandleTypeDef *hi2c);

HAL_StatusTypeDef LSM6DS_SetAccelDataRate(lsm6ds_data_rate_t rate);

HAL_StatusTypeDef LSM6DS_SetAccelRange(lsm6ds_accel_range_t range);

HAL_StatusTypeDef LSM6DS_SetGyroDataRate(lsm6ds_data_rate_t rate);

HAL_StatusTypeDef LSM6DS_SetGyroRange(lsm6ds_gyro_range_t range);

HAL_StatusTypeDef LSM6DS_GetTemp(float *result);

HAL_StatusTypeDef LSM6DS_GetAccel(lsm6ds_data_t *result);

HAL_StatusTypeDef LSM6DS_GetGyro(lsm6ds_data_t *result);

#endif /* LSM6DS_H */