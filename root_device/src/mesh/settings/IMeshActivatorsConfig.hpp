#pragma once
#include <map>
#include <functional>
#include <string>
#include "mesh/interfaces/IMeshActivator.hpp"
#include "mesh/activators/SerialPrintActivator.hpp"
#include "mesh/activators/basic/PingActivator.hpp"
#include "mesh/activators/basic/PingAnswerActivator.hpp"
#include "mesh/activators/basic/RSAKeyActivator.hpp"
#include "mesh/activators/basic/RSAKeyAnswerActivator.hpp"
#include "mesh/activators/basic/AuthErrorActivator.hpp"
#include "mesh/activators/basic/AuthInitActivator.hpp"
#include "mesh/activators/basic/AuthStep0Activator.hpp"
#include "mesh/activators/basic/AuthStep1Activator.hpp"
#include "mesh/activators/basic/AuthStep2Activator.hpp"
#include "mesh/activators/basic/AuthStep3Activator.hpp"
#include "mesh/activators/basic/AuthStep4Activator.hpp"
#include "mesh/activators/basic/AuthStep5Activator.hpp"
#include "mesh/activators/test/RSASerialActivator.hpp"
#include "mesh/activators/test/RSASerialAnswerActivator.hpp"
#include "mesh/security/ECCAdapter.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/security/auth/AuthHandler.hpp"

class IMeshActivators {
protected:
    RSAAdatper* rsa;
    ECCAdatper* ecc;
    AuthHandler* auth;
    
    std::map<std::string, shared_ptr<IMeshActivator>> activators;
    void setActivator(shared_ptr<IMeshActivator> activator) {
        this->activators[activator->getName()] = activator;
    }

private:
    void basic_setup() {
        this->setActivator(shared_ptr<IMeshActivator>(new PingActivator()));
        this->setActivator(shared_ptr<IMeshActivator>(new PingAnswerActivator()));
        this->setActivator(shared_ptr<IMeshActivator>(new RSAKeyActivator(rsa)));
        this->setActivator(shared_ptr<IMeshActivator>(new RSAKeyAnswerActivator(rsa)));
        this->setActivator(shared_ptr<IMeshActivator>(new AuthInitActivator()));
        this->setActivator(shared_ptr<IMeshActivator>(new AuthStep0Activator(auth)));
        this->setActivator(shared_ptr<IMeshActivator>(new AuthStep1Activator(auth)));
        this->setActivator(shared_ptr<IMeshActivator>(new AuthStep2Activator(auth)));
        this->setActivator(shared_ptr<IMeshActivator>(new AuthStep3Activator(rsa, auth)));
        this->setActivator(shared_ptr<IMeshActivator>(new AuthStep4Activator(rsa)));
        this->setActivator(shared_ptr<IMeshActivator>(new AuthStep5Activator(rsa)));
        this->setActivator(shared_ptr<IMeshActivator>(new AuthErrorActivator(auth)));
        this->setActivator(shared_ptr<IMeshActivator>(new RSASeraialActivator(rsa)));
        this->setActivator(shared_ptr<IMeshActivator>(new RSASerialAnswerActivator(rsa)));
    }

public:
    IMeshActivators(RSAAdatper* rsa, ECCAdatper* ecc, AuthHandler* auth)
    : rsa(rsa), ecc(ecc), auth(auth) {this->setup();}

    void setup() {
        this->basic_setup();
        this->setActivator(shared_ptr<IMeshActivator>(new SerialPrintActivator()));
    }

    shared_ptr<IMeshActivator> getActivator(shared_ptr<IMeshCommand> command) {
        return this->getActivator(command->get_name());
    }

    shared_ptr<IMeshActivator> getActivator(std::string command_name) {
        if(activators.find(command_name) != activators.end()) {
            return activators[command_name];
        } else {
            Serial.print(command_name.c_str());
            Serial.println(" not found");
            return NULL;
        }
    }
};