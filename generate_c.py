import protocol



import inspect
from dataclasses import is_dataclass
from enum import Enum


enums:list[Enum] = []
message_classes:list[object] = []


protocol.MESSAGE_TYPE_REGISTRY

for name, cls in inspect.getmembers(protocol, inspect.isclass):
    if cls.__module__ != protocol.__name__:
        continue

    if cls is protocol.MessageBase:
        continue

    if (
        is_dataclass(cls)
        and issubclass(cls, protocol.MessageBase)
    ):
        message_classes.append(cls)

    elif issubclass(cls, Enum):
        enums.append(cls)

generated_c = ""

# This will loop through the enums and generate the C equivalent enums
for enum in enums:
    fields = enum._member_map_
    _generated_enum = f"typedef enum {{\n"

    for field in fields:
        _generated_enum += f"    {field},\n"

    _generated_enum += f"}} {enum.__name__};\n"

    generated_c += _generated_enum







print(generated_c)



