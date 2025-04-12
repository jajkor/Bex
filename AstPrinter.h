#pragma once

#include "Expr.h"
#include "Stmt.h"
#include <iostream>
#include <sstream>
#include <string>

class AstPrinter : public ExprVisitor, public StmtVisitor {
public:
  std::string print(std::shared_ptr<Expr> expr) {
    std::string *result = static_cast<std::string *>(expr->accept(this));
    std::string value = *result;
    delete result;
    return value;
  }

  std::string print(std::shared_ptr<Stmt> stmt) {
    std::string *result = static_cast<std::string *>(stmt->accept(this));
    std::string value = *result;
    delete result;
    return value;
  }

  // ExprVisitor methods
  void *visitLiteralExpr(LiteralExpr *expr) override {
    std::stringstream ss;
    if (expr->value.is_bitvector) {
      if (expr->value.bits.size() == 1) {
        ss << (expr->value.bits[0] ? "true" : "false");
      } else {
        ss << "0b";
        for (bool bit : expr->value.bits) {
          ss << (bit ? "1" : "0");
        }
      }
    } else {
      ss << "literal(unknown)";
    }
    return new std::string(ss.str());
  }

  void *visitVariableExpr(VariableExpr *expr) override {
    return new std::string(expr->name->lexeme);
  }

  void *visitUnaryExpr(UnaryExpr *expr) override {
    std::string right = print(expr->right);
    std::stringstream ss;
    ss << "(" << expr->op->lexeme << " " << right << ")";
    return new std::string(ss.str());
  }

  void *visitBinaryExpr(BinaryExpr *expr) override {
    std::string left = print(expr->left);
    std::string right = print(expr->right);
    std::stringstream ss;
    ss << "(" << expr->op->lexeme << " " << left << " " << right << ")";
    return new std::string(ss.str());
  }

  void *visitMultiExpr(MultiExpr *expr) override {
    std::stringstream ss;
    ss << "(" << expr->op->lexeme;

    for (const auto &operand : expr->operands) {
      std::string op = print(operand);
      ss << " " << op;
    }

    ss << ")";
    return new std::string(ss.str());
  }

  void *visitGroupingExpr(GroupingExpr *expr) override {
    std::string inner = print(expr->expression);
    std::stringstream ss;
    ss << "(" << inner << ")";
    return new std::string(ss.str());
  }

  // StmtVisitor methods
  void *visitExpressionStmt(ExpressionStmt *stmt) override {
    std::string expr = print(stmt->expression);
    return new std::string(expr);
  }

  void *visitCircuitDefStmt(CircuitDefStmt *stmt) override {
    std::stringstream ss;
    ss << "(circuit " << stmt->name->lexeme;

    for (const auto &expr : stmt->body) {
      std::string e = print(expr);
      ss << " " << e;
    }

    ss << ")";
    return new std::string(ss.str());
  }

  void *visitBitDefStmt(BitDefStmt *stmt) override {
    std::string init = print(stmt->initializer);
    std::stringstream ss;
    ss << "(bit " << stmt->name->lexeme << " " << init << ")";
    return new std::string(ss.str());
  }

  void *visitBitVectorDefStmt(BitVectorDefStmt *stmt) override {
    std::stringstream ss;
    ss << "(bit_vector " << stmt->name->lexeme;

    for (const auto &expr : stmt->values) {
      std::string e = print(expr);
      ss << " " << e;
    }

    ss << ")";
    return new std::string(ss.str());
  }

  void *visitPrintStmt(PrintStmt *stmt) override {
    std::string expr = print(stmt->expression);
    std::stringstream ss;
    ss << "(print " << expr << ")";
    return new std::string(ss.str());
  }

  void *visitReturnStmt(ReturnStmt *stmt) override {
    std::string expr = print(stmt->value);
    std::stringstream ss;
    ss << "(return " << expr << ")";
    return new std::string(ss.str());
  }
};
