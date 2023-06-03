#pragma once
#include "painlessMesh.h"
#include <vector>
#include <map>
#include <functional>
#include <string>
#include "interfaces/IMeshCommand.hpp"
#include "utils/CommandToJSON.hpp"
#include "utils/JSONToCommand.hpp"
#include "settings/IMeshActivatorsConfig.hpp"
#include "utils/CommandsQueueAttendant.hpp"
#include "security/auth/AuthHandler.hpp"
#include "mesh/commands/basic/AuthStep0Command.hpp"

using namespace std;

class MeshHandler {
protected:
    painlessMesh* mesh;
    IMeshActivators* activators;
    CommandsQueueAttendant commands;
    RSAAdatper* rsa;
    ECCAdatper* ecc;
    AuthHandler* auth;

    void publish() {
        shared_ptr<IMeshCommand> command = this->commands.serve();
        if(command != NULL) {
            this->publish(command);
        }
    }

    void publish(shared_ptr<IMeshCommand> command) {
        command->set_transmitter(this->mesh->getNodeId());
        String json = CommandToJSON::getJSON(command);
        this->mesh->sendSingle(command->get_target(), json);       
    }

public:
    MeshHandler(painlessMesh* mesh, RSAAdatper* rsa, ECCAdatper* ecc, AuthHandler* auth) {
        this->mesh = mesh;
        this->rsa = rsa;
        this->ecc = ecc;
        this->auth = auth;
        activators = new IMeshActivators(this->rsa, this->ecc, this->auth);
    }

    function<void(const uint32_t &, const String &)> receivedCallback = [this](const uint32_t &from, const String &msg ){
        shared_ptr<IMeshCommand> command = JSONToCommand::getCommand(msg);
        shared_ptr<IMeshActivator> activator = activators->getActivator(command);
        if(activator != NULL) {
            activator->processCommand(command);
            activator->answer(commands.getQueue());
        } else {
            Serial.println("NO SUCH ACTIVATOR EXIST");
        }
    };

    function<void(const uint32_t &)> newConnectionCallback = [this](const uint32_t &from){
        if(from == MAIN_DEVICE) {
            CommandsQueue* queue = this->commands.getQueue();
            queue->addCommand(shared_ptr<IMeshCommand>(new AuthStep0Command(from)));
        }
    };

    void setup() {
        this->mesh->onReceive(this->receivedCallback);
        this->mesh->onNewConnection(this->newConnectionCallback);
    }

    void update() {
        this->publish();
    }

    void send(shared_ptr<IMeshCommand> command) {
        CommandsQueue* queue = this->commands.getQueue();
        queue->addCommand(command);
    }

    ~MeshHandler() {
        delete(activators);
    }

};