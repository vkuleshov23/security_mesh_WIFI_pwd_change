#pragma once
#include "../interfaces/IMeshCommand.hpp"
#include <string>
#include <AsyncJson.h>

using namespace std;

class JSONToCommand {
protected:
public:
    JSONToCommand() {}
    static inline IMeshCommand* getCommand(String str_doc) {
        StaticJsonDocument<512> doc;
        deserializeJson(doc, str_doc);
        IMeshCommand* command = new IMeshCommand(doc["name"], doc["transmitter"], doc["target"], doc["data"]);
        return command;
    }
};