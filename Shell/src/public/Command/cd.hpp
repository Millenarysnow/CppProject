#pragma once

#include <vector>
#include <string>

#include "command.hpp"

namespace MyShell {
    class Shell;

    class Cd : public Command
    {
    public:
        Cd() = default;
        Cd(MyShell::Shell* shell);
        virtual void Execute(std::vector<std::string>& Args) override;
    };
}