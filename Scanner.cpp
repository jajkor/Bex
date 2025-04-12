#include "Scanner.h"

bool Scanner::isAtEnd() { return this->current >= this->end; }

std::vector<std::shared_ptr<Token>> Scanner::scanTokens() {
  while (!isAtEnd()) {
    start = current;
    scanToken();
  }

  this->tokens.push_back(
      std::make_shared<Token>(TokenType::ENDOFFILE, "", literal{}, line));
  return std::move(this->tokens);
}

bool Scanner::isDigit(char c) { return '0' == c || c == '1'; }
bool Scanner::isAlpha(char c) {
  return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_';
}

bool Scanner::isAldigit(char c) { return isAlpha(c) || isDigit(c); }

void Scanner::handleIdentifier() {
  while (!isAtEnd() && isAldigit(peek())) {
    this->current++;
  }

  std::string tmp =
      this->source.substr(this->start, this->current - this->start);

  auto iter = this->keyword_table.find(tmp);
  if (iter != this->keyword_table.end()) {
    iter->second.updateLine(this->line);

    this->tokens.push_back(std::make_shared<Token>(iter->second.type, tmp,
                                                   iter->second.lit, line));
  } else {
    this->tokens.push_back(
        std::make_shared<Token>(TokenType::IDENTIFIER, tmp, literal{}, line));
  }
}

void Scanner::handleBitLiteral() {
  // Skip the '0b' prefix if present
  if (peek() == '0' && peekNext() == 'b') {
    advance(); // Skip '0'
    advance(); // Skip 'b'
  }

  std::vector<bool> bits;
  // Read all 0s and 1s
  while (!isAtEnd() && (peek() == '0' || peek() == '1')) {
    char bit = advance();
    bits.push_back(bit == '1');
  }

  // Create the token
  literal lit;
  lit.is_bitvector = true;
  lit.bits = bits;

  // If it's a single bit, we can also set the boolean value
  if (bits.size() == 1) {
    lit.boolean = bits[0];
    this->tokens.push_back(std::make_shared<Token>(
        TokenType::BOOL,
        this->source.substr(this->start, this->current - this->start), lit,
        line));
  } else {
    this->tokens.push_back(std::make_shared<Token>(
        TokenType::BIT_VECTOR,
        this->source.substr(this->start, this->current - this->start), lit,
        line));
  }
}

char Scanner::peek() {
  if (this->current > this->source.length())
    return '\0';

  return this->source[this->current];
}

char Scanner::peekNext() {
  if (current + 1 >= this->source.length())
    return '\0';
  return this->source[current + 1];
}

bool Scanner::match(char c) {
  if (isAtEnd())
    return false;
  if (this->source[this->current] != c)
    return false;

  this->current++;
  return true;
}

char Scanner::advance() { return this->source[this->current++]; }

void Scanner::scanToken() {
  char c = advance();
  switch (c) {
  case '(':
    this->tokens.push_back(
        std::make_shared<Token>(TokenType::LEFT_PAREN, "(", literal{}, line));
    break;
  case ')':
    this->tokens.push_back(
        std::make_shared<Token>(TokenType::RIGHT_PAREN, ")", literal{}, line));
    break;
  case ' ':
  case '\r':
  case '\t':
    // Ignore whitespace
    break;
  case '\n':
    line++; // Increment line number
    break;
  case '0':
    if (peek() == 'b') {
      current--; // Move back to include the '0' in the token
      handleBitLiteral();
    } else {
      literal lit;
      lit.boolean = false;
      lit.is_bitvector = true;
      lit.bits = {false};
      this->tokens.push_back(
          std::make_shared<Token>(TokenType::BOOL, "0", lit, line));
    }
    break;
  case '1': {
    // Just a single '1'
    literal lit;
    lit.boolean = true;
    lit.is_bitvector = true;
    lit.bits = {true};
    this->tokens.push_back(
        std::make_shared<Token>(TokenType::BOOL, "1", lit, line));
    break;
  }
  default:
    if (isAlpha(c)) {
      handleIdentifier();
      break;
    }
    std::cout << "Error: Unexpected character '" << c << "' at line " << line
              << std::endl;
    break;
  }
}

Scanner::Scanner(std::string source)
    : source(source), start(0), current(0), line(1) {
  this->end = source.length();

  this->keyword_table.insert(
      std::make_pair("not", Token(TokenType::NOT, "not", literal{}, 0)));
  this->keyword_table.insert(
      std::make_pair("and", Token(TokenType::AND, "and", literal{}, 0)));
  this->keyword_table.insert(
      std::make_pair("nand", Token(TokenType::NAND, "nand", literal{}, 0)));
  this->keyword_table.insert(
      std::make_pair("or", Token(TokenType::OR, "or", literal{}, 0)));
  this->keyword_table.insert(
      std::make_pair("nor", Token(TokenType::NOR, "nor", literal{}, 0)));
  this->keyword_table.insert(
      std::make_pair("xor", Token(TokenType::XOR, "xor", literal{}, 0)));
  this->keyword_table.insert(
      std::make_pair("xnor", Token(TokenType::XNOR, "xnor", literal{}, 0)));

  literal lit{};
  lit.boolean = false;
  lit.is_bitvector = true;
  lit.bits = {false};
  this->keyword_table.insert(
      std::make_pair("0", Token(TokenType::BOOL, "0", lit, 0)));

  lit.boolean = false;
  lit.is_bitvector = true;
  lit.bits = {false};
  this->keyword_table.insert(
      std::make_pair("false", Token(TokenType::FALSE, "false", lit, 0)));
  this->keyword_table.insert(
      std::make_pair("False", Token(TokenType::FALSE, "False", lit, 0)));

  this->keyword_table.insert(
      std::make_pair("print", Token(TokenType::PRINT, "print", literal{}, 0)));
  this->keyword_table.insert(std::make_pair(
      "return", Token(TokenType::RETURN, "return", literal{}, 0)));
  this->keyword_table.insert(std::make_pair(
      "circuit", Token(TokenType::CIRCUIT, "circuit", literal{}, 0)));

  lit.boolean = true;
  lit.is_bitvector = true;
  lit.bits = {true};
  this->keyword_table.insert(
      std::make_pair("1", Token(TokenType::BOOL, "1", lit, 0)));

  lit.boolean = true;
  lit.is_bitvector = true;
  lit.bits = {true};
  this->keyword_table.insert(
      std::make_pair("true", Token(TokenType::TRUE, "true", lit, 0)));
  this->keyword_table.insert(
      std::make_pair("True", Token(TokenType::TRUE, "True", lit, 0)));

  this->keyword_table.insert(
      std::make_pair("bit", Token(TokenType::BIT, "bit", literal{}, 0)));
  this->keyword_table.insert(std::make_pair(
      "bit_vector", Token(TokenType::BIT_VECTOR, "bit_vector", literal{}, 0)));
}
