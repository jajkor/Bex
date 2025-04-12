#include "BexInterpreter.h"
#include "Evaluator.h" // Include our new Evaluator
#include "Utils.h"     // Include the header, not the cpp file

const std::string HELP_MESSAGE =
    R"(Bex is a Boolean expression interpreter

Usage: bex [options] [script]

Options:
  -d, --verbose
      Print verbose output
  -h, --help
      Print help
)";

void BexInterpreter::runFile(std::string fileName) {
  std::string line;
  std::ifstream sourceFile(fileName);
  if (sourceFile.is_open()) {
    std::string source;
    while (getline(sourceFile, line)) {
      source += line + "\n";
    }
    run(source);
    sourceFile.close();
  } else {
    std::cerr << "Error: Unable to open file";
  }
}

void BexInterpreter::runPrompt() {
  std::string line;
  std::cout << ">> ";

  while (std::getline(std::cin, line)) {
    if (line.empty())
      break;
    run(line);
    std::cout << ">> ";
  }
}

void BexInterpreter::run(std::string source) {
  // Scan tokens
  Scanner scanner(source);
  auto tokens = scanner.scanTokens();

  if (BexInterpreter::opt.isDebugMode()) {
    printTokenStream(tokens);
  }

  // Parse tokens
  Parser parser(tokens);
  auto statements = parser.parse();

  if (BexInterpreter::opt.isDebugMode() && !statements.empty()) {
    printParseResults(statements);
  }

  // Evaluate parsed statements if there are any
  if (!statements.empty()) {
    Evaluator evaluator;
    evaluator.evaluate(statements);
  }
}

BexInterpreter::BexInterpreter(int argc, char **argv) {
  std::regex verbosePattern("^(-d|--verbose)$");
  std::regex helpPattern("^(-h|--help)$");
  std::regex bxFilePattern(R"(^(.+)\.bx$)");

  // Process all arguments
  for (int i = 1; i < argc; i++) {
    std::string arg(argv[i]);
    std::smatch match;

    if (std::regex_match(arg, match, verbosePattern)) {
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
