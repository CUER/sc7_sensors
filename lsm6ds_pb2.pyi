from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class msg_oneof(_message.Message):
    __slots__ = ["msg_temp_data"]
    MSG_TEMP_DATA_FIELD_NUMBER: _ClassVar[int]
    msg_temp_data: temp_data
    def __init__(self, msg_temp_data: _Optional[_Union[temp_data, _Mapping]] = ...) -> None: ...

class temp_data(_message.Message):
    __slots__ = ["err", "temp", "tick"]
    ERR_FIELD_NUMBER: _ClassVar[int]
    TEMP_FIELD_NUMBER: _ClassVar[int]
    TICK_FIELD_NUMBER: _ClassVar[int]
    err: int
    temp: float
    tick: int
    def __init__(self, tick: _Optional[int] = ..., temp: _Optional[float] = ..., err: _Optional[int] = ...) -> None: ...
