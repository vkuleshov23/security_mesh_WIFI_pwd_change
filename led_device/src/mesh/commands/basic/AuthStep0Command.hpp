#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class AuthStep0Command : public IMeshCommand {
protected:
public:
    AuthStep0Command(uint32_t target)
    : IMeshCommand(AUTH_STEP_0, target, string(AUTH_STEP_0)) {}
};