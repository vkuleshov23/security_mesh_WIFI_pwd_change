#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/security/auth/AuthHandler.hpp"
#include "mesh/commands/basic/AuthStep4Command.hpp"
#include "mesh/commands/basic/AuthErrorCommand.hpp"
#include <string>
#include <Arduino.h>
#include <memory>

class AuthStep3Activator : public IMeshActivator {
protected:
    RSAAdatper* rsa;
    AuthHandler* auth;

    void authenticate(uint32_t target, std::string hash_key_with_gamma) {
        if(this->auth->check_and_try_auth(target, hash_key_with_gamma, 1)) {
            Serial.printf("AUTHENTICATED: %zu \n", target);
            std::string hash = this->auth->addGammaThenHash(target, 2);
            Serial.println(hash.c_str());
            this->addToAnswer(shared_ptr<IMeshCommand>(
                new AuthStep4Command(target, hash, this->rsa)));
        } else {
            this->addToAnswer(shared_ptr<IMeshCommand>(
                new AuthErrorCommand(target)));
        }
    }

public:
    AuthStep3Activator(RSAAdatper* rsa, AuthHandler* auth) : IMeshActivator(AUTH_STEP_3) {
        this->rsa = rsa;
        this->auth = auth;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        uint32_t target = command->get_transmitter();
        std::string hash_key_with_gamma = this->rsa->decrypt(command->get_data());
        this->authenticate(target, hash_key_with_gamma);
    }
};