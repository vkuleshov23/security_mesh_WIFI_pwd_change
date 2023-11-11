#pragma once
#include "../interfaces/IMeshCommand.hpp"
#include <string>
#include <queue>

using namespace std;

class CommandsQueue {
protected:
    queue<shared_ptr<IMeshCommand>>* commands = NULL;
public:
    CommandsQueue(queue<shared_ptr<IMeshCommand>>* commands): commands(commands){}
    void addCommand(shared_ptr<IMeshCommand> command) {
        this->commands->push(command);
    }
};