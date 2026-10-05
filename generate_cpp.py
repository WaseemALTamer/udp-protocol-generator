import protocol



import inspect
from dataclasses import is_dataclass
from enum import Enum


enums = []
message_classes = []

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

print(enums)
print(message_classes)