#include "exit.hpp"

void MyShell::Exit::Execute(vector<string> &Args)
{
    shell->set_start(false);
}