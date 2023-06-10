#pragma once
#include <string>
#include <Arduino.h>
#include <memory>
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"

class AuthStep5Activator : public IMeshActivator {
protected:
    RSAAdatper* rsa;

public:
    AuthStep5Activator(RSAAdatper* rsa) : IMeshActivator(AUTH_STEP_5) {
        this->rsa = rsa;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        uint32_t target = command->get_transmitter();
        this->rsa->set_target_pub_key(target, this->rsa->decrypt(command->get_data()));
    }
};