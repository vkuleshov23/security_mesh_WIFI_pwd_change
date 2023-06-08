#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/commands/basic/AuthStep1Command.hpp"
#include <Arduino.h>
#include <memory>


class AuthStep0Activator : public IMeshActivator {
protected:
    AuthHandler* auth;
public:
    AuthStep0Activator(AuthHandler* auth) : IMeshActivator(AUTH_STEP_0) {
        this->auth = auth;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        this->addToAnswer(shared_ptr<IMeshCommand>(
            new AuthStep1Command(command->get_transmitter(), auth->genKeyAndStartAuth(command->get_transmitter()))));
    }
};