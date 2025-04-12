#pragma once

#include "Expr.h"
#include "Stmt.h"
#include "Token.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class ParseError : public std::runtime_error {
public:
  explicit ParseError(const std::string &what_arg)
      : std::runtime_error(what_arg) {}
};

class Parser {
private:
  std::vector<std::shared_ptr<Token>> tokens;
  int current = 0;

  // Utility methods
  std::shared_ptr<Token> peek();
  std::shared_ptr<Token> previous();
  bool isAtEnd();
  std::shared_ptr<Token> advance();
  bool check(TokenType type);
  bool match(TokenType type);
  bool match(std::initializer_list<TokenType> types);
  std::shared_ptr<Token> consume(TokenType type, const std::string &message);
  ParseError error(std::shared_ptr<Token> token, const std::string &message);
  void synchronize();

  // Grammar rules
  std::vector<std::shared_ptr<Stmt>> program();
  std::shared_ptr<Stmt> statement();
  std::shared_ptr<Stmt> definition();
  std::shared_ptr<Stmt> circuitDef();
  std::shared_ptr<Stmt> bitDef();
  std::shared_ptr<Stmt> bitVectorDef();
  std::shared_ptr<Stmt> expressionStatement();
  std::shared_ptr<Stmt> printStatement();
  std::shared_ptr<Stmt> returnStatement();

  std::shared_ptr<Expr> expression();
  std::shared_ptr<Expr> literal();
  std::shared_ptr<Expr> variableRef();
  std::shared_ptr<Expr> functionCall();
  std::shared_ptr<Expr> operation();
  std::shared_ptr<Expr> unaryOp();
  std::shared_ptr<Expr> binaryOp();
  std::shared_ptr<Expr> multiOp();
  std::shared_ptr<Expr> primary();

public:
  Parser(const std::vector<std::shared_ptr<Token>> &tokens);
  std::vector<std::shared_ptr<Stmt>> parse();
};
