
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


    typedef void (*BytesStreamCallback)(BytesStream content, IPAddress senderIP, uint16_t senderPort);
    extern BytesStreamCallback bytes_stream_callbacks[MAX_NETWORK_CALLBACKS];
    extern int bytes_stream_callback_count;
    bool register_bytes_stream_callback(BytesStreamCallback callback);

    typedef void (*CommandCallback)(Command content, IPAddress senderIP, uint16_t senderPort);
    extern CommandCallback command_callbacks[MAX_NETWORK_CALLBACKS];
    extern int command_callback_count;
    bool register_command_callback(CommandCallback callback);

    typedef void (*DiscoveryCallback)(Discovery content, IPAddress senderIP, uint16_t senderPort);
    extern DiscoveryCallback discovery_callbacks[MAX_NETWORK_CALLBACKS];
    extern int discovery_callback_count;
    bool register_discovery_callback(DiscoveryCallback callback);

    typedef void (*InformationCallback)(Information content, IPAddress senderIP, uint16_t senderPort);
    extern InformationCallback information_callbacks[MAX_NETWORK_CALLBACKS];
    extern int information_callback_count;
    bool register_information_callback(InformationCallback callback);

    typedef void (*MessageCallback)(Message content, IPAddress senderIP, uint16_t senderPort);
    extern MessageCallback message_callbacks[MAX_NETWORK_CALLBACKS];
    extern int message_callback_count;
    bool register_message_callback(MessageCallback callback);

    typedef void (*SetDateTimeCallback)(SetDateTime content, IPAddress senderIP, uint16_t senderPort);
    extern SetDateTimeCallback set_date_time_callbacks[MAX_NETWORK_CALLBACKS];
    extern int set_date_time_callback_count;
    bool register_set_date_time_callback(SetDateTimeCallback callback);

    typedef void (*SetDeviceNameCallback)(SetDeviceName content, IPAddress senderIP, uint16_t senderPort);
    extern SetDeviceNameCallback set_device_name_callbacks[MAX_NETWORK_CALLBACKS];
    extern int set_device_name_callback_count;
    bool register_set_device_name_callback(SetDeviceNameCallback callback);

    typedef void (*SetDeviceStatusCallback)(SetDeviceStatus content, IPAddress senderIP, uint16_t senderPort);
    extern SetDeviceStatusCallback set_device_status_callbacks[MAX_NETWORK_CALLBACKS];
    extern int set_device_status_callback_count;
    bool register_set_device_status_callback(SetDeviceStatusCallback callback);

    typedef void (*WifiConnectCallback)(WifiConnect content, IPAddress senderIP, uint16_t senderPort);
    extern WifiConnectCallback wifi_connect_callbacks[MAX_NETWORK_CALLBACKS];
    extern int wifi_connect_callback_count;
    bool register_wifi_connect_callback(WifiConnectCallback callback);


    void handle_message(
        const uint8_t* buffer,
        size_t len,
        IPAddress senderIP,
        uint16_t senderPort
    );

}
