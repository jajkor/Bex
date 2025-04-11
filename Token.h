#pragma once

#include <string>

enum class TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,

  // Literals.
  IDENTIFIER,
  BOOL,
  NUMBER,

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

  ENDOFFILE,
};

std::ostream &operator<<(std::ostream &os, const TokenType &type);

struct literal {
  bool boolean;
};

class Token {
public:
  TokenType type;
  std::string lexeme;
  literal lit;
  int line;

  std::string toString();
  void updateLine(int l);
  Token(TokenType type, std::string lexeme, literal lit, int line);
};
