#pragma once
#include <map>
#include <functional>
#include <string>
#include "../interfaces/IMeshActivator.hpp"
#include "../activators/SerialPrintActivator.hpp"
#include "../activators/basic/PingActivator.hpp"
#include "../activators/basic/PingAnswerActivator.hpp"

class IMeshActivators {
protected:
    std::map<std::string, IMeshActivator*> activators;
    void setActivator(IMeshActivator* activator) {
        this->activators[activator->getName()] = activator;
    }

private:
    void basic_setup() {}

public:
    IMeshActivators() { this->basic_setup(); }

    void setup() {
        this->setActivator(new SerialPrintActivator());
        this->setActivator(new PingActivator());
        this->setActivator(new PingAnswerActivator());
    }

    IMeshActivator* getActivator(IMeshCommand* command) {
        return this->getActivator(command->get_name());
    }

    IMeshActivator* getActivator(std::string command_name) {
        if(activators.find(command_name) != activators.end()) {
            return activators[command_name];
        } else {
            Serial.print(command_name.c_str());
            Serial.println(" nor found");
            return NULL;
        }
    }
};