#pragma once
#include "../interfaces/IMeshCommand.hpp"
#include <string>

using namespace std;

class VoidCommand : IMeshCommand {
private:
    string name = "void";
public:
    string getName() {
        return name;
    }
    void execute() {
        
    }
};