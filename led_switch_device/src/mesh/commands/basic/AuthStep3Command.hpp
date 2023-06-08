#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/ECCAdapter.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/security/SHA1.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class AuthStep3Command : public IMeshCommand {
protected:

public:
    AuthStep3Command(uint32_t target, std::string credential, RSAAdatper* rsa)
    : IMeshCommand(AUTH_STEP_3, target, credential) {}
    // : IMeshCommand(AUTH_STEP_3, target, rsa->encrypt_for_target(credential, target)) {}
};