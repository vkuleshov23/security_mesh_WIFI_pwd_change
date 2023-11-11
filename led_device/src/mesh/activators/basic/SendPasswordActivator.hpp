#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/security/auth/AuthHandler.hpp"
#include "mesh/security/MESH_SHA256.hpp"
#include <string>
#include <Arduino.h>
#include <memory>

class SendPasswordActivator : public IMeshActivator {
protected:
    RSAAdatper* rsa;
    AuthHandler* auth;

    std::string getHash(uint32_t target, std::string password) {
        std::string payload = this->auth->addGammaThenHash(target, 2);
        payload += MESH_SHA256::hashing(password);
        return MESH_SHA256::hashing(payload);
    }

    void getPassw(uint32_t target, std::string data) {
        int index = data.find('|');
        std::string password = data.substr(0, index);
        std::string hash = data.substr(index+1);
        if(hash == getHash(target, password)) {
                Serial.printf("password: %s\n", password.c_str());
        }
    }

public:
    SendPasswordActivator(RSAAdatper* rsa, AuthHandler* auth) : IMeshActivator(SEND_PASSWORD) {
        this->rsa = rsa;
        this->auth = auth;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        uint32_t target = command->get_transmitter();
        unsigned long ts = millis();
        this->getPassw(target, rsa->decrypt(command->get_data()));
        ts = millis() - ts;
        Serial.print("Password from ");
        Serial.print(target);
        Serial.print(" was checked and handled in: ");
        Serial.print(ts);
        Serial.println(" millis");
    }
};