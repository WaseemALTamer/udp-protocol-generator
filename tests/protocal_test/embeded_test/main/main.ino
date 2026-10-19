#include <Arduino.h>




#include "network_wifi.h"
#include "protocol.h"
#include "config.h"


#include "protocol_event_handler.h"



namespace EventHandlerTests {

    void on_message(
        Message content,
        IPAddress senderIP,
        uint16_t senderPort
    ) {
        Serial.println("[TEST] Message received");
        Serial.print("Sender IP: ");
        Serial.println(senderIP);
        Serial.print("Sender port: ");
        Serial.println(senderPort);
    }

    void on_command(
        Command content,
        IPAddress senderIP,
        uint16_t senderPort
    ) {
        Serial.println("[TEST] Command received");
        Serial.print("Sender IP: ");
        Serial.println(senderIP);
        Serial.print("Sender port: ");
        Serial.println(senderPort);
    }

    void on_discovery(
        Discovery content,
        IPAddress senderIP,
        uint16_t senderPort
    ) {
        Serial.println("[TEST] Discovery received");
        Serial.print("Sender IP: ");
        Serial.println(senderIP);
        Serial.print("Sender port: ");
        Serial.println(senderPort);
    }

    void on_information(
        Information content,
        IPAddress senderIP,
        uint16_t senderPort
    ) {
        Serial.println("[TEST] Information received");
    }

    void on_wifi_connect(
        WifiConnect content,
        IPAddress senderIP,
        uint16_t senderPort
    ) {
        Serial.println("[TEST] WiFiConnect received");
    }

    void on_set_device_name(
        SetDeviceName content,
        IPAddress senderIP,
        uint16_t senderPort
    ) {
        Serial.println("[TEST] SetDeviceName received");
    }

    void on_set_device_status(
        SetDeviceStatus content,
        IPAddress senderIP,
        uint16_t senderPort
    ) {
        Serial.println("[TEST] SetDeviceStatus received");
    }

    void on_set_date_time(
        SetDateTime content,
        IPAddress senderIP,
        uint16_t senderPort
    ) {
        Serial.println("[TEST] SetDateTime received");
    }

    void on_bytes_stream(
        BytesStream content,
        IPAddress senderIP,
        uint16_t senderPort
    ) {
        Serial.println("[TEST] BytesStream received");
    }

}



void register_protocol_event_tests() {
    using namespace ProtocalEventHandler;
    using namespace EventHandlerTests;

    Serial.println("[TEST] Registering protocol callbacks");

    Serial.println(register_message_callback(on_message)
        ? "[OK] Message callback registered"
        : "[ERROR] Message callback registration failed");

    Serial.println(register_command_callback(on_command)
        ? "[OK] Command callback registered"
        : "[ERROR] Command callback registration failed");

    Serial.println(register_discovery_callback(on_discovery)
        ? "[OK] Discovery callback registered"
        : "[ERROR] Discovery callback registration failed");

    Serial.println(register_information_callback(on_information)
        ? "[OK] Information callback registered"
        : "[ERROR] Information callback registration failed");

    Serial.println(register_wifi_connect_callback(on_wifi_connect)
        ? "[OK] WiFiConnect callback registered"
        : "[ERROR] WiFiConnect callback registration failed");

    Serial.println(register_set_device_name_callback(on_set_device_name)
        ? "[OK] SetDeviceName callback registered"
        : "[ERROR] SetDeviceName callback registration failed");

    Serial.println(register_set_device_status_callback(on_set_device_status)
        ? "[OK] SetDeviceStatus callback registered"
        : "[ERROR] SetDeviceStatus callback registration failed");

    Serial.println(register_set_date_time_callback(on_set_date_time)
        ? "[OK] SetDateTime callback registered"
        : "[ERROR] SetDateTime callback registration failed");

    Serial.println(register_bytes_stream_callback(on_bytes_stream)
        ? "[OK] BytesStream callback registered"
        : "[ERROR] BytesStream callback registration failed");
}






void setup() {

    Serial.begin(9600);

    // the order of the intiated systems are intentional
    Config::init();


    NetworkWifi::init();

    register_protocol_event_tests();
    
    
}


void loop() {

    // the order of updating the systems are intentional
    NetworkWifi::loop();


}