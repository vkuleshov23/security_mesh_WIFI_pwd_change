#pragma once
#include "../../interfaces/IMeshCommand.hpp"
#include "../../settings/CommandNames.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class PingAnswerCommand : public IMeshCommand {
protected:

public:
    PingAnswerCommand(uint32_t target, long way_time)
    : IMeshCommand(PING_ANSWER, target, std::to_string(way_time)) {
    }
};