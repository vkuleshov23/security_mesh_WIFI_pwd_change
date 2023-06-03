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
    RSAAdatper* rsa;
public:
    AuthStep0Activator(RSAAdatper* rsa) : IMeshActivator(AUTH_STEP_0) {
        this->rsa = rsa;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        this->addToAnswer(shared_ptr<IMeshCommand>(
            new AuthStep1Command(command->get_transmitter(), this->rsa)));
    }
};