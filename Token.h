#pragma once

#include <string>

enum TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,

  // One or two character tokens.

  // Literals.
  IDENTIFIER,
  BOOL,

  // Keywords.
  NOT,
  AND,
  NAND,
  OR,
  NOR,
  XOR,
  XNOR,
  PRINT,
  RETURN,
  BIT,
  BIT_VECTOR,
  CIRCUIT,
  TRUE,
  FALSE,
};

struct literal {
  bool boolean;
};

class Token {
public:
  TokenType type;
  std::string lexeme;
  literal lit;
  int line;
};
