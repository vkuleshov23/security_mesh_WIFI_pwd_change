#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class AuthStep3Command : public IMeshCommand {
protected:
public:
    AuthStep3Command(uint32_t target, std::string credentials)
    : IMeshCommand(AUTH_STEP_3, target, credentials) {}
};