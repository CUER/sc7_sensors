import serial

import imu_pb2


with serial.Serial('COM7', 115200, inter_byte_timeout=0.1) as serial_port:
    while True:
        no_bytes = int.from_bytes(serial_port.read(4), "little")
        print(f"Waiting for {no_bytes} bytes")
        serial_bytes = serial_port.read(no_bytes)

        imu_data = imu_pb2.imu_data_t()
        imu_data.ParseFromString(serial_bytes)
        print(f"Status: {imu_data.status}, accel data points {len(imu_data.accel_data)}, gyro data points {len(imu_data.gyro_data)}")
