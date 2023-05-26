#pragma once
#include "../../interfaces/IMeshCommand.hpp"
#include "../../interfaces/IMeshActivator.hpp"
#include "../../settings/CommandNames.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class PingAnswerActivator : public IMeshActivator {
protected:

public:
    PingAnswerActivator() 
    : IMeshActivator(PING_ANSWER) {}

    void process(shared_ptr<IMeshCommand> command) override {
        Serial.print("ping delay: ");
        Serial.print(millis() - std::stol(command->get_data()));
        Serial.print(" | from: ");
        Serial.println(command->get_transmitter());
    }
};