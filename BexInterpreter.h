#pragma once

#include <fstream>
#include <iostream>
#include <string>

class BexInterpreter {
private:
  bool hadError;
  void runFile(std::string fileName);
  void runPrompt();
  void run(std::string source);
  void report(int line, std::string where, std::string message);

public:
  BexInterpreter(int argc, char **argv);
  void error(int line, std::string message);
};
