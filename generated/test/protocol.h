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
    CONTENT_TYPE_BYTES_STREAM = 1,
    CONTENT_TYPE_MESSAGE = 2,
    CONTENT_TYPE_COMMAND = 3,
    CONTENT_TYPE_INFORMATION = 4,
    CONTENT_TYPE_DISCOVERY = 5,
    CONTENT_TYPE_WIFI_CONNECT = 6,
    CONTENT_TYPE_SET_DEVICE_STATUS = 7,
    CONTENT_TYPE_SET_DEVICE_NAME = 8,
    CONTENT_TYPE_SET_DATE_TIME = 9,
} ContentType;


typedef enum DeviceStatus {
    DEVICE_STATUS_NONE = 0,
    DEVICE_STATUS_BOOTING = 1,
    DEVICE_STATUS_NORMAL = 2,
    DEVICE_STATUS_NETWORK_ERROR = 3,
    DEVICE_STATUS_UNKNOWN_ERROR = 4,
} DeviceStatus;


typedef struct{
    uint32_t length;
    const uint8_t *content;
} BytesStream;

typedef struct{
    uint64_t sequence_number;
    ContentType content_type;
    const uint8_t *content;
} Command;

typedef struct{
} Discovery;

typedef struct{
} Information;

typedef struct{
    char device_name[64];
    uint8_t mac_address[6];
    double time_stamp;
    uint8_t is_encrypted;
    ContentType content_type;
    const uint8_t *content;
} Message;

typedef struct{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t month;
    uint32_t year;
} SetDateTime;

typedef struct{
    char device_name[64];
} SetDeviceName;

typedef struct{
    DeviceStatus status;
} SetDeviceStatus;

typedef struct{
    char ssid[32];
    char password[64];
} WifiConnect;


#ifdef __cplusplus
extern "C" {
#endif

uint8_t *bytes_stream_encode(
    const BytesStream *msg,
    uint8_t *buffer,
    size_t *buffer_size
);

const uint8_t *bytes_stream_decode(
    BytesStream *msg,
    const uint8_t *buffer,
    size_t *buffer_size
);

uint8_t *command_encode(
    const Command *msg,
    uint8_t *buffer,
    size_t *buffer_size
);

const uint8_t *command_decode(
    Command *msg,
    const uint8_t *buffer,
    size_t *buffer_size
);

uint8_t *discovery_encode(
    const Discovery *msg,
    uint8_t *buffer,
    size_t *buffer_size
);

const uint8_t *discovery_decode(
    Discovery *msg,
    const uint8_t *buffer,
    size_t *buffer_size
);

uint8_t *information_encode(
    const Information *msg,
    uint8_t *buffer,
    size_t *buffer_size
);

const uint8_t *information_decode(
    Information *msg,
    const uint8_t *buffer,
    size_t *buffer_size
);

uint8_t *message_encode(
    const Message *msg,
    uint8_t *buffer,
    size_t *buffer_size
);

const uint8_t *message_decode(
    Message *msg,
    const uint8_t *buffer,
    size_t *buffer_size
);

uint8_t *set_date_time_encode(
    const SetDateTime *msg,
    uint8_t *buffer,
    size_t *buffer_size
);

const uint8_t *set_date_time_decode(
    SetDateTime *msg,
    const uint8_t *buffer,
    size_t *buffer_size
);

uint8_t *set_device_name_encode(
    const SetDeviceName *msg,
    uint8_t *buffer,
    size_t *buffer_size
);

const uint8_t *set_device_name_decode(
    SetDeviceName *msg,
    const uint8_t *buffer,
    size_t *buffer_size
);

uint8_t *set_device_status_encode(
    const SetDeviceStatus *msg,
    uint8_t *buffer,
    size_t *buffer_size
);

const uint8_t *set_device_status_decode(
    SetDeviceStatus *msg,
    const uint8_t *buffer,
    size_t *buffer_size
);

uint8_t *wifi_connect_encode(
    const WifiConnect *msg,
    uint8_t *buffer,
    size_t *buffer_size
);

const uint8_t *wifi_connect_decode(
    WifiConnect *msg,
    const uint8_t *buffer,
    size_t *buffer_size
);


#ifdef __cplusplus
}
#endif

#endif /* PROTOCOL_H */
