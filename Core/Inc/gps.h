/**
 * @file
 * @brief Driver for Adafruit Ultimate GPS module
 * @note Works by receiving individual bytes over UART using interrupts,
 * then having other functions called in the main thread to process received data
*/
#ifndef GPS_H
#define GPS_H

#include <stdint.h>
#include <time.h>

#include "stm32l4xx_hal.h"

typedef struct {
    uint8_t year;  // Only last two digits
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} __attribute__ ((packed)) GPS_time_t;

typedef struct {
    GPS_time_t time;
    float latitude;
    float longitude;
    float speed;
    float course;
    float altitude;
} GPS_data_t;

HAL_StatusTypeDef GPS_Connect(UART_HandleTypeDef *huart_handle);

/**
 * @brief Forwards any received NMEA sentences to output UART
 * @param[in] output_huart_handle HAL Handle to UART to forward sentence over
 * @return Function success status
 * 
 * @note Received NMEA sentences are consumed, and cannot be processed again
*/
HAL_StatusTypeDef GPS_PrintBuffer(UART_HandleTypeDef *output_huart_handle);

/**
 * @brief Processes received NMEA sentences and updates current_data variable
 * 
 * @note Current data is only updated if GPS has a fix
*/
void GPS_ProcessBuffer();

/**
 * @brief Sends most recent data over UART
 * @param[in] output_huart_handle HAL Handle to UART to send data over
 * @return Function success status
 * 
 * @note Data is sent regardless of validity
*/
HAL_StatusTypeDef GPS_SendSerial(UART_HandleTypeDef *output_huart_handle);

/**
 * @brief Sends CAN messages with most recent data
 * @return Function success status
 * 
 * @note Data is sent regardless of validity
*/
HAL_StatusTypeDef GPS_SendCAN();

HAL_StatusTypeDef GPS_UARTRxCpltHandler();

#endif /* GPS_H */