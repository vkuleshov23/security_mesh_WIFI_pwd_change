#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "painlessMesh.h"
#include "settings/WiFiConfigurer.hpp"
#include "utils/Restarter.hpp"

#ifdef ESP8266
    #include "Hash.h"
    #include "FS.h"
    #include <ESPAsyncTCP.h>
#else
    #include "SPIFFS.h"
    #include <AsyncTCP.h>
#endif

class SecMesh {
protected:
    painlessMesh mesh;
    AsyncWebServer* server;
    WiFiConfigurer wifi_conf;
    Restarter restarter;

public:
    SecMesh(AsyncWebServer* server) {
        this->server = server;
    }

    void setup() {
        if(!LittleFS.begin()) {
            Serial.println("Failed to mount LittleFS! WiFi settings not read");
        } else {
            wifi_conf.readMeshWiFi();
        }
        this->mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION | DEBUG);
        this->mesh.init(mesh_ssid, mesh_password, MESH_PORT, WIFI_AP_STA, 1);
        this->mesh.stationManual(STATION_SSID, STATION_PASSWORD);
        this->mesh.setHostname(HOSTNAME);
        this->mesh.setRoot(true);
        this->mesh.setContainsRoot(true);
    }
};