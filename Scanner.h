#pragma once

#include <bitset>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Token.h"

class Scanner {
private:
  std::string source;
  std::vector<std::shared_ptr<Token>> tokens;
  std::map<std::string, Token> keyword_table;
  int start, end, current, line;

  bool isAtEnd();
  bool match(char c);
  bool isDigit(char c);
  char peek();
  char peekNext();
  bool isAlpha(char c);
  bool isAldigit(char c);
  void handleIdentifier();
  void handleBitLiteral();
  void scanToken();
  char advance();
  void addToken(TokenType type);
  void addToken(TokenType type, literal lit);

public:
  std::vector<std::shared_ptr<Token>> scanTokens();
  Scanner(std::string source);
};
