#include "Environment.h"

void Environment::define(const std::string &name, const literal &value) {
  values[name] = value;
}

void Environment::defineCircuit(const std::string &name,
                                std::shared_ptr<CircuitDefStmt> circuit) {
  circuits[name] = circuit;
}

literal Environment::get(const std::shared_ptr<Token> &name) {
  auto it = values.find(name->lexeme);
  if (it != values.end()) {
    return it->second;
  }

  if (enclosing != nullptr) {
    return enclosing->get(name);
  }

  throw RuntimeError(name, "Undefined variable '" + name->lexeme + "'.");
}

std::shared_ptr<CircuitDefStmt>
Environment::getCircuit(const std::shared_ptr<Token> &name) {
  auto it = circuits.find(name->lexeme);
  if (it != circuits.end()) {
    return it->second;
  }

  if (enclosing != nullptr) {
    return enclosing->getCircuit(name);
  }

  throw RuntimeError(name, "Undefined circuit '" + name->lexeme + "'.");
}

bool Environment::exists(const std::string &name) const {
  if (values.find(name) != values.end()) {
    return true;
  }

  if (enclosing != nullptr) {
    return enclosing->exists(name);
  }

  return false;
}

bool Environment::circuitExists(const std::string &name) const {
  if (circuits.find(name) != circuits.end()) {
    return true;
  }

  if (enclosing != nullptr) {
    return enclosing->circuitExists(name);
  }

  return false;
}
