
#include "Utils.h"
#include <iostream>

// A helper function to convert TokenType to string for debugging
std::string debugTokenTypeToString(TokenType type) {
  switch (type) {
  case TokenType::LEFT_PAREN:
    return "LEFT_PAREN";
  case TokenType::RIGHT_PAREN:
    return "RIGHT_PAREN";
  case TokenType::IDENTIFIER:
    return "IDENTIFIER";
  case TokenType::BOOL:
    return "BOOL";
  case TokenType::NUMBER:
    return "NUMBER";
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

// Function to print the token stream after scanning
void printTokenStream(const std::vector<std::shared_ptr<Token>> &tokens) {
  std::cout << "=== TOKEN STREAM ===" << '\n';
  for (size_t i = 0; i < tokens.size(); i++) {
    auto token = tokens[i];
    std::cout << i << ": [" << token->line << "] "
              << debugTokenTypeToString(token->type) << " '" << token->lexeme
              << "'" << '\n';
  }
  std::cout << "====================" << std::endl;
}

void printParseResults(const std::vector<std::shared_ptr<Stmt>> &statements) {
  std::cout << "=== PARSE STREAM ===" << '\n'
            << statements.size() << " statements" << std::endl;

  AstPrinter printer;
  for (int i = 0; i < statements.size(); i++) {
    std::string result = printer.print(statements[i]);
    std::cout << i + 1 << ": " << result << '\n';
  }
  std::cout << "====================" << std::endl;
}
