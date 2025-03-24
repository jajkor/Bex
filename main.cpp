#include <iostream>

void runFile(char* fileName) {
  std::cout << fileName;
}

void runPrompt() {
  std::cout << "REPL";
}

int main (int argc, char *argv[]) {

  if (argc > 1) {
    runFile(argv[1]);
  } else if (argc == 1) {
    runPrompt();
  } else {
    std::cout <<  "Usage: Bex [script]";
  }
  return 0;
}
