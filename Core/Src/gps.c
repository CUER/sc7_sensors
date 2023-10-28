#include "lsm6ds.h"

#include <string.h>

#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"

static UART_HandleTypeDef *huart;

#define GPS_BUF_LEN 100

static uint8_t gps_buffer[GPS_BUF_LEN];
static volatile uint64_t rx_pos;
static volatile uint64_t tx_pos;
static volatile long bytes_received;

HAL_StatusTypeDef GPS_Connect(UART_HandleTypeDef *huart_handle) {
    HAL_StatusTypeDef ret;
    huart = huart_handle;

    bytes_received = 0;
    tx_pos = 0;
    rx_pos = 0;

    ret = HAL_UART_Transmit(huart, (uint8_t*)PMTK_SET_NMEA_OUTPUT_RMCGGA, strlen(PMTK_SET_NMEA_OUTPUT_RMCGGA), 100);
    if (ret != HAL_OK) return ret;

    ret = HAL_UART_Receive_IT(huart, gps_buffer, 1);
    return ret;
}

HAL_StatusTypeDef GPS_SendRxData(UART_HandleTypeDef *output_huart_handle) {
    HAL_StatusTypeDef ret;
    if (tx_pos != rx_pos) {
      ret = HAL_UART_Transmit(output_huart_handle, &gps_buffer[tx_pos], 1, 100);
      if (ret != HAL_OK) return ret;
      tx_pos++;
      tx_pos = tx_pos % GPS_BUF_LEN;
    }
    return HAL_OK;
}

HAL_StatusTypeDef GPS_UARTRxCpltHandler(UART_HandleTypeDef *huart) {
    HAL_StatusTypeDef ret;
    bytes_received++;
    rx_pos++;
    rx_pos = rx_pos % GPS_BUF_LEN;
    ret = HAL_UART_Receive_IT(huart, &gps_buffer[rx_pos], 1);
    return ret;
}