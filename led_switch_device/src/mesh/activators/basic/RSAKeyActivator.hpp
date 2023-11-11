#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/commands/basic/RSAKeyAnswerCommand.hpp"
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
        Serial.print("RSA KEY: ");
        Serial.println(rsa->get_public_key().c_str());
        Serial.print("RSA GET KEY: ");
        Serial.println(command->get_data().c_str());
        this->addToAnswer(shared_ptr<IMeshCommand>(
                new RSAKeyAnswerCommand(command->get_transmitter(), rsa)));
    }
};