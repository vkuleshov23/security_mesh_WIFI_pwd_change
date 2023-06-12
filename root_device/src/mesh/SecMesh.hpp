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
#include "security/MESH_SHA256.hpp"
#include "security/auth/ModularExp.hpp"

#ifdef ESP8266
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
            this->send(shared_ptr<IMeshCommand>(new AuthInitCommand(MAIN_DEVICE)));
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
    Timer auth_connect_provocator;

    void check_auth() {
        if(auth_connect_provocator == 0) {
            auth_connect_provocator.start(AUTH_CHECK);
            bool isConnected = mesh.isConnected(MAIN_DEVICE);
            bool isAuth = authHandler.isAuth(MAIN_DEVICE);
            if(!isConnected) {
                if(isAuth) {
                    authHandler.disable(MAIN_DEVICE);
                }
            } else {
                if(!isAuth) {
                    this->send(shared_ptr<IMeshCommand>(new AuthInitCommand(MAIN_DEVICE)));
                }
            }
        }
    }

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
        Serial.printf("MY NODE ID -> %zu\n", this->mesh.getNodeId());
    }

    void update() {
        this->mesh.update();
        this->mesh_handler->update();
        this->authHandler.update();
        this->check_auth();
    };

    void send(shared_ptr<IMeshCommand> command) {
        this->mesh_handler->send(command);
    }

    void send_encrypt(const char* com_name, uint32_t target, string data) {
        this->send(shared_ptr<IMeshCommand>(
            new IMeshCommand(com_name, target, this->rsa.encrypt_for_target(data, target))));
    }

    

    void crypt_speed_test(std::string data) {
        Serial.print("data: ");
        Serial.println(data.c_str());

        unsigned long ts = millis();
        std::string res = this->rsa.encrypt(data);
        ts = millis() - ts;

        Serial.print(data.length());
        Serial.print(" length string encrypt in ");
        Serial.print(ts);
        Serial.println(" millis");

        Serial.print("\nencrypt data: ");
        Serial.println(res.c_str());

        ts = millis();
        data = this->rsa.decrypt(res);
        ts = millis() - ts;

        Serial.print(res.length());
        Serial.print(" length string decrypt in ");
        Serial.print(ts);
        Serial.println(" millis");

        Serial.print("\ndecrypt data: ");
        Serial.println(data.c_str());
    }

    void hash_speed_test(std::string data) {
        Serial.print("data: ");
        Serial.println(data.c_str());

        unsigned long ts = millis();
        std::string hash = MESH_SHA256::hashing(data);
        ts = millis() - ts;
        Serial.print(data.length());
        Serial.print(" length string decrypt in ");
        Serial.print(ts);
        Serial.println(" millis");

        Serial.print("\ndata hesh: ");
        Serial.print(hash.c_str());
        Serial.print("\ndata hesh len: ");
        Serial.println(hash.length());
    }

    void modular_exp_speed_test(uint32_t a, uint32_t d, uint32_t m) {
        Serial.print("base: ");
        Serial.println(a);
        Serial.print("degree: ");
        Serial.println(d);
        Serial.print("module: ");
        Serial.println(m);

        unsigned long ts = millis();
        uint64_t res = ModularExp::mod_exp(a, d, m);
        ts = millis() - ts;
        Serial.print("res: ");
        Serial.println(res);
        Serial.print("was calculated in: ");
        Serial.print(ts);
        Serial.println(" millis");
    }

    Restarter getRestarter(){return this->restarter;}

    void setRestarter(Restarter restarter){this->restarter = restarter;}

    ~SecMesh() {
        delete mesh_handler;
    }
};