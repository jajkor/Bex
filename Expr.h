#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Token.h"

// Forward declarations
class Stmt;
class Expr;
class LiteralExpr;
class VariableExpr;
class UnaryExpr;
class BinaryExpr;
class MultiExpr;
class GroupingExpr;
class CallExpr; // Added CallExpr

// Visitor pattern for expressions
class ExprVisitor {
public:
  virtual ~ExprVisitor() = default;

  virtual void *visitLiteralExpr(LiteralExpr *expr) = 0;
  virtual void *visitVariableExpr(VariableExpr *expr) = 0;
  virtual void *visitUnaryExpr(UnaryExpr *expr) = 0;
  virtual void *visitBinaryExpr(BinaryExpr *expr) = 0;
  virtual void *visitMultiExpr(MultiExpr *expr) = 0;
  virtual void *visitGroupingExpr(GroupingExpr *expr) = 0;
  virtual void *visitCallExpr(CallExpr *expr) = 0; // Added CallExpr visitor
};

// Base expression class
class Expr {
public:
  virtual ~Expr() = default;
  virtual void *accept(ExprVisitor *visitor) = 0;
};

// Literal expression (true, false, etc.)
class LiteralExpr : public Expr {
public:
  literal value;

  LiteralExpr(literal value) : value(value) {}

  void *accept(ExprVisitor *visitor) override {
    return visitor->visitLiteralExpr(this);
  }
};

// Variable reference (identifier)
class VariableExpr : public Expr {
public:
  std::shared_ptr<Token> name;

  VariableExpr(std::shared_ptr<Token> name) : name(name) {}

  void *accept(ExprVisitor *visitor) override {
    return visitor->visitVariableExpr(this);
  }
};

// Call expression (circuit call)
class CallExpr : public Expr {
public:
  std::shared_ptr<Token> callee;
  std::vector<std::shared_ptr<Expr>> arguments;

  CallExpr(std::shared_ptr<Token> callee,
           std::vector<std::shared_ptr<Expr>> arguments)
      : callee(callee), arguments(arguments) {}

  void *accept(ExprVisitor *visitor) override {
    return visitor->visitCallExpr(this);
  }
};

// Unary operations (not)
class UnaryExpr : public Expr {
public:
  std::shared_ptr<Token> op;
  std::shared_ptr<Expr> right;

  UnaryExpr(std::shared_ptr<Token> op, std::shared_ptr<Expr> right)
      : op(op), right(right) {}

  void *accept(ExprVisitor *visitor) override {
    return visitor->visitUnaryExpr(this);
  }
};

// Binary operations (xor, xnor, nand, nor)
class BinaryExpr : public Expr {
public:
  std::shared_ptr<Token> op;
  std::shared_ptr<Expr> left;
  std::shared_ptr<Expr> right;

  BinaryExpr(std::shared_ptr<Token> op, std::shared_ptr<Expr> left,
             std::shared_ptr<Expr> right)
      : op(op), left(left), right(right) {}

  void *accept(ExprVisitor *visitor) override {
    return visitor->visitBinaryExpr(this);
  }
};

// Multi-operand operations (and, or)
class MultiExpr : public Expr {
public:
  std::shared_ptr<Token> op;
  std::vector<std::shared_ptr<Expr>> operands;

  MultiExpr(std::shared_ptr<Token> op,
            std::vector<std::shared_ptr<Expr>> operands)
      : op(op), operands(operands) {}

  void *accept(ExprVisitor *visitor) override {
    return visitor->visitMultiExpr(this);
  }
};

// Grouping expression (parenthesized expressions)
class GroupingExpr : public Expr {
public:
  std::shared_ptr<Expr> expression;

  GroupingExpr(std::shared_ptr<Expr> expression) : expression(expression) {}

  void *accept(ExprVisitor *visitor) override {
    return visitor->visitGroupingExpr(this);
  }
};
