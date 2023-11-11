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


class AuthErrorActivator : public IMeshActivator {
protected:
    AuthHandler* auth;
public:
    AuthErrorActivator(AuthHandler* auth) : IMeshActivator(AUTH_ERROR) {
        this->auth = auth;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        this->auth->disable(command->get_target());
    }
};