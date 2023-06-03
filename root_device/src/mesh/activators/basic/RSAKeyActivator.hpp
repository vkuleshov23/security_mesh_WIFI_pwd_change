#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include <string>
#include <map>
#include <Arduino.h>
#include <memory>


class RSAKeyActivator : public IMeshActivator {
protected:
    RSAAdatper* rsa;
public:
    RSAKeyActivator(RSAAdatper* rsa) : IMeshActivator(RSA_KEY) {
        this->rsa = rsa;
    }
    
    void process(std::shared_ptr<IMeshCommand> command) override {
        this->rsa->set_target_pub_key(command->get_transmitter(), command->get_data());
    }
};