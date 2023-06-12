#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/security/auth/AuthHandler.hpp"
#include "mesh/commands/basic/AuthStep4Command.hpp"
#include "mesh/commands/basic/AuthErrorCommand.hpp"
#include "mesh/security/MESH_SHA256.hpp"
#include <string>
#include <Arduino.h>
#include <memory>

class AuthStep3Activator : public IMeshActivator {
protected:
    RSAAdatper* rsa;
    AuthHandler* auth;

    std::string getHash(uint32_t target, std::string pub_key) {
        std::string hash2 = this->auth->addGammaThenHash(target, 2);
        std::string payload = hash2 + MESH_SHA256::hashing(pub_key);
        std::string hash_payload = MESH_SHA256::hashing(payload);
        // std::string hash3 = this->auth->addGammaThenHash(target, 3);
        return hash_payload;
    }

    void authenticate(uint32_t target, std::string data) {
        int index = data.find('|');
        std::string key = data.substr(0, index);
        std::string hash = data.substr(index+1);
        if(hash == getHash(target, key)) {
            if(this->auth->auth(target)) {
                Serial.printf("AUTHENTICATED: %zu\n", target);
                this->rsa->set_target_pub_key(target, key);
                // this->addToAnswer(shared_ptr<IMeshCommand>(new AuthStep4Command(target, this->rsa)));
            }
        } else {
            this->addToAnswer(shared_ptr<IMeshCommand>(new AuthErrorCommand(target)));
        }
    }

public:
    AuthStep3Activator(RSAAdatper* rsa, AuthHandler* auth) : IMeshActivator(AUTH_STEP_3) {
        this->rsa = rsa;
        this->auth = auth;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        uint32_t target = command->get_transmitter();
        this->authenticate(target, command->get_data());
    }
};