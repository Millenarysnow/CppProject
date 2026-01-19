#pragma once

#include <vector>
#include <string>
#include "command.hpp"

using namespace std;

namespace MyShell {
    class Exit : public Command
    {
    public:
        Exit(MyShell::Shell* shell) : Command(shell) {}
        virtual void Execute(vector<string>& Args) override;
    };
}