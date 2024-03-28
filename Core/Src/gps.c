#include "gps.h"

#include <stdio.h>
#include <string.h>

#include <minmea.h>

#include "can.h"
#include "main.h"

#define GPS_SENTENCE_LEN MINMEA_MAX_SENTENCE_LENGTH
#define GPS_SENTENCE_BUF_COUNT 10

#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"

static UART_HandleTypeDef *huart;

static uint8_t gps_buffer[GPS_SENTENCE_BUF_COUNT][GPS_SENTENCE_LEN];
static volatile uint8_t rx_char_pos;
static volatile uint8_t rx_sentence_num;
static volatile uint8_t tx_sentence_num;

static int fix_quality;
static GPS_data_t current_data;

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

HAL_StatusTypeDef GPS_PrintBuffer(UART_HandleTypeDef *output_huart_handle) {
    HAL_StatusTypeDef ret;
    // New sentences to process
    while (rx_sentence_num != tx_sentence_num) {
        uint16_t len = (uint16_t)strnlen((char*)gps_buffer[tx_sentence_num], GPS_SENTENCE_LEN);
        ret = HAL_UART_Transmit(output_huart_handle, gps_buffer[tx_sentence_num], len, 100);
        if (ret != HAL_OK) return ret;

        tx_sentence_num++;
        tx_sentence_num = tx_sentence_num % GPS_SENTENCE_BUF_COUNT;
    }

    return HAL_OK;
}

static void GPS_Parse_RMC(char* nmea_sentence) {
    struct minmea_sentence_rmc frame;
    minmea_parse_rmc(&frame, nmea_sentence);

    current_data.time.year = (uint8_t)frame.date.year;
    current_data.time.month = (uint8_t)frame.date.month;
    current_data.time.day = (uint8_t)frame.date.day;
    current_data.time.hours = (uint8_t)frame.time.hours;
    current_data.time.minutes = (uint8_t)frame.time.minutes;
    current_data.time.seconds = (uint8_t)frame.time.seconds;
    current_data.latitude = minmea_tocoord(&frame.latitude);
    current_data.longitude = minmea_tocoord(&frame.longitude);
    current_data.speed = minmea_tofloat(&frame.speed);
    current_data.course = minmea_tofloat(&frame.course);
}

static void GPS_Parse_GGA(char* nmea_sentence) {
    struct minmea_sentence_gga frame;
    minmea_parse_gga(&frame, nmea_sentence);

    fix_quality = frame.fix_quality;

    current_data.latitude = minmea_tocoord(&frame.latitude);
    current_data.longitude = minmea_tocoord(&frame.longitude);
    current_data.altitude = minmea_tofloat(&frame.altitude);
    if (frame.altitude_units != 'M') {
        Error_Handler();
    }
}

void GPS_ProcessBuffer() {
    // New sentences to process
    while (rx_sentence_num != tx_sentence_num) {
        char* nmea_sentence = (char*)gps_buffer[tx_sentence_num];
        enum minmea_sentence_id id = minmea_sentence_id(nmea_sentence, false);

        switch (id) {
            case MINMEA_SENTENCE_RMC:
                GPS_Parse_RMC(nmea_sentence);
                break;

            case MINMEA_SENTENCE_GGA:
                GPS_Parse_GGA(nmea_sentence);
                break;

            default:
                break;
        }

        tx_sentence_num++;
        tx_sentence_num = tx_sentence_num % GPS_SENTENCE_BUF_COUNT;
    }
}

HAL_StatusTypeDef GPS_SendSerial(UART_HandleTypeDef *output_huart_handle) {
    char uart_buf[300];
    int len = snprintf(uart_buf, sizeof(uart_buf),
                       "GPS Data: "
                       "time = %u:%u:%u, latitude = %f, longitude = %f, "
                       "speed = %f, course = %f, altitude = %f\r\n",
                       current_data.time.hours, current_data.time.minutes, current_data.time.seconds,
                       current_data.latitude, current_data.longitude,
                       current_data.speed, current_data.course, current_data.altitude);

    return HAL_UART_Transmit(output_huart_handle, (uint8_t*)uart_buf, len, 1000);
}

HAL_StatusTypeDef GPS_SendCAN() {
    HAL_StatusTypeDef ret;

    ret = CAN_SendGPSTime(&current_data.time);
    if (ret != HAL_OK) return ret;

    ret = CAN_SendGPSPos(&current_data.latitude, &current_data.longitude);
    if (ret != HAL_OK) return ret;

    ret = CAN_SendGPSAltVel(&current_data.altitude, &current_data.speed);
    if (ret != HAL_OK) return ret;

    return HAL_OK;
}

HAL_StatusTypeDef GPS_UARTRxCpltHandler(UART_HandleTypeDef *huart) {
    HAL_StatusTypeDef ret;
    if (gps_buffer[rx_sentence_num][rx_char_pos] == '\n') {
        gps_buffer[rx_sentence_num][rx_char_pos + 1] = '\0';
        rx_sentence_num++;
        rx_sentence_num = rx_sentence_num % GPS_SENTENCE_BUF_COUNT;
        rx_char_pos = 0;

        // Have filled all GPS_SENTENCE_BUF_COUNT buffers
        if (rx_sentence_num == tx_sentence_num) Error_Handler();
    }
    else {
        rx_char_pos++;
        // Need space to add null byte after next received character
        if (rx_char_pos + 1 >= GPS_SENTENCE_LEN) Error_Handler();
    }
    ret = HAL_UART_Receive_IT(huart, &gps_buffer[rx_sentence_num][rx_char_pos], 1);
    return ret;
}