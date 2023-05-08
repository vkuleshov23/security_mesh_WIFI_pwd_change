#pragma once
#include "../../interfaces/IMeshCommand.hpp"
#include "../../settings/CommandNames.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class PingCommand : public IMeshCommand {
protected:

public:
    PingCommand(uint32_t target) 
    : IMeshCommand(PING, target, std::to_string(millis())) {
    }
};