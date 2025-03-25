#include "BexInterpreter.h"

void BexInterpreter::runFile(std::string fileName) {
  std::string line;
  std::ifstream sourceFile(fileName);
  if (sourceFile.is_open()) {
    while (getline(sourceFile, line)) {
      run(line);
    }
    sourceFile.close();
  } else {
    std::cout << "Unable to open file";
  }
}

void BexInterpreter::runPrompt() {
  std::string line;
  std::cout << ">> ";

  while (std::getline(std::cin, line)) {
    std::cout << line << std::endl;
    run(line);
    std::cout << ">> ";
  }
}

void BexInterpreter::run(std::string source) {
  Scanner s(source);
  auto tokens = s.scanTokens();
}

BexInterpreter::BexInterpreter(int argc, char **argv) {
  hadError = false;

  if (argc > 2) {
    std::cout << "Usage: bex [script]";
    exit(0);
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    runPrompt();
  }
}
