# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: imu.proto
# Protobuf Python Version: 4.25.1
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


import nanopb_pb2 as nanopb__pb2


DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\timu.proto\x1a\x0cnanopb.proto\"\"\n\x07\x65rror_t\x12\x17\n\x07\x65rr_msg\x18\x01 \x01(\tB\x06\x92?\x03\x08\xc8\x01\"/\n\x0c\x61\x63\x63\x65l_data_t\x12\t\n\x01x\x18\x01 \x01(\x02\x12\t\n\x01y\x18\x02 \x01(\x02\x12\t\n\x01z\x18\x03 \x01(\x02\".\n\x0bgyro_data_t\x12\t\n\x01x\x18\x01 \x01(\x02\x12\t\n\x01y\x18\x02 \x01(\x02\x12\t\n\x01z\x18\x03 \x01(\x02\"}\n\nimu_data_t\x12\x1d\n\x06status\x18\x01 \x01(\x0e\x32\r.HAL_status_e\x12(\n\naccel_data\x18\x02 \x03(\x0b\x32\r.accel_data_tB\x05\x92?\x02\x10\x64\x12&\n\tgyro_data\x18\x03 \x03(\x0b\x32\x0c.gyro_data_tB\x05\x92?\x02\x10\x64\"Z\n\x12telemetry_packet_t\x12\x19\n\x05\x65rror\x18\x01 \x01(\x0b\x32\x08.error_tH\x00\x12\x1f\n\x08imu_data\x18\x02 \x01(\x0b\x32\x0b.imu_data_tH\x00\x42\x08\n\x06packet*H\n\x0cHAL_status_e\x12\n\n\x06HAL_OK\x10\x00\x12\r\n\tHAL_ERROR\x10\x01\x12\x0c\n\x08HAL_BUSY\x10\x02\x12\x0f\n\x0bHAL_TIMEOUT\x10\x03\x62\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'imu_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
  DESCRIPTOR._options = None
  _globals['_ERROR_T'].fields_by_name['err_msg']._options = None
  _globals['_ERROR_T'].fields_by_name['err_msg']._serialized_options = b'\222?\003\010\310\001'
  _globals['_IMU_DATA_T'].fields_by_name['accel_data']._options = None
  _globals['_IMU_DATA_T'].fields_by_name['accel_data']._serialized_options = b'\222?\002\020d'
  _globals['_IMU_DATA_T'].fields_by_name['gyro_data']._options = None
  _globals['_IMU_DATA_T'].fields_by_name['gyro_data']._serialized_options = b'\222?\002\020d'
  _globals['_HAL_STATUS_E']._serialized_start=379
  _globals['_HAL_STATUS_E']._serialized_end=451
  _globals['_ERROR_T']._serialized_start=27
  _globals['_ERROR_T']._serialized_end=61
  _globals['_ACCEL_DATA_T']._serialized_start=63
  _globals['_ACCEL_DATA_T']._serialized_end=110
  _globals['_GYRO_DATA_T']._serialized_start=112
  _globals['_GYRO_DATA_T']._serialized_end=158
  _globals['_IMU_DATA_T']._serialized_start=160
  _globals['_IMU_DATA_T']._serialized_end=285
  _globals['_TELEMETRY_PACKET_T']._serialized_start=287
  _globals['_TELEMETRY_PACKET_T']._serialized_end=377
# @@protoc_insertion_point(module_scope)
