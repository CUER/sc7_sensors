import serial

import lsm6ds_pb2


with serial.Serial('/dev/ttyACM0', 115200) as serial_port:
    while True:
        no_bytes = int.from_bytes(serial_port.read(4), "little")
        print(f"Waiting for {no_bytes} bytes")
        serial_bytes = serial_port.read(no_bytes)
        print(f"Received {len(serial_bytes)} bytes: {serial_bytes}")
        
        temp = lsm6ds_pb2.temp_data()
        temp.ParseFromString(serial_bytes)
        print(temp)
