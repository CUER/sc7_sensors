import nanopb_pb2 as _nanopb_pb2
from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor
HAL_BUSY: HAL_status_e
HAL_ERROR: HAL_status_e
HAL_OK: HAL_status_e
HAL_TIMEOUT: HAL_status_e

class accel_data_t(_message.Message):
    __slots__ = ["x", "y", "z"]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    Z_FIELD_NUMBER: _ClassVar[int]
    x: float
    y: float
    z: float
    def __init__(self, x: _Optional[float] = ..., y: _Optional[float] = ..., z: _Optional[float] = ...) -> None: ...

class gyro_data_t(_message.Message):
    __slots__ = ["x", "y", "z"]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    Z_FIELD_NUMBER: _ClassVar[int]
    x: float
    y: float
    z: float
    def __init__(self, x: _Optional[float] = ..., y: _Optional[float] = ..., z: _Optional[float] = ...) -> None: ...

class imu_data_t(_message.Message):
    __slots__ = ["accel_data", "gyro_data", "status"]
    ACCEL_DATA_FIELD_NUMBER: _ClassVar[int]
    GYRO_DATA_FIELD_NUMBER: _ClassVar[int]
    STATUS_FIELD_NUMBER: _ClassVar[int]
    accel_data: _containers.RepeatedCompositeFieldContainer[accel_data_t]
    gyro_data: _containers.RepeatedCompositeFieldContainer[gyro_data_t]
    status: HAL_status_e
    def __init__(self, status: _Optional[_Union[HAL_status_e, str]] = ..., accel_data: _Optional[_Iterable[_Union[accel_data_t, _Mapping]]] = ..., gyro_data: _Optional[_Iterable[_Union[gyro_data_t, _Mapping]]] = ...) -> None: ...

class HAL_status_e(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = []
