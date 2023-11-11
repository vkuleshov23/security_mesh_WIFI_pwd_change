#pragma once
#include "../interfaces/IMeshCommand.hpp"
#include "../interfaces/IMeshActivator.hpp"
#include "../settings/CommandNames.hpp"
#include <string>
#include <Arduino.h>
#include <memory>


class SerialPrintActivator : public IMeshActivator {
protected:

public:
    SerialPrintActivator() : IMeshActivator(PRINT) {}

    void process(std::shared_ptr<IMeshCommand> command) override {
        // this->setParam(command->get_data());
        Serial.println(command->get_data().c_str());
    }
};