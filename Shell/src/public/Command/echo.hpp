#pragma once

#include <vector>
#include <string>

#include "command.hpp"

namespace MyShell {
    class Echo : public Command
    {
    public:
        Echo(MyShell::Shell* shell) : Command(shell) {}
        virtual void Execute(vector<string>& Args) override;
    };
}