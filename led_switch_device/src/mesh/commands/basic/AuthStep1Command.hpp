#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class AuthStep1Command : public IMeshCommand {
protected:

public:
    AuthStep1Command(uint32_t target, std::string credentials)
    : IMeshCommand(AUTH_STEP_1, target, credentials) {}
};