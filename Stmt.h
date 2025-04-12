#pragma once

#include "Expr.h"
#include "Token.h"
#include <memory>
#include <string>
#include <vector>

// Forward declarations
class StmtVisitor;
class ExpressionStmt;
class CircuitDefStmt;
class BitDefStmt;
class BitVectorDefStmt;
class PrintStmt;
class ReturnStmt;

// Visitor pattern for statements
class StmtVisitor {
public:
  virtual ~StmtVisitor() = default;

  virtual void *visitExpressionStmt(ExpressionStmt *stmt) = 0;
  virtual void *visitCircuitDefStmt(CircuitDefStmt *stmt) = 0;
  virtual void *visitBitDefStmt(BitDefStmt *stmt) = 0;
  virtual void *visitBitVectorDefStmt(BitVectorDefStmt *stmt) = 0;
  virtual void *visitPrintStmt(PrintStmt *stmt) = 0;
  virtual void *visitReturnStmt(ReturnStmt *stmt) = 0;
};

// Base statement class
class Stmt {
public:
  virtual ~Stmt() = default;
  virtual void *accept(StmtVisitor *visitor) = 0;
};

// Expression statement (just an expression)
class ExpressionStmt : public Stmt {
public:
  std::shared_ptr<Expr> expression;

  ExpressionStmt(std::shared_ptr<Expr> expression) : expression(expression) {}

  void *accept(StmtVisitor *visitor) override {
    return visitor->visitExpressionStmt(this);
  }
};

// Circuit definition statement
class CircuitDefStmt : public Stmt {
public:
  std::shared_ptr<Token> name;
  std::vector<std::shared_ptr<Token>> parameters; // Added parameters vector
  std::vector<std::shared_ptr<Expr>> body;

  CircuitDefStmt(
      std::shared_ptr<Token> name,
      std::vector<std::shared_ptr<Token>> parameters, // Added parameters
      std::vector<std::shared_ptr<Expr>> body)
      : name(name), parameters(parameters), body(body) {}

  void *accept(StmtVisitor *visitor) override {
    return visitor->visitCircuitDefStmt(this);
  }
};

// Bit definition statement
class BitDefStmt : public Stmt {
public:
  std::shared_ptr<Token> name;
  std::shared_ptr<Expr> initializer;

  BitDefStmt(std::shared_ptr<Token> name, std::shared_ptr<Expr> initializer)
      : name(name), initializer(initializer) {}

  void *accept(StmtVisitor *visitor) override {
    return visitor->visitBitDefStmt(this);
  }
};

// Bit vector definition statement
class BitVectorDefStmt : public Stmt {
public:
  std::shared_ptr<Token> name;
  std::vector<std::shared_ptr<Expr>> values;

  BitVectorDefStmt(std::shared_ptr<Token> name,
                   std::vector<std::shared_ptr<Expr>> values)
      : name(name), values(values) {}

  void *accept(StmtVisitor *visitor) override {
    return visitor->visitBitVectorDefStmt(this);
  }
};

// Print statement
class PrintStmt : public Stmt {
public:
  std::shared_ptr<Expr> expression;

  PrintStmt(std::shared_ptr<Expr> expression) : expression(expression) {}

  void *accept(StmtVisitor *visitor) override {
    return visitor->visitPrintStmt(this);
  }
};

// Return statement
class ReturnStmt : public Stmt {
public:
  std::shared_ptr<Expr> value;

  ReturnStmt(std::shared_ptr<Expr> value) : value(value) {}

  void *accept(StmtVisitor *visitor) override {
    return visitor->visitReturnStmt(this);
  }
};
