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
    std::map<std::string, shared_ptr<IMeshActivator>> activators;
    void setActivator(shared_ptr<IMeshActivator> activator) {
        this->activators[activator->getName()] = activator;
    }

private:
    void basic_setup() {
        this->setActivator(shared_ptr<IMeshActivator>(new PingActivator()));
        this->setActivator(shared_ptr<IMeshActivator>(new PingAnswerActivator()));
    }

public:
    IMeshActivators() { this->basic_setup(); }

    void setup() {
        this->setActivator(shared_ptr<IMeshActivator>(new SerialPrintActivator()));
    }

    shared_ptr<IMeshActivator> getActivator(shared_ptr<IMeshCommand> command) {
        return this->getActivator(command->get_name());
    }

    shared_ptr<IMeshActivator> getActivator(std::string command_name) {
        if(activators.find(command_name) != activators.end()) {
            return activators[command_name];
        } else {
            Serial.print(command_name.c_str());
            Serial.println(" nor found");
            return NULL;
        }
    }
};