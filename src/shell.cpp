#include "shell.hpp"

#ifdef _WIN32
constexpr char PATH_LIST_SEPARATOR = ';';
#else
constexpr char PATH_LIST_SEPARATOR = ':';
#endif

#include <iostream>
#include <sstream>

#include <cstring>

#include "exit.hpp"
#include "echo.hpp"
#include "type.hpp"
#include "pwd.hpp"
#include "utils.hpp"

MyShell::Shell::Shell()
{
    Exit* exit = new Exit(this);
    Echo* echo = new Echo(this);
    Type* type = new Type(this);
    Pwd* pwd = new Pwd(this);

    Commands.insert({"exit", "echo", "type", "pwd"});
    Execute.insert({{"exit", exit}, {"echo", echo}, {"type", type}, {"pwd", pwd}});

    get_path_dirs();
}

MyShell::Shell::~Shell()
{
    if(CArgs != nullptr)
    {
        for(int i = 0; CArgs[i] != nullptr; i++)
        {
            delete[] CArgs[i];
        }
        delete[] CArgs;
        CArgs = nullptr;
    }
}

bool MyShell::Shell::is_builtin(const string &cmd) const
{
    return Commands.find(cmd) != Commands.end();
}

void MyShell::Shell::input()
{
    Input.clear();
    InputCommand.clear();
    Args.clear();

    std::getline(std::cin, Input);

    stringstream ss;
    ss.str(Input);

    string temp;
    ss >> InputCommand;
    while(ss >> temp)
        Args.push_back(temp);
}

void MyShell::Shell::execute()
{
    if(Commands.find(InputCommand) != Commands.end())
    {
        Command* cmd = Execute[InputCommand];
        if (cmd)
            cmd->Execute(Args);

        return ;
    }

    string dir = MyShell::is_exist(InputCommand, PathDir);
    if(!dir.empty())
    {
        cast_args();
        MyShell::call_external_program(dir, Args, this);
        return ;
    }

    std::cout << Input << ": " << "command not found" << std::endl;
}

void MyShell::Shell::get_path_dirs()
{
    const char* value = std::getenv("PATH");
    if(value != nullptr)
    {
        std::string Path = std::string(value);

        std::stringstream ss;
        ss.str(Path);

        std::string temp;
        while(getline(ss, temp, PATH_LIST_SEPARATOR))
        {
            PathDir.push_back(temp);
        }
    }
    else
    {
        PathDir = {};
    }
}

void MyShell::Shell::cast_args()
{
    if(CArgs != nullptr)
    {
        for(int i = 0; CArgs[i] != nullptr; i++)
        {
            delete[] CArgs[i];
        }
        delete[] CArgs;
    }

    vector<string> temp = {InputCommand};
    temp.insert(temp.end(), Args.begin(), Args.end());

    CArgs = new char*[temp.size() + 1];
    for (int i = 0; i < temp.size(); i++)
    {
        const char* cstr = temp[i].c_str();
        char* copy = new char[temp[i].length() + 1];
        strcpy(copy, cstr);
        CArgs[i] = copy;
    }
    CArgs[temp.size()] = nullptr;
}
