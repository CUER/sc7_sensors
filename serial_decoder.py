import serial
import struct

with serial.Serial('COM3', 115200) as serial_port:
    while True:
        float_bytes = serial_port.read(4)
        actual_float = struct.unpack("<f", float_bytes)
        print(actual_float)
