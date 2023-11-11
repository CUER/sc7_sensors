#ifndef __BME280_H__
#define __BME280_H__

#include <stdint.h>

#include "stm32l4xx_hal.h"

#define I2C_DELAY_TIMEOUT HAL_MAX_DELAY
#define BME280_ADDRESS (0x77 << 1)  
#define BME280_ADDRESS_ALTERNATE (0x76)


#define BME280_REGISTER_DIG_T1 (0x80)
#define BME280_REGISTER_DIG_T2 (0x8A)
#define BME280_REGISTER_DIG_T3 (0x8C)
#define BME280_REGISTER_CONTROLHUMID (0xF2)
#define BME280_REGISTER_STATUS (0XF3)
#define BME280_REGISTER_CONTROL (0xF4)
#define BME280_REGISTER_CONFIG (0xF5)
#define BME280_REGISTER_PRESSUREDATA (0xF7)
#define BME280_REGISTER_TEMPDATA (0xFA)
#define BME280_REGISTER_HUMIDDATA (0xFD)
#define BME280_REGISTER_CHIPID (0xD0)
#define BME280_REGISTER_VERSION (0xD1)
#define BME280_REGISTER_SOFTRESET (0xE0)

// typedef enum {
//   BME280_REGISTER_DIG_T1 = 0x88,
//   BME280_REGISTER_DIG_T2 = 0x8A,
//   BME280_REGISTER_DIG_T3 = 0x8C,

//   BME280_REGISTER_DIG_P1 = 0x8E,
//   BME280_REGISTER_DIG_P2 = 0x90,
//   BME280_REGISTER_DIG_P3 = 0x92,
//   BME280_REGISTER_DIG_P4 = 0x94,
//   BME280_REGISTER_DIG_P5 = 0x96,
//   BME280_REGISTER_DIG_P6 = 0x98,
//   BME280_REGISTER_DIG_P7 = 0x9A,
//   BME280_REGISTER_DIG_P8 = 0x9C,
//   BME280_REGISTER_DIG_P9 = 0x9E,

//   BME280_REGISTER_DIG_H1 = 0xA1,
//   BME280_REGISTER_DIG_H2 = 0xE1,
//   BME280_REGISTER_DIG_H3 = 0xE3,
//   BME280_REGISTER_DIG_H4 = 0xE4,
//   BME280_REGISTER_DIG_H5 = 0xE5,
//   BME280_REGISTER_DIG_H6 = 0xE7,

//   BME280_REGISTER_CHIPID = 0xD0,
//   BME280_REGISTER_VERSION = 0xD1,
//   BME280_REGISTER_SOFTRESET = 0xE0,

//   BME280_REGISTER_CAL26 = 0xE1, // R calibration stored in 0xE1-0xF0

//   BME280_REGISTER_CONTROLHUMID = 0xF2,
//   BME280_REGISTER_STATUS = 0XF3,
//   BME280_REGISTER_CONTROL = 0xF4,
//   BME280_REGISTER_CONFIG = 0xF5,
//   BME280_REGISTER_PRESSUREDATA = 0xF7,
//   BME280_REGISTER_TEMPDATA = 0xFA,
//   BME280_REGISTER_HUMIDDATA = 0xFD
// };

enum sensor_mode {
    MODE_SLEEP = 0b00,
    MODE_FORCED = 0b01,
    MODE_NORMAL = 0b11
};
void setSampling(sensor_mode mode = MODE_NORMAL,
                sensor_sampling tempSampling = SAMPLING_X16,
                sensor_sampling pressSampling = SAMPLING_X16,
                sensor_sampling humSampling = SAMPLING_X16,
                sensor_filter filter = FILTER_OFF,
                standby_duration duration = STANDBY_MS_0_5);

typedef struct {
  uint16_t dig_T1; ///< temperature compensation value
  int16_t dig_T2;  ///< temperature compensation value
  int16_t dig_T3;  ///< temperature compensation value

  uint16_t dig_P1; ///< pressure compensation value
  int16_t dig_P2;  ///< pressure compensation value
  int16_t dig_P3;  ///< pressure compensation value
  int16_t dig_P4;  ///< pressure compensation value
  int16_t dig_P5;  ///< pressure compensation value
  int16_t dig_P6;  ///< pressure compensation value
  int16_t dig_P7;  ///< pressure compensation value
  int16_t dig_P8;  ///< pressure compensation value
  int16_t dig_P9;  ///< pressure compensation value

  uint8_t dig_H1; ///< humidity compensation value
  int16_t dig_H2; ///< humidity compensation value
  uint8_t dig_H3; ///< humidity compensation value
  int16_t dig_H4; ///< humidity compensation value
  int16_t dig_H5; ///< humidity compensation value
  int8_t dig_H6;  ///< humidity compensation value
} bme280_calib_data;

HAL_StatusTypeDef BME280_Connect(I2C_HandleTypeDef *hi2c);

HAL_StatusTypeDef BME280_GetTemp(float *result);

HAL_StatusTypeDef BME280_GetPressure(float *result);

HAL_StatusTypeDef BME280_GetHumidity(float *result);

#endif