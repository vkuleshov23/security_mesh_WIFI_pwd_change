#pragma once
#include <memory>
#include "../interfaces/ISmartValue.hpp"
#include "../commands/VoidCommand.hpp"
#include "../utils/MeshSmartValues.hpp"


class MeshSmartValuesSettings {
protected:
    MeshSmartValues values;
public:
    MeshSmartValuesSettings() {
        values.addCommand(std::make_shared<ISmartValue>(VoidCommand()));
    }
    MeshSmartValues getCommands() {
        return values;
    }
};
