#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/commands/test/RSASerialAnswerCommand.hpp"
#include <string>
#include <Arduino.h>
#include <memory>


class RSASeraialActivator : public IMeshActivator {
protected:
    RSAAdatper* rsa;
public:
    RSASeraialActivator(RSAAdatper* rsa) : IMeshActivator(RSA_PRINT) {
        this->rsa = rsa;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        // string data = this->rsa->decrypt(command->get_data());
        // Serial.println(data.c_str());
        Serial.println(command->get_data().c_str());
        this->addToAnswer(shared_ptr<IMeshCommand>(
                new RSASerialAnswerCommand(command->get_transmitter(), command->get_data() + " !", rsa)));
    }
};