#pragma once
#include <Arduino.h>
#include <set>
#include <ESPAsyncWebServer.h>
#include "painlessMesh.h"
#include "settings/WiFiConfigurer.hpp"
#include "utils/Restarter.hpp"
#include "MeshServer.hpp"
#include "MeshHandler.hpp"
#include "SecMesh.hpp"

#ifdef ESP8266
    #include "Hash.h"
    #include "FS.h"
    #include <ESPAsyncTCP.h>
#else
    #include "SPIFFS.h"
    #include <AsyncTCP.h>
#endif

class MainSecMesh : public SecMesh {
protected:
    MeshServer *mesh_server;
public:

    MainSecMesh(AsyncWebServer* server) : SecMesh() {
        this->mesh_server = new MeshServer(&this->mesh, server, &this->wifi_conf, &this->restarter, &this->authHandler);

    }
    
    void setup() {
        if(!SPIFFS.begin()) {
            Serial.println("Failed to mount LittleFS! WiFi settings not read");
        } else {
            Serial.println("LittleFS was mounted!");
            wifi_conf.readMeshWiFi();
        }
        this->mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION | DEBUG);
        this->mesh.init(mesh_ssid, mesh_password, MESH_PORT, WIFI_AP_STA, 1);
        this->mesh.stationManual(STATION_SSID, STATION_PASSWORD);
        this->mesh.setHostname(HOSTNAME);
        this->mesh.setContainsRoot(false);
        this->mesh_handler->setup();
        this->mesh_server->setup();
        Serial.printf("MY NODE ID -> %zu\n", this->mesh.getNodeId());
    }

    ~MainSecMesh() {
        delete mesh_server;
    }
};