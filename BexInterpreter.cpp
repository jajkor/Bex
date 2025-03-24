#include "BexInterpreter.h"

void BexInterpreter::runFile(std::string fileName) {
  std::string line;
  std::ifstream sourceFile(fileName);
  if (sourceFile.is_open()) {
    while (getline(sourceFile, line)) {
      std::cout << line << '\n';
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

void BexInterpreter::error(int line, std::string message) {
  report(line, "", message);
}

void BexInterpreter::report(int line, std::string where, std::string message) {
  std::cout << "[line " << line << "] Error" << where << ": " << message;
  hadError = true;
}

void BexInterpreter::run(std::string source) {}

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
