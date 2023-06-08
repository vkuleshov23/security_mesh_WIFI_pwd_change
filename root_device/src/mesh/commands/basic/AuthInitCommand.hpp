#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class AuthInitCommand : public IMeshCommand {
protected:
public:
    AuthInitCommand(uint32_t target)
    : IMeshCommand(AUTH_INIT, target, string(AUTH_INIT)) {}
};