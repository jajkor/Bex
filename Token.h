#pragma once

#include <string>
#include <vector>

enum class TokenType {
  // Single-character tok.
  LEFT_PAREN,
  RIGHT_PAREN,

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

  ENDOFFILE,
};

std::ostream &operator<<(std::ostream &os, const TokenType &type);

struct literal {
  bool boolean;
  std::vector<bool> bits; // For bit vectors
  bool is_bitvector;      // Flag to indicate if this is a bit vector
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
