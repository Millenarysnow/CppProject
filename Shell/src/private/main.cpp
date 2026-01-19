#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <sstream>

#include "shell.hpp"

using namespace std;

int main() 
{
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  MyShell::Shell shell;

  while(shell.get_start())
  {
    //std::cout << "$ ";

    shell.input();

    shell.execute();
  }
}
