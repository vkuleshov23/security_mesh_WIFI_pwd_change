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
    RSAAdatper* rsa;
    AuthHandler* auth;
public:
    AuthStep1Activator(RSAAdatper* rsa, AuthHandler* auth) : IMeshActivator(AUTH_STEP_1) {
        this->rsa = rsa;
        this->auth = auth;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        uint32_t target = command->get_transmitter();
        this->rsa->set_target_pub_key(target, command->get_data());
        std::string key = this->auth->genKeyAndStartAuth(target);
        this->addToAnswer(shared_ptr<IMeshCommand>(
            new AuthStep2Command(target, key, this->rsa)));
    }
};