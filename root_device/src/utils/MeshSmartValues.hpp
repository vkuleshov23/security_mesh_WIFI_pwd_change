#pragma once
#include <string>
#include <map>
#include <string>
#include <memory>
#include "../interfaces/ISmartValue.hpp"

using namespace std;

class MeshSmartValues {
private:
    map<string, shared_ptr<ISmartValue>> values;
public:
    void addCommand(shared_ptr<ISmartValue> value) {
        this->values[value->getName()] = value;
    }
    shared_ptr<ISmartValue> getSmartValue(string name) {
        return values[name];
    }
};