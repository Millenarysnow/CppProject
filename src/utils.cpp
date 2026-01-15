#include "utils.hpp"

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#endif

#include "shell.hpp"

#include <iostream>

std::string MyShell::is_exist(std::string &CmdName, const std::vector<std::string>& PathDir)
{
    std::string result;

    for (auto& dir : PathDir)
    {
        std::string current_dir = dir + "/" + CmdName;

#ifdef _WIN32
        if (_access(current_dir.c_str(), 02) == 0)
#else
        if (access(current_dir.c_str(), X_OK) == 0)
#endif
        {
            result = current_dir;
            break;
        }
    }

    return result;
}

int MyShell::call_external_program(const std::string &ProgramPath, const std::vector<std::string> &Args, const MyShell::Shell* shell)
{
    int retn = 0;
#ifdef _WIN32
    // ...
#else
    pid_t pid = fork();
    if (pid == 0)
    {
        execv(ProgramPath.c_str(), shell->get_cargs());
        std::cerr << "execv failed" << std::endl;
        exit(1);
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            retn = WEXITSTATUS(status);
        }
    }
    else
    {
        std::cerr << "fork failed" << std::endl;
    }
#endif
    return retn;
}

int MyShell::make_multilevel_dirs(const std::string &path)
{
    string current_path;
    for (auto it = path.rbegin(); it != path.rend(); it++)
    {
        // TODO : 支持windows
        if (*it != '/') continue;

        current_path = path.substr(0, path.rend() - it - 1);
        break;
    }

    if(current_path.empty()) return -1;

    if(access(current_path.c_str(), F_OK) == 0)
    {
        return 0;
    }
    else
    {
        if(mkdir(current_path.c_str(), 0755) == 0)
        {
            return 0;
        }
        else
        {
            make_multilevel_dirs(current_path);
            return mkdir(current_path.c_str(), 0755);
        }
    }

    return -1;
}
