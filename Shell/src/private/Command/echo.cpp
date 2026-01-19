#include "echo.hpp"

#include <iostream>

void MyShell::Echo::Execute(vector<string> &Args)
{
    if(Args.empty())
    {
        std::cout << std::endl;
        return;
    }

    for (int i = 0; i < Args.size() - 1; i++)
    {
        std::cout << Args[i] << " ";
    }
    std::cout << Args[Args.size() - 1] << std::endl;
}