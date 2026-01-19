#pragma once

#include <vector>
#include <string>

#include "shell.hpp"

using namespace std;

namespace MyShell {
    class Command
    {
    public:
        Command(MyShell::Shell* _shell) : shell(_shell) { };

        virtual void Execute(vector<string>& Args) = 0;

    protected:
        MyShell::Shell* shell;
    };
}