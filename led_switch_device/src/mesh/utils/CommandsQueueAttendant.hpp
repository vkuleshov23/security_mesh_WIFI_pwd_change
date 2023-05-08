#pragma once
#include "../interfaces/IMeshCommand.hpp"
#include "CommandsQueue.hpp"
#include <queue>

using namespace std;

class CommandsQueueAttendant {
protected:
    queue<IMeshCommand*>* commands = new queue<IMeshCommand*>;
    CommandsQueue* c_queue = new CommandsQueue(this->commands);; 
public:
    CommandsQueue* getQueue() {
        return this->c_queue;
    }
    
    IMeshCommand* serve() {
        if (!this->commands->empty()) {
            IMeshCommand* command = this->commands->front();
            this->commands->pop();
            return command;
        } else {
            return NULL;
        }
    }
};