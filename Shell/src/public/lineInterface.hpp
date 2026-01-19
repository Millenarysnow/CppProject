#pragma once

#ifdef _WIN32
struct linenoiseCompletions;
#endif

namespace MyShell 
{
    class Shell;

    void cross_platform_register_linecompletion(Shell* shell);

    char* cross_platform_readline(char* prompt);

#ifdef _WIN32
    void completion(const char* buf, linenoiseCompletions* lc);
#else
    char* command_generator(const char* text, int state);
#endif 
}