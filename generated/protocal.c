#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

typedef enum ContentType{
    NONE,
    MESSAGE,
    COMMAND,
    INFORMATION,
    DISCOVERY,
    ONLINE,
    WIFI_CONNECT,
} ContentType;

typedef struct MessageBase MessageBase;

typedef size_t (*EncodeFunction)(
    void *self,
    uint8_t *buffer,
    size_t buffer_size
);

typedef int (*DecodeFunction)(
    void *self,
    const uint8_t *buffer,
    size_t buffer_size
);

struct MessageBase{
    EncodeFunction encode;
    DecodeFunction decode;
};




typedef struct{
    MessageBase base;

    // content start here
    uint8_t version;
    char device_name[64];
    uint8_t mac_address[6];
    double time_stamp;
    uint8_t is_encrypted;
    ContentType content_type;
    void *content;
} Message;

size_t message_encode(
    void *self,
    uint8_t *buffer,
    size_t buffer_size
)
{
    // Convert self back to your Message instance
    typeof(Message) *message = self;

    size_t offset = 0;

    if (buffer_size < 81)
        return 0;

    buffer[offset++] = message->version;

    memset(&buffer[offset], 0, 64);
    strncpy(
        (char *)&buffer[offset],
        message->device_name,
        63
    );
    offset += 64;

    memcpy(&buffer[offset], message->mac_address, 6);
    offset += 6;

    memcpy(&buffer[offset], &message->time_stamp, sizeof(double));
    offset += sizeof(double);

    buffer[offset++] = message->is_encrypted;
    buffer[offset++] = message->content_type;

    return offset;
}


Message message = {
    .base = {
        .encode = message_encode
    },

    .version = 1,
    .device_name = "",
    .mac_address = {0},
    .time_stamp = 0,
    .is_encrypted = 0,
    .content_type = NONE,
    .content = NULL
};




int main(void)
{
    strcpy(message.device_name, "hello there");

    uint8_t buffer[81];

    size_t length = message.base.encode(
        &message.base,
        buffer,
        sizeof(buffer)
    );

    printf("Encoded %zu bytes:\n", length);

    for (size_t i = 0; i < length; i++) {
        printf("%02X ", buffer[i]);
    }

    printf("\n");

    return 0;
}

