#pragma once

#include "Environment.h"
#include "Expr.h"
#include "Stmt.h"
#include <memory>
#include <vector>

class Evaluator : public ExprVisitor, public StmtVisitor {
private:
  std::shared_ptr<Environment> environment;

  // Helper methods for boolean operations
  literal performNot(const literal &operand);
  literal performAnd(const std::vector<literal> &operands);
  literal performOr(const std::vector<literal> &operands);
  literal performXor(const literal &left, const literal &right);
  literal performXnor(const literal &left, const literal &right);
  literal performNand(const literal &left, const literal &right);
  literal performNor(const literal &left, const literal &right);

  // Helper for circuit calls
  literal executeCircuitCall(const std::shared_ptr<Token> &name,
                             const std::vector<literal> &arguments);

  // Type checking and error handling
  bool isBit(const literal &value) const;
  bool isBitVector(const literal &value) const;
  void checkBitOperand(const std::shared_ptr<Token> &op,
                       const literal &operand);
  void checkBitVectorOperand(const std::shared_ptr<Token> &op,
                             const literal &operand);

public:
  Evaluator();

  // Main evaluation methods
  void evaluate(const std::vector<std::shared_ptr<Stmt>> &statements);
  literal evaluateExpr(std::shared_ptr<Expr> expr);
  void executeStmt(std::shared_ptr<Stmt> stmt);

  // ExprVisitor implementation
  void *visitLiteralExpr(LiteralExpr *expr) override;
  void *visitVariableExpr(VariableExpr *expr) override;
  void *visitUnaryExpr(UnaryExpr *expr) override;
  void *visitBinaryExpr(BinaryExpr *expr) override;
  void *visitMultiExpr(MultiExpr *expr) override;
  void *visitGroupingExpr(GroupingExpr *expr) override;
  void *visitCallExpr(CallExpr *expr) override;

  // StmtVisitor implementation
  void *visitExpressionStmt(ExpressionStmt *stmt) override;
  void *visitCircuitDefStmt(CircuitDefStmt *stmt) override;
  void *visitBitDefStmt(BitDefStmt *stmt) override;
  void *visitBitVectorDefStmt(BitVectorDefStmt *stmt) override;
  void *visitPrintStmt(PrintStmt *stmt) override;
  void *visitReturnStmt(ReturnStmt *stmt) override;
};
