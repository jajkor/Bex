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

void printScannerResults(std::string line,
                         std::vector<std::shared_ptr<Token>> tokens) {
  std::cout << "Scanning Line: " << line << std::endl;

  std::cout << std::endl << "KEYWORD:\t" << "LEXEME:" << std::endl;

  for (auto token : tokens) {
    TokenType t = token.get()->type;
    std::cout << t << ", \t\t" << token.get()->lexeme << std::endl;
  }
}

void BexInterpreter::run(std::string source) {
  Scanner s(source);
  auto tokens = s.scanTokens();
  if (opt.getDebug() == true)
    printScannerResults(source, tokens);
}

BexInterpreter::BexInterpreter(int argc, char **argv) {
  Options opt;
  std::regex debugPattern("^(-d|--debug)$");
  std::regex helpPattern("^(-h|--help)$");
  std::regex bxFilePattern(R"(^(.+)\.bx$)");
  hadError = false;

  // Process all arguments
  for (int i = 1; i < argc; i++) {
    std::string arg(argv[i]);
    std::smatch match;

    if (std::regex_match(arg, match, debugPattern)) {
      opt.setDebug(true);
    } else if (std::regex_match(arg, match, bxFilePattern)) {
      if (opt.hasFileName()) {
        std::cout << "Error: Multiple .bx files specified\n";
        std::cout << "Usage: bex [options] [script.bx]\n";
        exit(1);
      }
      opt.setFileName(arg);
    } else {
      std::cout << "Unknown argument: " << arg << "\n";
      std::cout << "Usage: bex [options] [script.bx]\n";
      exit(1);
    }
  }

  // Run file or prompt based on whether a file was specified
  if (opt.hasFileName()) {
    runFile(opt.getFileName());
  } else {
    runPrompt();
  }
}
