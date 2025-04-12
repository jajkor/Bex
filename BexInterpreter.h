#pragma once

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "AstPrinter.h"
#include "Options.h"
#include "Parser.h"
#include "Scanner.h"
#include "Stmt.h"

class BexInterpreter {
private:
  Options opt;
  void runFile(std::string fileName);
  void runPrompt();
  void run(std::string source);
  void printParseResults(const std::vector<std::shared_ptr<Stmt>> &statements);

public:
  BexInterpreter(int argc, char **argv);
};
