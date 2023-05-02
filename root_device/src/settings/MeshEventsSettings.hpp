#pragma once
#include <memory>
#include "../interfaces/IScenario.hpp"
#include "../commands/VoidCommand.hpp"
#include "../utils/MeshSmartValues.hpp"


class MeshScenarioSettings {
protected:
    MeshSmartValues values;
public:
    MeshScenarioSettings() {
        values.addCommand(std::make_shared<IScenario>(VoidCommand()));
    }
    MeshSmartValues getCommands() {
        return values;
    }
};
