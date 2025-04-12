#include "BexInterpreter.h"
#include "Evaluator.h"    // Include our new Evaluator
#include "ScannerDebug.h" // Include the header, not the cpp file

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

void BexInterpreter::printParseResults(
    const std::vector<std::shared_ptr<Stmt>> &statements) {
  std::cout << "PARSE RESULTS: " << statements.size() << " statements"
            << std::endl;

  AstPrinter printer;
  for (int i = 0; i < statements.size(); i++) {
    std::string result = printer.print(statements[i]);
    std::cout << i + 1 << ": " << result << std::endl;
  }
}

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

  // Always print the token stream for debugging
  printTokenStream(tokens);

  if (BexInterpreter::opt.isDebugMode()) {
    printScannerResults(source, tokens);
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
