#include "lineInterface.hpp"

#ifdef _WIN32
#include "linenoise.h"
#else
#include <readline/readline.h>
#endif

#include "shell.hpp"
#include <string>
#include <vector>

static MyShell::Shell* ShellInstance = nullptr;

void MyShell::cross_platform_register_linecompletion(Shell* shell)
{
    ShellInstance = shell;
#ifdef _WIN32
    linenoiseSetCompletionCallback(MyShell::completion);
#else
    rl_attempted_completion_function = [](const char* text, int start, int end) -> char** 
    {
        rl_attempted_completion_over = 1; 
        if (start == 0) {
            return rl_completion_matches(text, command_generator);
        }
        return nullptr; 
    };
#endif
}

char* MyShell::cross_platform_readline(char *prompt)
{
#ifdef _WIN32
    return linenoise(prompt);
#else
    return readline(prompt);
#endif
}

#ifdef _WIN32
    void MyShell::completion(const char *buf, linenoiseCompletions *lc)
    {
        auto matches = ShellInstance->match_comands(std::string(buf));

        for (const auto& match : matches) 
        {
            linenoiseAddCompletion(lc, match.c_str());
        }
    }
#else
    char *MyShell::command_generator(const char *text, int state)
    {
        static std::vector<std::string> cache;
        static int index;

        if(state == 0)
        {
            cache = ShellInstance->match_comands(std::string(text));
            index = 0;
        }

        if (index < cache.size()) 
        {
            return strdup(cache[(index++) % cache.size()].c_str());
        } 
        else 
        {
            return nullptr;
        }

        return nullptr;
    }
#endif