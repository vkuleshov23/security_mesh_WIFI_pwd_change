#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/ECCAdapter.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class RSAKeyCommand : public IMeshCommand {
protected:
public:
    RSAKeyCommand(uint32_t target, RSAAdatper* rsa)
    : IMeshCommand(RSA_KEY, target, rsa->get_public_key()) {
    }
};