import nanopb_pb2 as _nanopb_pb2
from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class HAL_status_e(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    HAL_OK: _ClassVar[HAL_status_e]
    HAL_ERROR: _ClassVar[HAL_status_e]
    HAL_BUSY: _ClassVar[HAL_status_e]
    HAL_TIMEOUT: _ClassVar[HAL_status_e]
HAL_OK: HAL_status_e
HAL_ERROR: HAL_status_e
HAL_BUSY: HAL_status_e
HAL_TIMEOUT: HAL_status_e

class error_t(_message.Message):
    __slots__ = ("err_msg",)
    ERR_MSG_FIELD_NUMBER: _ClassVar[int]
    err_msg: str
    def __init__(self, err_msg: _Optional[str] = ...) -> None: ...

class accel_data_t(_message.Message):
    __slots__ = ("x", "y", "z")
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    Z_FIELD_NUMBER: _ClassVar[int]
    x: float
    y: float
    z: float
    def __init__(self, x: _Optional[float] = ..., y: _Optional[float] = ..., z: _Optional[float] = ...) -> None: ...

class gyro_data_t(_message.Message):
    __slots__ = ("x", "y", "z")
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    Z_FIELD_NUMBER: _ClassVar[int]
    x: float
    y: float
    z: float
    def __init__(self, x: _Optional[float] = ..., y: _Optional[float] = ..., z: _Optional[float] = ...) -> None: ...

class imu_data_t(_message.Message):
    __slots__ = ("status", "accel_data", "gyro_data")
    STATUS_FIELD_NUMBER: _ClassVar[int]
    ACCEL_DATA_FIELD_NUMBER: _ClassVar[int]
    GYRO_DATA_FIELD_NUMBER: _ClassVar[int]
    status: HAL_status_e
    accel_data: _containers.RepeatedCompositeFieldContainer[accel_data_t]
    gyro_data: _containers.RepeatedCompositeFieldContainer[gyro_data_t]
    def __init__(self, status: _Optional[_Union[HAL_status_e, str]] = ..., accel_data: _Optional[_Iterable[_Union[accel_data_t, _Mapping]]] = ..., gyro_data: _Optional[_Iterable[_Union[gyro_data_t, _Mapping]]] = ...) -> None: ...

class telemetry_packet_t(_message.Message):
    __slots__ = ("error", "imu_data")
    ERROR_FIELD_NUMBER: _ClassVar[int]
    IMU_DATA_FIELD_NUMBER: _ClassVar[int]
    error: error_t
    imu_data: imu_data_t
    def __init__(self, error: _Optional[_Union[error_t, _Mapping]] = ..., imu_data: _Optional[_Union[imu_data_t, _Mapping]] = ...) -> None: ...
