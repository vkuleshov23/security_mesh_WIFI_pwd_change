#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/commands/basic/AuthStep0Command.hpp"
#include <Arduino.h>
#include <memory>


class AuthInitActivator : public IMeshActivator {
public:
    AuthInitActivator() : IMeshActivator(AUTH_INIT) {}

    void process(std::shared_ptr<IMeshCommand> command) override {
        this->addToAnswer(shared_ptr<IMeshCommand>(
            new AuthStep0Command(command->get_transmitter())));
    }
};