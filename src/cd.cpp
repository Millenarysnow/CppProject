#include "cd.hpp"

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <iostream>

MyShell::Cd::Cd(MyShell::Shell *shell) : Command(shell)
{ }

void MyShell::Cd::Execute(std::vector<std::string> &Args)
{
    int ret = 0;
    
    if(!Args[0].empty())
    {
        if(Args[0][0] == '~')
        {
#ifdef _WIN32
            char* home = getenv("USERPROFILE");
#else
            char* home = getenv("HOME");
#endif
            if(home != nullptr)
            {
                std::string new_path = std::string(home) + Args[0].substr(1);
                ret = chdir(new_path.c_str());
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = chdir(Args[0].c_str());
        }
    }
    else
    {
        ret = -1;
    }


    if(ret == 0)
    { }
    else
    {
        std::cout << "cd: " << Args[0] << ": No such file or directory" << std::endl;
    }
}