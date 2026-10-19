#include <Preferences.h>
#include <Arduino.h>

#include "Config.h"
#include "protocol_event_handler.h"



// unless you establish a strong connection saving and data will feel jankey
// certain config edits will require reboot for example changing wifi

namespace Config{

    Preferences preferences;

    char name[64];


    // network config
    char network_ssid[32];
    char network_password[64];


    char hotspot_ssid[32];
    char hotspot_password[64];


    int network_port = 39394;




    void save() {

        if (!preferences.begin("config", false)) {
            Serial.println("Failed to open configuration storage.");
            return;
        }

        // clear existing values before writing the new ones
        preferences.remove("name");
        preferences.remove("net_ssid");
        preferences.remove("net_password");
        preferences.remove("ap_ssid");
        preferences.remove("ap_password");
        preferences.remove("ap_port");

        // write the current configuration.
        preferences.putString("name", name);
        preferences.putString("net_ssid", network_ssid);
        preferences.putString("net_password", network_password);
        preferences.putString("ap_ssid", hotspot_ssid);
        preferences.putString("ap_password", hotspot_password);
        preferences.putInt("net_port", network_port);

        preferences.end();

        Serial.println("Configuration saved.");
    }


    bool load() {

        if (!preferences.begin("config", true)) {
            return false;
        }

        // Check whether a saved configuration exists.
        if (!preferences.isKey("name")) {
            preferences.end();
            return false;
        }


        preferences.getString("name", name, sizeof(name));

        preferences.getString(
            "net_ssid", network_ssid, sizeof(network_ssid));

        preferences.getString(
            "net_password", network_password, sizeof(network_password));

        preferences.getString(
            "ap_ssid", hotspot_ssid, sizeof(hotspot_ssid));

        preferences.getString(
            "ap_password", hotspot_password, sizeof(hotspot_password));

        network_port = preferences.getInt("net_port", 39394); // hard coded defult value change that please

        preferences.end();

        return true;
    }




    void init() {
        // Default values used only if nothing has been saved.
        strncpy(name, "NODE-1", sizeof(name));
        name[sizeof(name) - 1] = '\0';

        network_ssid[0] = '\0';
        network_password[0] = '\0';

        hotspot_ssid[0] = '\0';
        hotspot_password[0] = '\0';

        network_port = 39394;


        strcpy(network_ssid, "Waseem");
        strcpy(network_password, "Waseem16102005");

        strcpy(hotspot_ssid, "ESP32-DEVELOPMENT");
        strcpy(hotspot_password, "");


        /*
        
        if (load()) {
            Serial.println("Configuration loaded.");
        } else {
            Serial.println("No saved configuration found; using defaults.");
        }
        
        */



        //Serial.printf("Device name: %s\n", name);
        //Serial.printf("WiFi SSID: '%s'\n", network_ssid);
        //Serial.printf("WiFi Password: '%s'\n", network_password);
        

    }




    void loop(){

    }

}



