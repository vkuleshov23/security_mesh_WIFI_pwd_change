#pragma once
#include "../interfaces/IMeshCommand.hpp"
#include <string>
#include <queue>

using namespace std;

class CommandsQueue {
protected:
    queue<IMeshCommand*>* commands = NULL;
public:
    CommandsQueue(queue<IMeshCommand*>* commands): commands(commands){}
    void addCommand(IMeshCommand* command) {
        this->commands->push(command);
    }
};