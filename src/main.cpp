#include <iostream>
#include <string>

using namespace std;

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  bool Start = true;
  while(Start)
  {
    std::cout << "$ ";

    string inputCommand;
    getline(cin, inputCommand);
    
    cout << inputCommand << ": " << "command not found" << endl;
  }
}
