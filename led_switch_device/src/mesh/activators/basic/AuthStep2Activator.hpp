#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/settings/CommandNames.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/security/auth/AuthHandler.hpp"
#include "mesh/commands/basic/AuthStep3Command.hpp"
#include <string>
#include <Arduino.h>
#include <memory>


class AuthStep2Activator : public IMeshActivator {
protected:
    RSAAdatper* rsa;
    AuthHandler* auth;

    void set_rsa(uint32_t target, std::string encrypted_public_key) {
        std::string public_key = this->rsa->decrypt(encrypted_public_key);
        Serial.println(public_key.c_str());
        this->rsa->set_target_pub_key(target, public_key);
        yield();
    }

    std::string set_data_and_start_auth(uint32_t target, std::string encrypted_public_data) {
        // std::string public_data = this->rsa->decrypt(encrypted_public_data);
        // Serial.println(public_data.c_str());
        // this->auth->startAuth(target, public_data);
        this->auth->startAuth(target, encrypted_public_data);
        yield();
        return this->auth->addGammaThenHash(target, 1);
    }

public:
    AuthStep2Activator(RSAAdatper* rsa, AuthHandler* auth) : IMeshActivator(AUTH_STEP_2) {
        this->rsa = rsa;
        this->auth = auth;
    }

    void process(std::shared_ptr<IMeshCommand> command) override {
        uint32_t target = command->get_transmitter();
        std::string data = command->get_data();
        int index = data.find('|');

        std::string encrypted_public_key = data.substr(0, index);
        this->set_rsa(target, encrypted_public_key);

        std::string encrypted_public_data = data.substr(index+1);
        std::string data_with_gamma = this->set_data_and_start_auth(target, encrypted_public_data);

        this->addToAnswer(shared_ptr<IMeshCommand>(
            new AuthStep3Command(target, data_with_gamma, this->rsa)));
    }
};