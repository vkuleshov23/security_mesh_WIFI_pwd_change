#pragma once
#include "../interfaces/IMeshCommand.hpp"
#include "../interfaces/ISmartValue.hpp"
#include <string>
#include <vector>
#include "ArduinoJson.h"

using namespace std;

class SendToExecutor : IMeshCommand {
private:
    string name = "sendToExecutor";
    vector<ISmartValue> value;
public:
    string getName() {
        return name;
    }
    void execute(DynamicJsonDocument data) {
        if(data["target"].as<uint32_t>() != this->mesh->getNodeId()){
            this->sendEventToExecutor(data["target"].as<uint32_t>(), data["executor"].as<String>(), data["event"].as<String>());
        }else{
            Serial.printf("trying to find executor");
            String executor = data["executor"].as<String>();
            for(uint32_t i = 0; i < this->values.size(); ++i){
                if(this->values[i].getName() == executor){
                    Serial.printf("executor is finded");
                    this->values[i].processEvent(data["event"].as<String>());
                }
            }
        }
    }
};