#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class AuthStep4Command : public IMeshCommand {
protected:

public:
    AuthStep4Command(uint32_t target, RSAAdatper* rsa)
    : IMeshCommand(AUTH_STEP_4, target, rsa->get_public_key()) {}
};