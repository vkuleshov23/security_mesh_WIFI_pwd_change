#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class AuthStep5Command : public IMeshCommand {
protected:

public:
    AuthStep5Command(uint32_t target, RSAAdatper* rsa)
    : IMeshCommand(AUTH_STEP_5, target, 
        rsa->encrypt_for_target(rsa->get_public_key(), target)) {}
};