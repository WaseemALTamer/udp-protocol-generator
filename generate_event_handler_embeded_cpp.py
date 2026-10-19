from pathlib import Path
BASE_DIR = Path(__file__).resolve().parent




import protocol



from dataclasses import is_dataclass, fields, MISSING
from enum import Enum
import inspect
import shutil
import os



enums:list[Enum] = []
message_classes:list[object] = []



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



def to_snake(name):
    out = ""
    for i, ch in enumerate(name):
        if ch.isupper() and i:
            out += "_"
        out += ch
    return out




def generate_release_code(protocol_classes):
    lines = []

    for cls in protocol_classes:
        name = cls.__name__
        snake_name = to_snake(name).lower()

        lines.extend([
            f"    typedef void (*{name}Callback)({name} content, IPAddress senderIP, uint16_t senderPort);",
            f"    extern {name}Callback {snake_name}_callbacks[MAX_NETWORK_CALLBACKS];",
            f"    extern int {snake_name}_callback_count;",
            f"    bool register_{snake_name}_callback({name}Callback callback);",
            "",
        ])

    return "\n".join(lines).rstrip()


generated_h = """
/*
    THIS CODE IS GENEARTED DO NOT CHANGE, IF YOU WANT TO CHANGE LOOK THROUGH THE
    PYTHON MIRROR OF THE PROTOCAL AND REGENERATE THE CODE WITH THE GENERATOR
*/


#pragma once
#include <Arduino.h>
#include "protocol.h"


#define MAX_NETWORK_CALLBACKS 5

namespace ProtocalEventHandler
{


{GENERATED_CODE}


    void handle_message(
        const uint8_t* buffer,
        size_t len,
        IPAddress senderIP,
        uint16_t senderPort
    );

}
"""





generated_c = """
/*
    THIS CODE IS GENEARTED DO NOT CHANGE, IF YOU WANT TO CHANGE LOOK THROUGH THE
    PYTHON MIRROR OF THE PROTOCAL AND REGENERATE THE CODE WITH THE GENERATOR
*/


#include <Arduino.h>
#include "protocol.h"
#include "protocol_event_handler.h"




namespace ProtocalEventHandler
{
    // this namespace will proccess each message at a time 
    // this namespace will not  work if  you are running a
    // RTOS 


    // this file will work in the following way:
    
    
    // meessage will be  taken and  dispatch_content is excauted
    // where content is message, then dispatch_content will call
    // it self  recursivelly as long  as the messages them selfs
    // contain a content_type, meaning that we recursively after
    // each message decode we trigger a event for the message


    // current flow handel_message -> (dispatch_content -> event) -> (dispatch_content -> event) -> ... -> (dispatch_content -> event)
    // the flow will keep decoding the message until the message ends with content that doesnt hold greedy bytes

    
{GENERATED_CODE_1}


    bool dispatch_content(
        ContentType type,
        const uint8_t* buffer,
        size_t len,
        IPAddress senderIP,
        uint16_t senderPort,
        ContentType* next_type,
        const uint8_t** next_buffer,
        size_t* next_len
    ) {
        if (buffer == nullptr ||
            next_type == nullptr ||
            next_buffer == nullptr ||
            next_len == nullptr) {
            return false;
        }

        size_t remaining = len;
        const uint8_t* payload = nullptr;

        *next_buffer = nullptr;
        *next_len = 0;
        *next_type = CONTENT_TYPE_NONE;

        switch (type) {
 
{GENERATED_CODE_2}

            default:
                Serial.println("Unknown content type");
                return false;
        }
    }




    void handle_message(
        const uint8_t* buffer,
        size_t len,
        IPAddress senderIP,
        uint16_t senderPort
    ) {
        ContentType type = CONTENT_TYPE_MESSAGE;
        const uint8_t* current_buffer = buffer;
        size_t current_len = len;

        while (current_buffer != nullptr && current_len > 0) {
            ContentType next_type = CONTENT_TYPE_NONE;
            const uint8_t* next_buffer = nullptr;
            size_t next_len = 0;

            if (!dispatch_content(
                    type,
                    current_buffer,
                    current_len,
                    senderIP,
                    senderPort,
                    &next_type,
                    &next_buffer,
                    &next_len)) {
                break;
            }

            // Terminal content types have no next layer.
            if (next_buffer == nullptr || next_len == 0) {
                break;
            }

            type = next_type;
            current_buffer = next_buffer;
            current_len = next_len;
        }
    }
}
"""


def generate_event_handler_vars(protocol_classes):
    lines = []

    for cls in protocol_classes:
        name = cls.__name__
        snake_name = to_snake(name).lower()

        lines.extend([
            f"    {name} {snake_name}{{}};",
            f"    {name}Callback {snake_name}_callbacks[MAX_NETWORK_CALLBACKS] = {{}};",
            f"    int {snake_name}_callback_count = 0;",
            "",
            "    // registration function",
            f"    bool register_{snake_name}_callback({name}Callback callback) {{",
            "        if (callback == nullptr ||",
            f"            {snake_name}_callback_count >= MAX_NETWORK_CALLBACKS) return false;",
            "",
            f"        {snake_name}_callbacks[{snake_name}_callback_count++] = callback;",
            "        return true;",
            "    }",
            "",
            "    // trigger function",
            f"    void trigger_{snake_name}_callbacks({name} content, IPAddress senderIP, uint16_t senderPort) {{",
            f"        for (int i = 0; i < {snake_name}_callback_count; i++)",
            f"            {snake_name}_callbacks[i](content, senderIP, senderPort);",
            "    }",
            "",
        ])

    return "\n".join(lines).rstrip()



def generate_dispatch_cases(message_classes, message_type_registry):
    lines = []

    for content_type, cls in message_type_registry.items():
        name = cls.__name__

        # Only generate cases for classes in message_classes.
        if cls not in message_classes:
            continue

        snake_name = to_snake(name).lower()
        has_nested_content = (
            "content_type" in cls.__annotations__
            and "content" in cls.__annotations__
        )

        lines.extend([
            f"            case CONTENT_TYPE_{content_type.name}: {{",
            f"                payload = {snake_name}_decode(&{snake_name}, buffer, &remaining);",
            "",
            "                if (payload == nullptr) {",
            "                    return false;",
            "                }",
            "",
            f"                trigger_{snake_name}_callbacks({snake_name}, senderIP, senderPort);",
        ])

        if has_nested_content:
            lines.extend([
                "",
                f"                *next_type = {snake_name}.content_type;",
                "                *next_buffer = payload;",
                "                *next_len = remaining;",
            ])

        lines.extend([
            "",
            "                return true;",
            "            }",
            "",
        ])

    return "\n".join(lines).rstrip()



generated_h = generated_h.replace(
    "{GENERATED_CODE}",
    generate_release_code(message_classes)
)


generated_c = generated_c.replace(
    "{GENERATED_CODE_1}",
    generate_event_handler_vars(message_classes)
)


generated_c = generated_c.replace(
    "{GENERATED_CODE_2}",
    generate_dispatch_cases(
        message_classes,
        protocol.MESSAGE_TYPE_REGISTRY
    )
)



if __name__ == "__main__":

    folder = "generated"
    os.makedirs(folder, exist_ok=True)

    header_file_path = os.path.join(folder, "protocol_event_handler.h")
    implementation_file_path = os.path.join(folder, "protocol_event_handler.c")


    with open(header_file_path, "w") as f:
        f.write(generated_h)

    with open(implementation_file_path, "w") as f:
        f.write(generated_c)