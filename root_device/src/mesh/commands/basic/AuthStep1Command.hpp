#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/ECCAdapter.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class AuthStep1Command : public IMeshCommand {
protected:
public:
    AuthStep1Command(uint32_t target, RSAAdatper* rsa)
    : IMeshCommand(AUTH_STEP_1, target, rsa->get_public_key()) {}
};