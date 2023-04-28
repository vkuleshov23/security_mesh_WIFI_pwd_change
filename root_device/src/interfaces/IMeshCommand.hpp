#pragma once
#include <string>

using namespace std;

class IMeshCommand {
public:
    virtual string getName();
    virtual void execute();
};