#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/ECCAdapter.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class AuthErrorCommand : public IMeshCommand {
protected:
public:
    AuthErrorCommand(uint32_t target)
    : IMeshCommand(AUTH_ERROR, target, string(AUTH_ERROR)) {}
};