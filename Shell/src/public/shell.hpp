#pragma once

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

namespace MyShell{

    class Command;

    class Shell
    {
    public:
        Shell();
        ~Shell();

        vector<string> match_comands(const string& text);

        bool get_start() const { return Start; }
        void set_start(bool val) { Start = val; }

        const vector<string>& get_dirs_vector() const { return PathDir; }
        const set<string>& get_commands() const { return BuiltinCommands; }
        char** get_cargs() const { return CArgs; }

        bool is_builtin(const string& cmd) const;

        void input();

        void execute();
    private:
        bool Start = true;    

        string Input;

        string InputCommand;
        vector<string> Args;

        set<string> BuiltinCommands;
        unordered_map<string, Command*> Execute;

        set<string> AllCommands;

        std::vector<std::string> PathDir;

        char** CArgs = nullptr;

        int RedirectOperator = -1;

        void get_path_dirs();
        void cast_args();
    };
}