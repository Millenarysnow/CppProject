#pragma once 

#include <string>
#include <vector>

namespace MyShell {
    class Shell;

    std::string is_exist(std::string& CmdName, const std::vector<std::string>& PathDir);

    int call_external_program(const std::string& ProgramPath, const std::vector<std::string>& Args, const MyShell::Shell* shell);

    int make_multilevel_dirs(const std::string& path);
}