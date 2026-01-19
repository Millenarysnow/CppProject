#include "pwd.hpp"

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <iostream>

MyShell::Pwd::Pwd(MyShell::Shell *shell) : Command(shell)
{ }

void MyShell::Pwd::Execute(std::vector<std::string> &Args)
{
    char buffer[1024];

#ifdef _WIN32
    if (_getcwd(buffer, sizeof(buffer)) != nullptr)
    {
        std::cout << buffer << std::endl;
    }
    else
    {
        std::cerr << "pwd: error retrieving current directory" << std::endl;
    }
#else
    if (getcwd(buffer, sizeof(buffer)) != nullptr)
    {
        std::cout << buffer << std::endl;
    }
    else
    {
        std::cerr << "pwd: error retrieving current directory" << std::endl;
    }
#endif
}
