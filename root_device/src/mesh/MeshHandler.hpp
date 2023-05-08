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

using namespace std;

class MeshHandler {
protected:
    painlessMesh* mesh;
    IMeshActivators activators;
    CommandsQueueAttendant commands;

    void publish() {
        IMeshCommand* command = this->commands.serve();
        if(command != NULL) {
            this->publish(command);
        }
    }

    void publish(IMeshCommand* command) {
        command->set_transmitter(this->mesh->getNodeId());
        String json = CommandToJSON::getJSON(command);
        this->mesh->sendSingle(command->get_target(), json);       
    }

public:
    MeshHandler(painlessMesh* mesh) {
        this->mesh = mesh;
    }

    function<void(const uint32_t &, const String &)> receivedCallback = [this](const uint32_t &from, const String &msg ){
        IMeshCommand* command = JSONToCommand::getCommand(msg);
        IMeshActivator* activator = activators.getActivator(command);
        activator->processCommand(command);
        activator->answer(commands.getQueue());
    };

    void setup() {
        this->mesh->onReceive(this->receivedCallback);
        this->activators.setup();
    }

    void update() {
        this->publish();
    }

    void send(IMeshCommand* command) {
        CommandsQueue* queue = this->commands.getQueue();
        queue->addCommand(command);
    }

};