#pragma once
#include "../../interfaces/IMeshCommand.hpp"
#include "../../settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/security/auth/AuthHandler.hpp"
#include "mesh/security/MESH_SHA256.hpp"
#include <string>
#include <Arduino.h>

using namespace std;

class SendPasswordCommand : public IMeshCommand {
protected:
    std::string get_singed_password(uint32_t target, std::string password, AuthHandler* auth) {
        std::string payload = auth->addGammaThenHash(target, 2);
        payload += MESH_SHA256::hashing(password);
        return password + "|" + MESH_SHA256::hashing(payload);
    }
public:
    SendPasswordCommand(uint32_t target, std::string password, RSAAdatper* rsa, AuthHandler* auth) 
    : IMeshCommand(SEND_PASSWORD, target, rsa->encrypt_for_target(get_singed_password(target, password, auth), target)) {
    }
};