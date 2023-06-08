#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include <string>
#include <map>
#include <Arduino.h>
#include <memory>


class RSAKeyAnswerActivator : public IMeshActivator {
protected:
    RSAAdatper* rsa;
public:
    RSAKeyAnswerActivator(RSAAdatper* rsa) : IMeshActivator(RSA_ANSWER_KEY) {
        this->rsa = rsa;
    }
    
    void process(std::shared_ptr<IMeshCommand> command) override {
        Serial.print("----");
        std::string key = this->rsa->decrypt(command->get_data());
        Serial.print("----");
        this->rsa->set_target_pub_key(command->get_transmitter(), key);
        Serial.print("----");
        Serial.print("RSA KEY: ");
        Serial.println(rsa->get_public_key().c_str());
        Serial.print("RSA GET KEY: ");
        Serial.println(key.c_str());
    }
};