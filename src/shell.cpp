#include "shell.hpp"

#ifdef _WIN32
constexpr char PATH_LIST_SEPARATOR = ';';
#else
constexpr char PATH_LIST_SEPARATOR = ':';
#include <unistd.h>
#include <fcntl.h>
#endif

#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>

#include "exit.hpp"
#include "echo.hpp"
#include "type.hpp"
#include "pwd.hpp"
#include "cd.hpp"
#include "utils.hpp"

MyShell::Shell::Shell()
{
    Exit* exit = new Exit(this);
    Echo* echo = new Echo(this);
    Type* type = new Type(this);
    Pwd* pwd = new Pwd(this);
    Cd* cd = new Cd(this);

    Commands.insert({"exit", "echo", "type", "pwd", "cd"});
    Execute.insert({{"exit", exit}, {"echo", echo}, {"type", type}, {"pwd", pwd}, {"cd", cd}});

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
    RedirectOperator = -1;

    std::getline(std::cin, Input);

    Input += ' '; // 方便最后一个参数处理

    string temp;
    bool InSingleQuote = false;
    bool InDoubleQuote = false;
    for (int i = 0; i < Input.length(); i++)
    {
        if(Input[i] == ' ') // 空格
        {
            if(InSingleQuote || InDoubleQuote)
            {
                temp += Input[i];
            }
            else // 处理拿到的完整参数
            {
                if(!temp.empty())
                {
                    if(InputCommand.empty())
                    {
                        InputCommand = temp;
                    }
                    else
                    {
                        Args.push_back(temp);

                        if(Args.back().back() == '>') // 定位重定向符号
                        {
                            RedirectOperator = Args.size() - 1;
                        }
                    }
                    temp.clear();
                }
            }
        }
        else if(Input[i] == '\'') // 单引号
        {
            if(InDoubleQuote == false)
                InSingleQuote = !InSingleQuote;
            else
                temp += Input[i];
        }
        else if(Input[i] == '\"') // 双引号
        {
            if(InSingleQuote == false)
                InDoubleQuote = !InDoubleQuote;
            else
                temp += Input[i];
        }
        else if(Input[i] == '\\') // 转义符
        {
            if(InSingleQuote) // 单引号内完全不转义
                temp += Input[i];
            else if(InDoubleQuote)
            {
                i++;
                if(i < Input.length())
                {
                    if(Input[i] == '\"' || Input[i] == '\\' || Input[i] == '$' || Input[i] == '`') // ！！没有处理转义换行符的情况
                    // TODO: 处理转义换行符的情况
                        temp += Input[i];
                    else
                    {
                        temp += '\\';
                        temp += Input[i];
                    }
                }
            }
            else // 不在引号内转义
            {
                i++;
                if(i < Input.length())
                    temp += Input[i];
            }
        }
        else // 普通字符
        {
            temp += Input[i];
        }
    }

    Input.pop_back(); // 去掉最后多余的空格
}

void MyShell::Shell::execute()
{
    const int StdoutFd = dup(STDOUT_FILENO);
    const int StderrFd = dup(STDERR_FILENO);
    
    if (RedirectOperator != -1) // 处理重定向
    {
        if (RedirectOperator + 1 < Args.size())
        {
            int FileFd = -1;
            if(Args[RedirectOperator] == ">>" || Args[RedirectOperator] == "1>>" || Args[RedirectOperator] == "2>>") // 追加
                FileFd = open(Args[RedirectOperator + 1].c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
            else // 覆盖
                FileFd = open(Args[RedirectOperator + 1].c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);

            if (FileFd == -1)
            {
                std::cout << "Redirection file open error" << std::endl;
                close(StdoutFd);
                close(StderrFd);
                return;
            }

            Args.erase(Args.begin() + RedirectOperator, Args.begin() + RedirectOperator + 2);

            if(Args[RedirectOperator] == ">" || Args[RedirectOperator] == "1>" || Args[RedirectOperator] == ">>" || Args[RedirectOperator] == "1>>")
                dup2(FileFd, STDOUT_FILENO);
            else if(Args[RedirectOperator] == "2>" || Args[RedirectOperator] == "2>>")
                dup2(FileFd, STDERR_FILENO);
            close(FileFd);
        }
    }

    // --------- 执行命令 ---------

    if (Commands.find(InputCommand) != Commands.end()) // 内置命令
    {
        Command* cmd = Execute[InputCommand];
        if (cmd)
            cmd->Execute(Args);
    }
    else
    {
        string dir = MyShell::is_exist(InputCommand, PathDir);
        if (!dir.empty()) // 外部命令
        {
            cast_args(); 
            MyShell::call_external_program(dir, Args, this);
        }
        else
        {
            std::cout << Input << ": " << "command not found" << std::endl;
        }
    }

    std::cout.flush();
    fflush(stdout);

    dup2(StdoutFd, STDOUT_FILENO);
    close(StdoutFd);
    dup2(StderrFd, STDERR_FILENO);
    close(StderrFd);
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
