#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "painlessMesh.h"
#include "settings/WiFiConfigurer.hpp"
#include "mesh/commands/test/RSASerialCommand.hpp"
#include "mesh/commands/basic/RSAKeyCommand.hpp"
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
            this->send(shared_ptr<IMeshCommand>(new AuthStep0Command(from)));
            // this->send(shared_ptr<IMeshCommand>(new RSAKeyCommand(from, &this->rsa)));
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
        Serial.printf("MY NODE ID -> %zu\n", this->mesh.getNodeId());
    }

    void update() {
        this->mesh.update();
        this->mesh_handler->update();
        this->authHandler.update();
    };

    void send(shared_ptr<IMeshCommand> command) {
        this->mesh_handler->send(command);
    }

    void send_encrypt(const char* com_name, uint32_t target, string data) {
        this->send(shared_ptr<IMeshCommand>(
            new IMeshCommand(com_name, target, this->rsa.encrypt_for_target(data, target))));
    }

    Restarter getRestarter(){return this->restarter;}

    void setRestarter(Restarter restarter){this->restarter = restarter;}

    ~SecMesh() {
        delete mesh_handler;
    }
};