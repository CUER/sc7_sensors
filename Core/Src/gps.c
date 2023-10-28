#include "lsm6ds.h"

#include <string.h>

#include <minmea.h>

#define GPS_SENTENCE_LEN MINMEA_MAX_SENTENCE_LENGTH
#define GPS_SENTENCE_BUF_COUNT 10

#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"

static UART_HandleTypeDef *huart;

static uint8_t gps_buffer[GPS_SENTENCE_BUF_COUNT][GPS_SENTENCE_LEN];
static volatile uint8_t rx_char_pos;
static volatile uint8_t rx_sentence_num;
static volatile uint8_t tx_sentence_num;

HAL_StatusTypeDef GPS_Connect(UART_HandleTypeDef *huart_handle) {
    HAL_StatusTypeDef ret;
    huart = huart_handle;

    rx_char_pos = 0;
    rx_sentence_num = 0;
    tx_sentence_num = 0;

    ret = HAL_UART_Transmit(huart, (uint8_t*)PMTK_SET_NMEA_OUTPUT_RMCGGA, strlen(PMTK_SET_NMEA_OUTPUT_RMCGGA), 100);
    if (ret != HAL_OK) return ret;

    ret = HAL_UART_Receive_IT(huart, gps_buffer[rx_sentence_num], 1);
    return ret;
}

HAL_StatusTypeDef GPS_SendRxData(UART_HandleTypeDef *output_huart_handle) {
    HAL_StatusTypeDef ret;
    if (rx_sentence_num != tx_sentence_num) {
        uint16_t len = strlen((char*)gps_buffer[tx_sentence_num]);
        ret = HAL_UART_Transmit(output_huart_handle, gps_buffer[tx_sentence_num], len, 100);
        if (ret != HAL_OK) return ret;
        tx_sentence_num++;
        tx_sentence_num = tx_sentence_num % GPS_SENTENCE_BUF_COUNT;
    }
    return HAL_OK;
}

HAL_StatusTypeDef GPS_UARTRxCpltHandler(UART_HandleTypeDef *huart) {
    HAL_StatusTypeDef ret;
    if (gps_buffer[rx_sentence_num][rx_char_pos] == '\n') {
        gps_buffer[rx_sentence_num][rx_char_pos + 1] = '\0';
        rx_sentence_num++;
        rx_sentence_num = rx_sentence_num % GPS_SENTENCE_BUF_COUNT;
        rx_char_pos = 0;
    }
    else {
        rx_char_pos++;
        if (rx_char_pos >= GPS_SENTENCE_LEN) return HAL_ERROR;
    }
    ret = HAL_UART_Receive_IT(huart, &gps_buffer[rx_sentence_num][rx_char_pos], 1);
    return ret;
}