#include "proto_utils.h"

#include "pb_encode.h"

#include "utils.h"

#define UART_BUFFER_LEN telemetry_packet_t_size

static UART_HandleTypeDef* huart;

static uint8_t uart_buffer[UART_BUFFER_LEN];

void Proto_Setup(UART_HandleTypeDef* uart_handle) {
    huart = uart_handle;
}

static void Proto_SendTelemetryPacket(telemetry_packet_t* telem_packet) {
    bool nanopb_ret;

    pb_ostream_t stream = pb_ostream_from_buffer(uart_buffer, UART_BUFFER_LEN);
    nanopb_ret = pb_encode(&stream, telemetry_packet_t_fields, &telem_packet);

    if (nanopb_ret != true) {
        UTIL_Error("%s:%i: NanoPB error with message: %s\r\n",
                    __FILE__, __LINE__, stream.errmsg);
    }

    CHECK_HAL_RETURN(HAL_UART_Transmit(huart, (uint8_t*)&stream.bytes_written, 4, UART_DELAY_TIMEOUT));

    if (stream.bytes_written == 0) return;

    CHECK_HAL_RETURN(HAL_UART_Transmit(huart, (uint8_t*)uart_buffer, stream.bytes_written, UART_DELAY_TIMEOUT));
}

void Proto_SendIMUData(imu_data_t* imu_data) {
    telemetry_packet_t telem_packet = telemetry_packet_t_init_zero;
    telem_packet.packet.imu_data = *imu_data;
    telem_packet.which_packet = telemetry_packet_t_imu_data_tag;

    Proto_SendTelemetryPacket(&telem_packet);
}