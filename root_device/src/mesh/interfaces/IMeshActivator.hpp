#pragma once

#include <functional>
#include <string>
#include <list>
#include "IMeshCommand.hpp"
#include "../utils/CommandsQueue.hpp"

using namespace std;

class IMeshActivator {
private:
    list<IMeshCommand*> commandsToSend;
protected:
    string command_name;
    string param;

    virtual void process(IMeshCommand* command) {}
    const void addToAnswer(IMeshCommand* command) {
        this->commandsToSend.push_back(command);
    }

public:

    IMeshActivator(const string command_name)
    : command_name(command_name){}

    const string getName() const{
        return command_name;
    }

    const string getParam() const{
        return param;
    }

    const void answer(CommandsQueue* queue) {
        std::list<IMeshCommand*>::iterator i = commandsToSend.begin();
        while(i != commandsToSend.end()) {
            queue->addCommand(*i);
            i = commandsToSend.erase(i);
        }
    }

    void setParam(string param) {
        this->param = param;
    }

    void processCommand(IMeshCommand* command){
        process(command);
    }

};