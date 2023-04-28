#pragma once
#include <string>
#include <map>
#include <string>
#include <memory>
#include "../interfaces/IMeshCommand.hpp"

using namespace std;

class MeshCommands {
private:
    map<string, shared_ptr<IMeshCommand>> commands;
public:
    void addCommand(shared_ptr<IMeshCommand> command) {
        this->commands[command->getName()] = command;
    }
    shared_ptr<IMeshCommand> getCommand(string name) {
        return commands[name];
    }
};