#pragma once
#include "../interfaces/IMeshCommand.hpp"
#include "../settings/CommandNames.hpp"
#include <string>

using namespace std;

class SerialPrintCommand : public IMeshCommand {
protected:

public:
    SerialPrintCommand(uint32_t target, string data) 
    : IMeshCommand(PRINT, target, data) {}
};