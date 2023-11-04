#include "lsm6ds.h"

#include <stdio.h>
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
    char uart_buf[300];
    if (rx_sentence_num != tx_sentence_num) {
        char* nmea_sentence = (char*)gps_buffer[tx_sentence_num];
        enum minmea_sentence_id id = minmea_sentence_id(nmea_sentence, false);
        switch (id) {
            case MINMEA_SENTENCE_RMC: {
                struct minmea_sentence_rmc frame;
                if (minmea_parse_rmc(&frame, nmea_sentence)) {
                    uint16_t len;
                    len = snprintf(uart_buf, sizeof(uart_buf), "$xxRMC floating point degree coordinates and speed: (%f,%f) %f\r\n",
                                   minmea_tocoord(&frame.latitude),
                                   minmea_tocoord(&frame.longitude),
                                   minmea_tofloat(&frame.speed));
                    ret = HAL_UART_Transmit(output_huart_handle, (uint8_t*)uart_buf, len, 1000);
                    if (ret != HAL_OK) return ret;
                }
                else {

                }
            } break;

            case MINMEA_SENTENCE_GGA: {
                struct minmea_sentence_gga frame;
                if (minmea_parse_gga(&frame, nmea_sentence)) {
                    uint16_t len;
                    len = snprintf(uart_buf, sizeof(uart_buf), "$xxGGA: fix quality: %d\r\n", frame.fix_quality);
                    ret = HAL_UART_Transmit(output_huart_handle, (uint8_t*)uart_buf, len, 1000);
                    if (ret != HAL_OK) return ret;
                }
                else {

                }
            }

            case MINMEA_INVALID: {

            } break;

            default: {

            } break;
        }
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