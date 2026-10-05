import inspect
import protocol

for name, obj in inspect.getmembers(protocol, inspect.isclass):
    print(name)


import inspect
from dataclasses import is_dataclass

classes = []

for name, cls in inspect.getmembers(protocol, inspect.isclass):
    if (
        cls.__module__ == protocol.__name__
        and is_dataclass(cls)
        and issubclass(cls, protocol.MessageBase)
        and cls is not protocol.MessageBase
    ):
        classes.append(cls)


print(classes)