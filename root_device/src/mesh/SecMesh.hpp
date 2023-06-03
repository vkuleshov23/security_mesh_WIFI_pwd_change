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

    function<void(const uint32_t &)> newConnectionCallback = [this](const uint32_t &from){
        if(from == MAIN_DEVICE) {
            this->mesh_handler->send(shared_ptr<IMeshCommand>(new AuthStep0Command(from)));
        }
    };

protected:
    painlessMesh mesh;
    WiFiConfigurer wifi_conf;
    Restarter restarter;
    MeshHandler *mesh_handler;
    RSAAdatper rsa;
    ECCAdatper ecc;
    AuthHandler authHandler;

public:
    SecMesh() {
        this->mesh_handler = new MeshHandler(&mesh, &rsa, &ecc, &authHandler);
    }

    void setup() {
        if(!SPIFFS.begin()) {
            Serial.println("Failed to mount LittleFS! WiFi settings not read");
        } else {
            Serial.println("LittleFS was mounted!");
            wifi_conf.readMeshWiFi();
        }
        this->mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION | DEBUG);
        this->mesh.init(mesh_ssid, mesh_password, &scheduler, MESH_PORT);
        this->mesh.setContainsRoot(false);
        this->mesh.onNewConnection(this->newConnectionCallback);
        ecc.setup();
        this->mesh_handler->setup();
        newConnectionCallback(MAIN_DEVICE);
    }

    void update() {
        this->mesh.update();
        this->mesh_handler->update();
        this->authHandler.update();
    };

    void send(shared_ptr<IMeshCommand> command) {
        this->mesh_handler->send(command);
    }

    Restarter getRestarter(){return this->restarter;}

    void setRestarter(Restarter restarter){this->restarter = restarter;}

    ~SecMesh() {
        delete mesh_handler;
    }
};