#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class AuthStep2Command : public IMeshCommand {
protected:

public:
    AuthStep2Command(uint32_t target, std::string credentials)
    : IMeshCommand(AUTH_STEP_2, target, credentials) {}
};