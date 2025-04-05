#pragma once

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "Options.h"
#include "Scanner.h"

class BexInterpreter {
private:
  Options opt;
  void runFile(std::string fileName);
  void runPrompt();
  void run(std::string source);

public:
  BexInterpreter(int argc, char **argv);
};
