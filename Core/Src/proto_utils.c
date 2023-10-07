#include <proto_utils.h>

#define UART_BUFFER_LEN imu_data_t_size

static UART_HandleTypeDef* huart;

static uint8_t uart_buffer[UART_BUFFER_LEN];

void Proto_Setup(UART_HandleTypeDef* uart_handle) {
    huart = uart_handle;
}

HAL_StatusTypeDef Proto_SendIMUData(imu_data_t* imu_data) {
    bool nanopb_ret;
    HAL_StatusTypeDef ret;

    pb_ostream_t stream = pb_ostream_from_buffer(uart_buffer, UART_BUFFER_LEN);

    nanopb_ret = pb_encode(&stream, imu_data_t_fields, imu_data);
    if (nanopb_ret != true) return HAL_ERROR;

    ret = HAL_UART_Transmit(huart, (uint8_t*)&stream.bytes_written, 4, UART_DELAY_TIMEOUT);
    if (ret != HAL_OK) return ret;

    if (stream.bytes_written != 0) {
        HAL_UART_Transmit(huart, (uint8_t*)uart_buffer, stream.bytes_written, UART_DELAY_TIMEOUT);
        if (ret != HAL_OK) return ret;
    }

    return HAL_OK;
}