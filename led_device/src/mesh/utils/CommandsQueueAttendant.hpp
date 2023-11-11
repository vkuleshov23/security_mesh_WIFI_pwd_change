#pragma once
#include "mesh/interfaces/IMeshCommand.hpp"
#include "CommandsQueue.hpp"
#include <queue>

using namespace std;

class CommandsQueueAttendant {
protected:
    queue<shared_ptr<IMeshCommand>>* commands = new queue<shared_ptr<IMeshCommand>>;
    CommandsQueue* c_queue = new CommandsQueue(this->commands); 
public:
    CommandsQueue* getQueue() {
        return this->c_queue;
    }
    
    shared_ptr<IMeshCommand> serve() {
        if (!this->commands->empty()) {
            shared_ptr<IMeshCommand> command = this->commands->front();
            this->commands->pop();
            return command;
        } else {
            return NULL;
        }
    }
    
    ~CommandsQueueAttendant() {
        delete commands;
        delete c_queue;
    }
};