#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/security/auth/AuthHandler.hpp"
#include "mesh/commands/basic/AuthStep3Command.hpp"
#include "mesh/commands/basic/AuthErrorCommand.hpp"
#include <string>
#include <Arduino.h>
#include <memory>


class AuthStep2Activator : public IMeshActivator {
protected:
    AuthHandler* auth;

    void authenticate(uint32_t target, std::string hash_key_with_gamma) {
        if(this->auth->check_and_try_auth(target, hash_key_with_gamma, 1)) {
            Serial.printf("AUTHENTICATED: %zu \n", target);
            std::string hash = this->auth->addGammaThenHash(target, 2);
            this->addToAnswer(shared_ptr<IMeshCommand>(new AuthStep3Command(target, hash)));
        } else {
            this->addToAnswer(shared_ptr<IMeshCommand>(new AuthErrorCommand(target)));
        }
    }

public:
    AuthStep2Activator(AuthHandler* auth) : IMeshActivator(AUTH_STEP_2) {
        this->auth = auth;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        this->authenticate(command->get_transmitter(), command->get_data());
    }
};