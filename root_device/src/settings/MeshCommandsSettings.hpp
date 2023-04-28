#pragma once
#include <memory>
#include "../interfaces/IMeshCommand.hpp"
#include "../commands/VoidCommand.hpp"
#include "../utils/MeshCommands.hpp"


class MeshCommandsSettings {
protected:
    MeshCommands commands;
public:
    MeshCommandsSettings() {
        commands.addCommand(std::make_shared<IMeshCommand>(VoidCommand()));
    }
    MeshCommands getCommands() {
        return commands;
    }
};
