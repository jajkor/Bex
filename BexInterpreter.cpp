#include "BexInterpreter.h"

const std::string HELP_MESSAGE =
    R"(Bex is a Boolean expression interpreter

Usage: bex [options] [script]

Options:
  -d, --debug
      Use debug output
  -h, --help
      Print help
)";

void printScannerResults(std::string line,
                         std::vector<std::shared_ptr<Token>> tokens) {
  std::cout << "LINE: " << "\n" << line << "\n";

  std::cout << "KEYWORD:\t" << "LEXEME:" << "\n";

  for (auto &token : tokens) {
    std::cout << token.get()->type << "\t" << token.get()->lexeme << "\n";
  }
  std::cout << std::endl;
}

void BexInterpreter::runFile(std::string fileName) {
  std::string line;
  std::ifstream sourceFile(fileName);
  if (sourceFile.is_open()) {
    while (getline(sourceFile, line)) {
      run(line);
    }
    sourceFile.close();
  } else {
    std::cerr << "Error: Unable to open file";
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
  if (BexInterpreter::opt.isDebugMode()) {
    printScannerResults(source, tokens);
  }
}

BexInterpreter::BexInterpreter(int argc, char **argv) {
  std::regex debugPattern("^(-d|--debug)$");
  std::regex helpPattern("^(-h|--help)$");
  std::regex bxFilePattern(R"(^(.+)\.bx$)");

  // Process all arguments
  for (int i = 1; i < argc; i++) {
    std::string arg(argv[i]);
    std::smatch match;

    if (std::regex_match(arg, match, debugPattern)) {
      opt.setDebugMode(true);
    } else if (std::regex_match(arg, match, bxFilePattern)) {
      if (opt.hasFileName()) {
        std::cerr << "Error: Multiple .bx files specified" << "\n";
        std::cerr << "Usage: bex [options] [script.bx]" << "\n";
        exit(EXIT_FAILURE);
      }
      opt.setFileName(arg);
    } else if (std::regex_match(arg, match, helpPattern)) {
      std::cout << HELP_MESSAGE << std::endl;
      exit(EXIT_SUCCESS);
    } else {
      std::cerr << "Unknown argument: " << arg << "\n";
      std::cerr << "Usage: bex [options] [script.bx]" << "\n";
      exit(EXIT_FAILURE);
    }
  }
  // Run file or prompt based on whether a file was specified
  if (opt.hasFileName()) {
    runFile(opt.getFileName());
  } else {
    runPrompt();
  }
}
