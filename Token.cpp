#include "Token.h"

// Member initializer list
Token::Token(TokenType type, std::string lexeme, literal lit, int line) : type(type), lexeme(std::move(lexeme)), lit(std::move(lit)), line(line) {

}

void Token::updateLine(int l) {
  this->line = l;
}
