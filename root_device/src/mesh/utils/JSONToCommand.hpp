#pragma once
#include "../interfaces/IMeshCommand.hpp"
#include <string>
#include <AsyncJson.h>

using namespace std;

class JSONToCommand {
protected:
public:
    JSONToCommand() {}
    static inline shared_ptr<IMeshCommand> getCommand(String str_doc) {
        StaticJsonDocument<2048> doc;
        deserializeJson(doc, str_doc);
        IMeshCommand command(doc["name"], doc["transmitter"], doc["target"], doc["data"]);
        return shared_ptr<IMeshCommand>(new IMeshCommand(doc["name"], doc["transmitter"], doc["target"], doc["data"]));
    }
};