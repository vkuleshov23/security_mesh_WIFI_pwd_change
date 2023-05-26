#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "painlessMesh.h"
#include "settings/WiFiConfigurer.hpp"
#include "utils/Restarter.hpp"
#include "MeshServer.hpp"
#include "MeshHandler.hpp"

#ifdef ESP8266
    #include "Hash.h"
    #include "FS.h"
    #include <ESPAsyncTCP.h>
#else
    #include "SPIFFS.h"
    #include <AsyncTCP.h>
#endif

class SecMesh {
private:
    Scheduler scheduler;
protected:
    painlessMesh *mesh;
    WiFiConfigurer wifi_conf;
    Restarter restarter;
    MeshHandler *mesh_handler;

public:
    SecMesh(painlessMesh* mesh) {
        this->mesh = mesh;
        this->mesh_handler = new MeshHandler(mesh);
    }

    void setup() {
        if(!SPIFFS.begin()) {
            Serial.println("Failed to mount LittleFS! WiFi settings not read");
        } else {
            Serial.println("LittleFS was mounted!");
            wifi_conf.readMeshWiFi();
        }
        this->mesh->setDebugMsgTypes(ERROR | STARTUP | CONNECTION | DEBUG);
        this->mesh->init(mesh_ssid, mesh_password, &scheduler, MESH_PORT);
        this->mesh->setContainsRoot(false);
        this->mesh_handler->setup();
    }

    void update() {
        this->mesh->update();
        this->mesh_handler->update();
    };

    void send(shared_ptr<IMeshCommand> command) {
        this->mesh_handler->send(command);
    }

    Restarter getRestarter(){return this->restarter;}

    void setRestarter(Restarter restarter){this->restarter = restarter;}

    MeshHandler *getMesh_handler(){return this->mesh_handler;}

    void setMesh_handler(MeshHandler *mesh_handler){this->mesh_handler = mesh_handler;}

    ~SecMesh() {
        delete mesh_handler;
    }
};