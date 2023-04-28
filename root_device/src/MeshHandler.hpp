#pragma once
#include "painlessMesh.h"
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <WiFiConsts.h>
#include "Scenario.hpp"

using namespace std;

class MeshHandler {
protected:
    painlessMesh* mesh;
    StaticJsonDocument<512> settings;
    vector<Scene> scenes;

public:
    function<void(const uint32_t &, const String &)> receivedCallback = [this](const uint32_t &from, const String &msg ){
        Serial.printf("bridge: Received from %u msg=%s\n", from, msg.c_str());
        
        DynamicJsonDocument data(256);
        deserializeJson(data, msg);

        //Node msg
        if(data["command"].as<String>() == "sendEventToExecutor"){
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
        }else
        
        if(data["command"].as<String>() == "sendEventToRoot"){
            String activator = data["activator"].as<String>();
            if(this->mesh->isRoot()){
                for(uint32_t i = 0; i < this->scenes.size(); ++i){
                    if(this->scenes[i].activator == activator){
                        Serial.printf("find scene: activator=%s; event=%s; executor=%s; target=%u;\n",
                                     this->scenes[i].activator.c_str(),
                                     this->scenes[i].event.c_str(),
                                     this->scenes[i].executor.c_str(),
                                     this->scenes[i].target);
                        this->sendEventToExecutor(this->scenes[i].target, this->scenes[i].executor, this->scenes[i].event);
                    }
                }
            }else{
                this->sendEventToRoot(activator);
            }
        }
    };

};