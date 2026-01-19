#pragma once

#include <vector>
#include <string>

#include "command.hpp"

namespace MyShell {
    class Type : public Command
    {
    public:
        Type() = default;
        Type(MyShell::Shell* shell);
        virtual void Execute(std::vector<std::string>& Args) override;

    private:
    };
}
