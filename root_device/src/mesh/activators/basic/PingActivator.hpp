#pragma once
#include "../../interfaces/IMeshCommand.hpp"
#include "../../interfaces/IMeshActivator.hpp"
#include "../../settings/CommandNames.hpp"
#include "../../commands/basic/PingAnswerCommand.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class PingActivator : public IMeshActivator {
protected:
public:
    PingActivator() 
    : IMeshActivator(PING) {}

    void process(shared_ptr<IMeshCommand> command) override {
        Serial.print(command->get_name().c_str());
        Serial.print(" from: ");
        Serial.println(command->get_transmitter());
        this->addToAnswer(shared_ptr<IMeshCommand>(
            new PingAnswerCommand(command->get_transmitter(), std::stol(command->get_data()))));
    }
};