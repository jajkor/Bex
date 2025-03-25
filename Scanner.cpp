#include "Scanner.h"
#include <vector>

bool Scanner::isAtEnd() {
  return this->current >= this->end;
}

std::vector<std::shared_ptr<Token>> Scanner::scanTokens() {
  while (!isAtEnd()) {
    start = current;
    scanToken();
  }

  this->tokens.push_back(std::make_shared<Token>(ENDOFFILE, "", literal{}, line));
  return std::move(this->tokens);
}

bool Scanner::isDigit(char c)
{
	return '0' <= c && c <= '9';
}
bool Scanner::isAlpha(char c)
{
	return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_';
}

bool Scanner::isAldigit(char c)
{
	return isAlpha(c) || isDigit(c);
}

void Scanner::handleIdentifier()
{
	while (!isAtEnd() && isAldigit(peek()))
	{
		this->current++;
	}

  std::string tmp = this->source.substr(this->start, this->current - this->start);
	
	auto iter = this->keyword_table.find(tmp);
	if (iter != this->keyword_table.end())
	{
		iter->second.updateLine(this->line);
		
		//this->tokens.push_back(new Token(iter->second));
    this->tokens.push_back(std::make_shared<Token>(IDENTIFIER, tmp, literal{}, line));
	}
	else
	{
    this->tokens.push_back(std::make_shared<Token>(IDENTIFIER, tmp, literal{}, line));
	}
}

char Scanner::peek()
{
	if (this->current > this->source.length())
		return '\0';

	return this->source[this->current];
}

char Scanner::peekNext()
{
	if (current + 1 >= this->source.length())
		return  '\0';
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

char Scanner::advance() {
  return this->source[this->current++];
}

void Scanner::scanToken() {
  char c = advance();
  switch (c) {
    case '(': 
      this->tokens.push_back(std::make_shared<Token>(LEFT_PAREN, "(", literal{}, line));
      break;
    case ')': 
      this->tokens.push_back(std::make_shared<Token>(RIGHT_PAREN, ")", literal{}, line));
      break;
    case ' ':
      break;
    default:
      if (isAlpha(c)) {
        handleIdentifier();
        break;
      }
      std::cout << "Error: " << c << std::endl;
      break;
  }
}

Scanner::Scanner(std::string source) : source(source), start(0), current(0), line(1) {
  this->end = source.length();

	this->keyword_table.insert(std::make_pair("not", Token(NOT, "not", literal{}, 0)));
	this->keyword_table.insert(std::make_pair("and", Token(AND, "and", literal{}, 0)));
	this->keyword_table.insert(std::make_pair("nand", Token(NAND, "nand", literal{}, 0)));
	this->keyword_table.insert(std::make_pair("or", Token(OR, "or", literal{}, 0)));
	this->keyword_table.insert(std::make_pair("nor", Token(NOR, "nor", literal{}, 0)));
	this->keyword_table.insert(std::make_pair("xor", Token(XOR, "xor", literal{}, 0)));

  literal lit{};
  lit.boolean = false;
	this->keyword_table.insert(std::make_pair("false", Token(FALSE, "false", literal{}, 0)));
	this->keyword_table.insert(std::make_pair("print", Token(PRINT, "print", literal{}, 0)));
	this->keyword_table.insert(std::make_pair("return", Token(RETURN, "return", literal{}, 0)));
	this->keyword_table.insert(std::make_pair("circuit", Token(CIRCUIT, "circuit", literal{}, 0)));

  lit.boolean = true;
	this->keyword_table.insert(std::make_pair("true", Token(TRUE, "true", literal{}, 0)));
  this->keyword_table.insert(std::make_pair("bit", Token(BIT, "bit", literal{}, 0)));
	this->keyword_table.insert(std::make_pair("bit_vector", Token(BIT_VECTOR, "bit_vector", literal{}, 0)));
}
