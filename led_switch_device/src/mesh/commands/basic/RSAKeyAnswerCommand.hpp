#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/ECCAdapter.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class RSAKeyAnswerCommand : public IMeshCommand {
protected:
public:
    RSAKeyAnswerCommand(uint32_t target, RSAAdatper* rsa)
    : IMeshCommand(RSA_ANSWER_KEY, target, rsa->encrypt_for_target(rsa->get_public_key(), target)) {
    }
};