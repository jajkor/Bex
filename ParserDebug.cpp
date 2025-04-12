
#include "ParserDebug.h"
#include <iostream>

// Simple debug function that runs before trying to parse a statement
void debugStatement(const Parser &parser, int current) {
  std::cout << "About to parse statement at position " << current << std::endl;
}

// Debug function that runs before trying to parse an expression
void debugExpression(const Parser &parser, int current) {
  std::cout << "About to parse expression at position " << current << std::endl;
}
