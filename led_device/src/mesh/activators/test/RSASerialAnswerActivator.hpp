#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include <string>
#include <Arduino.h>
#include <memory>


class RSASerialAnswerActivator : public IMeshActivator {
protected:
    RSAAdatper* rsa;
public:
    RSASerialAnswerActivator(RSAAdatper* rsa) : IMeshActivator(RSA_PRINT_ANSWER) {
        this->rsa = rsa;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        string data = this->rsa->decrypt(command->get_data());
        Serial.print("RSA PRINT ANSWER: ");
        Serial.println(data.c_str());
    }
};