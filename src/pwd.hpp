#pragma once

#include <vector>
#include <string>

#include "command.hpp"

namespace MyShell {
    class Shell;

    class Pwd : public Command
    {
    public:
        Pwd() = default;
        Pwd(MyShell::Shell* shell);
        virtual void Execute(std::vector<std::string>& Args) override;
    };
}