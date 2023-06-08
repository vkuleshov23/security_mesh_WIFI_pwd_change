#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/ECCAdapter.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class AuthStep2Command : public IMeshCommand {
protected:
    std::string encryptRsaAndPublicData(uint32_t target, std::string public_data, RSAAdatper* rsa) {
        std::string packet = 
            rsa->encrypt_for_target(rsa->get_public_key(), target) + "|" + public_data; 
            // rsa->encrypt_for_target(rsa->get_public_key(), target) + "|" + rsa->encrypt_for_target(public_data, target); 
        return packet;
    }

public:
    AuthStep2Command(uint32_t target, std::string public_data, RSAAdatper* rsa)
    : IMeshCommand(AUTH_STEP_2, target, encryptRsaAndPublicData(target, public_data, rsa)) {}
};