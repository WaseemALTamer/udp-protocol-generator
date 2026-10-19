
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

    
    BytesStream bytes_stream{};
    BytesStreamCallback bytes_stream_callbacks[MAX_NETWORK_CALLBACKS] = {};
    int bytes_stream_callback_count = 0;

    // registration function
    bool register_bytes_stream_callback(BytesStreamCallback callback) {
        if (callback == nullptr ||
            bytes_stream_callback_count >= MAX_NETWORK_CALLBACKS) return false;

        bytes_stream_callbacks[bytes_stream_callback_count++] = callback;
        return true;
    }

    // trigger function
    void trigger_bytes_stream_callbacks(BytesStream content, IPAddress senderIP, uint16_t senderPort) {
        for (int i = 0; i < bytes_stream_callback_count; i++)
            bytes_stream_callbacks[i](content, senderIP, senderPort);
    }

    Command command{};
    CommandCallback command_callbacks[MAX_NETWORK_CALLBACKS] = {};
    int command_callback_count = 0;

    // registration function
    bool register_command_callback(CommandCallback callback) {
        if (callback == nullptr ||
            command_callback_count >= MAX_NETWORK_CALLBACKS) return false;

        command_callbacks[command_callback_count++] = callback;
        return true;
    }

    // trigger function
    void trigger_command_callbacks(Command content, IPAddress senderIP, uint16_t senderPort) {
        for (int i = 0; i < command_callback_count; i++)
            command_callbacks[i](content, senderIP, senderPort);
    }

    Discovery discovery{};
    DiscoveryCallback discovery_callbacks[MAX_NETWORK_CALLBACKS] = {};
    int discovery_callback_count = 0;

    // registration function
    bool register_discovery_callback(DiscoveryCallback callback) {
        if (callback == nullptr ||
            discovery_callback_count >= MAX_NETWORK_CALLBACKS) return false;

        discovery_callbacks[discovery_callback_count++] = callback;
        return true;
    }

    // trigger function
    void trigger_discovery_callbacks(Discovery content, IPAddress senderIP, uint16_t senderPort) {
        for (int i = 0; i < discovery_callback_count; i++)
            discovery_callbacks[i](content, senderIP, senderPort);
    }

    Information information{};
    InformationCallback information_callbacks[MAX_NETWORK_CALLBACKS] = {};
    int information_callback_count = 0;

    // registration function
    bool register_information_callback(InformationCallback callback) {
        if (callback == nullptr ||
            information_callback_count >= MAX_NETWORK_CALLBACKS) return false;

        information_callbacks[information_callback_count++] = callback;
        return true;
    }

    // trigger function
    void trigger_information_callbacks(Information content, IPAddress senderIP, uint16_t senderPort) {
        for (int i = 0; i < information_callback_count; i++)
            information_callbacks[i](content, senderIP, senderPort);
    }

    Message message{};
    MessageCallback message_callbacks[MAX_NETWORK_CALLBACKS] = {};
    int message_callback_count = 0;

    // registration function
    bool register_message_callback(MessageCallback callback) {
        if (callback == nullptr ||
            message_callback_count >= MAX_NETWORK_CALLBACKS) return false;

        message_callbacks[message_callback_count++] = callback;
        return true;
    }

    // trigger function
    void trigger_message_callbacks(Message content, IPAddress senderIP, uint16_t senderPort) {
        for (int i = 0; i < message_callback_count; i++)
            message_callbacks[i](content, senderIP, senderPort);
    }

    SetDateTime set_date_time{};
    SetDateTimeCallback set_date_time_callbacks[MAX_NETWORK_CALLBACKS] = {};
    int set_date_time_callback_count = 0;

    // registration function
    bool register_set_date_time_callback(SetDateTimeCallback callback) {
        if (callback == nullptr ||
            set_date_time_callback_count >= MAX_NETWORK_CALLBACKS) return false;

        set_date_time_callbacks[set_date_time_callback_count++] = callback;
        return true;
    }

    // trigger function
    void trigger_set_date_time_callbacks(SetDateTime content, IPAddress senderIP, uint16_t senderPort) {
        for (int i = 0; i < set_date_time_callback_count; i++)
            set_date_time_callbacks[i](content, senderIP, senderPort);
    }

    SetDeviceName set_device_name{};
    SetDeviceNameCallback set_device_name_callbacks[MAX_NETWORK_CALLBACKS] = {};
    int set_device_name_callback_count = 0;

    // registration function
    bool register_set_device_name_callback(SetDeviceNameCallback callback) {
        if (callback == nullptr ||
            set_device_name_callback_count >= MAX_NETWORK_CALLBACKS) return false;

        set_device_name_callbacks[set_device_name_callback_count++] = callback;
        return true;
    }

    // trigger function
    void trigger_set_device_name_callbacks(SetDeviceName content, IPAddress senderIP, uint16_t senderPort) {
        for (int i = 0; i < set_device_name_callback_count; i++)
            set_device_name_callbacks[i](content, senderIP, senderPort);
    }

    SetDeviceStatus set_device_status{};
    SetDeviceStatusCallback set_device_status_callbacks[MAX_NETWORK_CALLBACKS] = {};
    int set_device_status_callback_count = 0;

    // registration function
    bool register_set_device_status_callback(SetDeviceStatusCallback callback) {
        if (callback == nullptr ||
            set_device_status_callback_count >= MAX_NETWORK_CALLBACKS) return false;

        set_device_status_callbacks[set_device_status_callback_count++] = callback;
        return true;
    }

    // trigger function
    void trigger_set_device_status_callbacks(SetDeviceStatus content, IPAddress senderIP, uint16_t senderPort) {
        for (int i = 0; i < set_device_status_callback_count; i++)
            set_device_status_callbacks[i](content, senderIP, senderPort);
    }

    WifiConnect wifi_connect{};
    WifiConnectCallback wifi_connect_callbacks[MAX_NETWORK_CALLBACKS] = {};
    int wifi_connect_callback_count = 0;

    // registration function
    bool register_wifi_connect_callback(WifiConnectCallback callback) {
        if (callback == nullptr ||
            wifi_connect_callback_count >= MAX_NETWORK_CALLBACKS) return false;

        wifi_connect_callbacks[wifi_connect_callback_count++] = callback;
        return true;
    }

    // trigger function
    void trigger_wifi_connect_callbacks(WifiConnect content, IPAddress senderIP, uint16_t senderPort) {
        for (int i = 0; i < wifi_connect_callback_count; i++)
            wifi_connect_callbacks[i](content, senderIP, senderPort);
    }


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
 
            case CONTENT_TYPE_MESSAGE: {
                payload = message_decode(&message, buffer, &remaining);

                if (payload == nullptr) {
                    return false;
                }

                trigger_message_callbacks(message, senderIP, senderPort);

                *next_type = message.content_type;
                *next_buffer = payload;
                *next_len = remaining;

                return true;
            }

            case CONTENT_TYPE_BYTES_STREAM: {
                payload = bytes_stream_decode(&bytes_stream, buffer, &remaining);

                if (payload == nullptr) {
                    return false;
                }

                trigger_bytes_stream_callbacks(bytes_stream, senderIP, senderPort);

                return true;
            }

            case CONTENT_TYPE_COMMAND: {
                payload = command_decode(&command, buffer, &remaining);

                if (payload == nullptr) {
                    return false;
                }

                trigger_command_callbacks(command, senderIP, senderPort);

                *next_type = command.content_type;
                *next_buffer = payload;
                *next_len = remaining;

                return true;
            }

            case CONTENT_TYPE_WIFI_CONNECT: {
                payload = wifi_connect_decode(&wifi_connect, buffer, &remaining);

                if (payload == nullptr) {
                    return false;
                }

                trigger_wifi_connect_callbacks(wifi_connect, senderIP, senderPort);

                return true;
            }

            case CONTENT_TYPE_SET_DEVICE_STATUS: {
                payload = set_device_status_decode(&set_device_status, buffer, &remaining);

                if (payload == nullptr) {
                    return false;
                }

                trigger_set_device_status_callbacks(set_device_status, senderIP, senderPort);

                return true;
            }

            case CONTENT_TYPE_INFORMATION: {
                payload = information_decode(&information, buffer, &remaining);

                if (payload == nullptr) {
                    return false;
                }

                trigger_information_callbacks(information, senderIP, senderPort);

                return true;
            }

            case CONTENT_TYPE_SET_DEVICE_NAME: {
                payload = set_device_name_decode(&set_device_name, buffer, &remaining);

                if (payload == nullptr) {
                    return false;
                }

                trigger_set_device_name_callbacks(set_device_name, senderIP, senderPort);

                return true;
            }

            case CONTENT_TYPE_DISCOVERY: {
                payload = discovery_decode(&discovery, buffer, &remaining);

                if (payload == nullptr) {
                    return false;
                }

                trigger_discovery_callbacks(discovery, senderIP, senderPort);

                return true;
            }

            case CONTENT_TYPE_SET_DATE_TIME: {
                payload = set_date_time_decode(&set_date_time, buffer, &remaining);

                if (payload == nullptr) {
                    return false;
                }

                trigger_set_date_time_callbacks(set_date_time, senderIP, senderPort);

                return true;
            }

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

        while (current_buffer != nullptr) {
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

            // Stop only when there is no next content layer.
            // A zero-length payload can still be a valid message.
            if (next_buffer == nullptr) {
                break;
            }

            type = next_type;
            current_buffer = next_buffer;
            current_len = next_len;
        }
    }
}
