#include "type.hpp"

#include <iostream>
#include <cstdlib>
#include <sstream>

#include "utils.hpp"

MyShell::Type::Type(MyShell::Shell *shell) : Command(shell)
{ }

void MyShell::Type::Execute(std::vector<std::string> &Args)
{
    if(Args.size() == 0)
    {
        std::cout << "error" << std::endl;
        return;
    }

    if(shell->is_builtin(Args[0]))
    {
        std::cout << Args[0] << " is a shell builtin" << std::endl;
        return;
    }

    if(shell->get_dirs_vector().size() > 0)
    {
        std::string res = MyShell::is_exist(Args[0], shell->get_dirs_vector());
        if(!res.empty())
        {
            std::cout << Args[0] << " is " << res << std::endl;
            return;
        }
    }

    std::cout << Args[0] << ": not found" << std::endl;
}

