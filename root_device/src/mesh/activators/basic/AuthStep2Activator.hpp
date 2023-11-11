#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/security/auth/AuthHandler.hpp"
#include "mesh/commands/basic/AuthStep3Command.hpp"
#include "mesh/commands/basic/AuthErrorCommand.hpp"
#include "mesh/security/MESH_SHA256.hpp"
#include <string>
#include <Arduino.h>
#include <memory>


class AuthStep2Activator : public IMeshActivator {
protected:
    AuthHandler* auth;
    RSAAdatper* rsa;

    std::string get_message(uint32_t target) {
        std::string pub_key = this->rsa->get_public_key();
        std::string hash2 = this->auth->addGammaThenHash(target, 2);
        std::string payload = hash2 + MESH_SHA256::hashing(pub_key);
        std::string hash_payload = MESH_SHA256::hashing(payload);
        // std::string hash3 = this->auth->addGammaThenHash(target, 3);
        return pub_key + "|" + hash_payload;
    }

    void authenticate(uint32_t target, std::string hash_key) {
        if(this->auth->check_and_try_auth(target, hash_key, 1)) {
            Serial.printf("AUTHENTICATED: %zu\n", target);
            this->addToAnswer(shared_ptr<IMeshCommand>(new AuthStep3Command(target, get_message(target))));
        } else {
            this->addToAnswer(shared_ptr<IMeshCommand>(new AuthErrorCommand(target)));
        }
    }

public:
    AuthStep2Activator(AuthHandler* auth, RSAAdatper* rsa) : IMeshActivator(AUTH_STEP_2) {
        this->auth = auth;
        this->rsa = rsa;

    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        this->authenticate(command->get_transmitter(), command->get_data());
    }
};