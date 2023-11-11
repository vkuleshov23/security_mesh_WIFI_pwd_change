#pragma once
#include "../interfaces/IMeshCommand.hpp"
#include <string>
#include <AsyncJson.h>

using namespace std;

class CommandToJSON {
protected:

public:
    CommandToJSON() {}
    static inline String getJSON(shared_ptr<IMeshCommand> command) {
        DynamicJsonDocument doc(2048);
        String res;
        doc["name"] = command->get_name();
        doc["target"] = command->get_target();
        doc["transmitter"] = command->get_transmitter();
        doc["data"] = command->get_data();
        serializeJson(doc, res);
        return res;
        
    }
};