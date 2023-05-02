#pragma once
#include <string>
#include <map>
#include <string>
#include <memory>
#include "../interfaces/IMeshEvent.hpp"

using namespace std;

class MeshEvents {
private:
    map<string, shared_ptr<IMeshEvent>> scenes;
public:
    void addCommand(shared_ptr<IMeshEvent> scene) {
        this->scenes[scene->getName()] = value;
    }
    shared_ptr<IMeshEvent> getSmartValue(string name) {
        return scenes[name];
    }
};