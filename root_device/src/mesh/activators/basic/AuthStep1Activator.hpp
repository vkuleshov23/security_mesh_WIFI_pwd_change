#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/security/auth/AuthHandler.hpp"
#include "mesh/commands/basic/AuthStep2Command.hpp"
#include <string>
#include <Arduino.h>
#include <memory>


class AuthStep1Activator : public IMeshActivator {
protected:
    AuthHandler* auth;
public:
    AuthStep1Activator(AuthHandler* auth) : IMeshActivator(AUTH_STEP_1) {
        this->auth = auth;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        uint32_t target = command->get_transmitter();
        this->auth->startAuth(target, command->get_data());
        this->addToAnswer(shared_ptr<IMeshCommand>(
            new AuthStep2Command(target, this->auth->addGammaThenHash(target, 1))));
    }
};