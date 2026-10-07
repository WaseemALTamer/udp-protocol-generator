/*
    THIS CODE IS GENERATED. DO NOT CHANGE.
    IF YOU WANT TO CHANGE IT, MODIFY THE PYTHON PROTOCOL
    AND REGENERATE THIS CODE.
*/

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stddef.h>

typedef enum ContentType {
    CONTENT_TYPE_NONE = 0,
    CONTENT_TYPE_MESSAGE = 1,
    CONTENT_TYPE_COMMAND = 2,
    CONTENT_TYPE_INFORMATION = 3,
    CONTENT_TYPE_DISCOVERY = 4,
    CONTENT_TYPE_ONLINE = 5,
    CONTENT_TYPE_WIFI_CONNECT = 6,
} ContentType;


typedef struct{
    uint64_t sequence_number;
    ContentType content_type;
    void *content;
} Command;

uint8_t *Command_encode(
    const Command *msg,
    uint8_t *buffer,
    size_t *buffer_size
);

const uint8_t *Command_decode(
    Command *msg,
    const uint8_t *buffer,
    size_t *buffer_size
);

typedef struct{
    uint8_t version;
    char device_name[64];
    uint8_t mac_address[6];
    double time_stamp;
    uint8_t is_encrypted;
    ContentType content_type;
    void *content;
} Message;

uint8_t *Message_encode(
    const Message *msg,
    uint8_t *buffer,
    size_t *buffer_size
);

const uint8_t *Message_decode(
    Message *msg,
    const uint8_t *buffer,
    size_t *buffer_size
);

typedef struct{
    char ssid[32];
    char password[64];
} WifiConnect;

uint8_t *Wifi_Connect_encode(
    const WifiConnect *msg,
    uint8_t *buffer,
    size_t *buffer_size
);

const uint8_t *Wifi_Connect_decode(
    WifiConnect *msg,
    const uint8_t *buffer,
    size_t *buffer_size
);

#endif /* PROTOCOL_H */
