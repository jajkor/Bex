#include "Token.h"

// Member initializer list
Token::Token(TokenType type, std::string lexeme, literal lit, int line)
    : type(type), lexeme(std::move(lexeme)), lit(std::move(lit)), line(line) {}

std::string tokenTypeToString(TokenType type) {
  switch (type) {
  case TokenType::LEFT_PAREN:
    return "LEFT_PAREN";
  case TokenType::RIGHT_PAREN:
    return "RIGHT_PAREN";
  case TokenType::IDENTIFIER:
    return "IDENTIFIER";
  case TokenType::BOOL:
    return "BOOL";
  case TokenType::NOT:
    return "NOT";
  case TokenType::AND:
    return "AND";
  case TokenType::NAND:
    return "NAND";
  case TokenType::OR:
    return "OR";
  case TokenType::NOR:
    return "NOR";
  case TokenType::XOR:
    return "XOR";
  case TokenType::XNOR:
    return "XNOR";
  case TokenType::PRINT:
    return "PRINT";
  case TokenType::RETURN:
    return "RETURN";
  case TokenType::BIT:
    return "BIT";
  case TokenType::BIT_VECTOR:
    return "BIT_VECTOR";
  case TokenType::CIRCUIT:
    return "CIRCUIT";
  case TokenType::TRUE:
    return "TRUE";
  case TokenType::FALSE:
    return "FALSE";
  case TokenType::ENDOFFILE:
    return "ENDOFFILE";
  default:
    return "UNKNOWN";
  }
}

// Stream operator for TokenType
std::ostream &operator<<(std::ostream &os, const TokenType &type) {
  os << tokenTypeToString(static_cast<TokenType>(type));
  return os;
}

void Token::updateLine(int l) { this->line = l; }
