#pragma once
#include <string>
#include <Arduino.h>
#include <memory>
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/security/auth/AuthHandler.hpp"
#include "mesh/commands/basic/AuthStep5Command.hpp"
#include "mesh/commands/basic/AuthErrorCommand.hpp"

class AuthStep4Activator : public IMeshActivator {
protected:
    RSAAdatper* rsa;

public:
    AuthStep4Activator(RSAAdatper* rsa) : IMeshActivator(AUTH_STEP_4) {
        this->rsa = rsa;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        uint32_t target = command->get_transmitter();
        this->rsa->set_target_pub_key(target, command->get_data());
        this->addToAnswer(shared_ptr<IMeshCommand>(new AuthStep5Command(target, this->rsa)));
    }
};