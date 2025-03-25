#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "Scanner.h"

class BexInterpreter {
private:
  bool hadError;
  void runFile(std::string fileName);
  void runPrompt();
  void run(std::string source);

public:
  BexInterpreter(int argc, char **argv);
};
