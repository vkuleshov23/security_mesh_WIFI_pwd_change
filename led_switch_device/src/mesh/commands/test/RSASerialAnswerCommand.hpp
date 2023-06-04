#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include <string>

using namespace std;

class RSASerialAnswerCommand : public IMeshCommand {
protected:

public:
    RSASerialAnswerCommand(uint32_t target, string data, RSAAdatper* rsa) 
    : IMeshCommand(RSA_PRINT_ANSWER, target, rsa->encrypt_for_target(data, target)) {}
};