#pragma once

#include <memory>
#include <stdexcept> // Add this include for std::runtime_error
#include <string>
#include <unordered_map>

#include "Expr.h"
#include "Stmt.h"
#include "Token.h"

class RuntimeError : public std::runtime_error {
public:
  std::shared_ptr<Token> token;

  RuntimeError(std::shared_ptr<Token> token, const std::string &message)
      : std::runtime_error(message), token(token) {}
};

class Environment {
private:
  std::unordered_map<std::string, literal> values;
  std::unordered_map<std::string, std::shared_ptr<CircuitDefStmt>> circuits;
  std::shared_ptr<Environment> enclosing;

public:
  Environment() : enclosing(nullptr) {}
  Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {}

  void define(const std::string &name, const literal &value);
  void defineCircuit(const std::string &name,
                     std::shared_ptr<CircuitDefStmt> circuit);

  literal get(const std::shared_ptr<Token> &name);
  std::shared_ptr<CircuitDefStmt>
  getCircuit(const std::shared_ptr<Token> &name);

  bool exists(const std::string &name) const;
  bool circuitExists(const std::string &name) const;
};
