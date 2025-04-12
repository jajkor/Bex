#include "Evaluator.h"
#include <iostream>

Evaluator::Evaluator() : environment(std::make_shared<Environment>()) {}

void Evaluator::evaluate(const std::vector<std::shared_ptr<Stmt>> &statements) {
  try {
    for (const auto &stmt : statements) {
      executeStmt(stmt);
    }
  } catch (RuntimeError &error) {
    std::cerr << "[line " << error.token->line
              << "] Runtime Error: " << error.what() << std::endl;
  }
}

literal Evaluator::evaluateExpr(std::shared_ptr<Expr> expr) {
  return *static_cast<literal *>(expr->accept(this));
}

void Evaluator::executeStmt(std::shared_ptr<Stmt> stmt) { stmt->accept(this); }

// Helper methods for boolean operations

literal Evaluator::performNot(const literal &operand) {
  literal result;
  result.is_bitvector = operand.is_bitvector;

  if (operand.is_bitvector) {
    result.bits.resize(operand.bits.size());
    for (size_t i = 0; i < operand.bits.size(); i++) {
      result.bits[i] = !operand.bits[i];
    }

    // If it's a single-bit bitvector, also set the boolean value
    if (operand.bits.size() == 1) {
      result.boolean = !operand.boolean;
    }
  } else {
    result.boolean = !operand.boolean;
  }

  return result;
}

literal Evaluator::performAnd(const std::vector<literal> &operands) {
  // Check if all operands are bit vectors of the same size or bits
  bool allBits = true;
  bool allBitVectors = true;
  size_t vectorSize = 0;

  for (const auto &op : operands) {
    if (op.is_bitvector && op.bits.size() > 1) {
      allBits = false;
      if (vectorSize == 0) {
        vectorSize = op.bits.size();
      } else if (vectorSize != op.bits.size()) {
        throw std::runtime_error(
            "Cannot perform AND on bit vectors of different sizes");
      }
    } else {
      allBitVectors = false;
    }
  }

  literal result;

  if (allBitVectors && !allBits && vectorSize > 0) {
    // Perform bit-vector AND
    result.is_bitvector = true;
    result.bits.resize(vectorSize, true); // Initialize all bits to 1 (true)

    for (const auto &op : operands) {
      if (op.is_bitvector && op.bits.size() > 1) {
        for (size_t i = 0; i < vectorSize; i++) {
          result.bits[i] = result.bits[i] && op.bits[i];
        }
      } else {
        // Single bit value applied to all positions
        bool bitValue = op.is_bitvector ? op.bits[0] : op.boolean;
        for (size_t i = 0; i < vectorSize; i++) {
          result.bits[i] = result.bits[i] && bitValue;
        }
      }
    }
  } else {
    // Perform single-bit AND
    result.is_bitvector = true;
    result.bits.resize(1, true); // Initialize to true
    result.boolean = true;

    for (const auto &op : operands) {
      bool bitValue = op.is_bitvector ? op.bits[0] : op.boolean;
      result.bits[0] = result.bits[0] && bitValue;
      result.boolean = result.boolean && bitValue;
    }
  }

  return result;
}

literal Evaluator::performOr(const std::vector<literal> &operands) {
  // Check if all operands are bit vectors of the same size or bits
  bool allBits = true;
  bool allBitVectors = true;
  size_t vectorSize = 0;

  for (const auto &op : operands) {
    if (op.is_bitvector && op.bits.size() > 1) {
      allBits = false;
      if (vectorSize == 0) {
        vectorSize = op.bits.size();
      } else if (vectorSize != op.bits.size()) {
        throw std::runtime_error(
            "Cannot perform OR on bit vectors of different sizes");
      }
    } else {
      allBitVectors = false;
    }
  }

  literal result;

  if (allBitVectors && !allBits && vectorSize > 0) {
    // Perform bit-vector OR
    result.is_bitvector = true;
    result.bits.resize(vectorSize, false); // Initialize all bits to 0 (false)

    for (const auto &op : operands) {
      if (op.is_bitvector && op.bits.size() > 1) {
        for (size_t i = 0; i < vectorSize; i++) {
          result.bits[i] = result.bits[i] || op.bits[i];
        }
      } else {
        // Single bit value applied to all positions
        bool bitValue = op.is_bitvector ? op.bits[0] : op.boolean;
        for (size_t i = 0; i < vectorSize; i++) {
          result.bits[i] = result.bits[i] || bitValue;
        }
      }
    }
  } else {
    // Perform single-bit OR
    result.is_bitvector = true;
    result.bits.resize(1, false); // Initialize to false
    result.boolean = false;

    for (const auto &op : operands) {
      bool bitValue = op.is_bitvector ? op.bits[0] : op.boolean;
      result.bits[0] = result.bits[0] || bitValue;
      result.boolean = result.boolean || bitValue;
    }
  }

  return result;
}

literal Evaluator::performXor(const literal &left, const literal &right) {
  literal result;

  // Check if both operands are bit vectors of the same size
  if (left.is_bitvector && right.is_bitvector && left.bits.size() > 1 &&
      right.bits.size() > 1) {

    if (left.bits.size() != right.bits.size()) {
      throw std::runtime_error(
          "Cannot perform XOR on bit vectors of different sizes");
    }

    // Perform bit-vector XOR
    result.is_bitvector = true;
    result.bits.resize(left.bits.size());

    for (size_t i = 0; i < left.bits.size(); i++) {
      result.bits[i] = left.bits[i] != right.bits[i];
    }
  } else {
    // Perform single-bit XOR
    bool leftBit = left.is_bitvector ? left.bits[0] : left.boolean;
    bool rightBit = right.is_bitvector ? right.bits[0] : right.boolean;

    result.is_bitvector = true;
    result.bits.resize(1);
    result.bits[0] = leftBit != rightBit;
    result.boolean = leftBit != rightBit;
  }

  return result;
}

literal Evaluator::performXnor(const literal &left, const literal &right) {
  literal result;

  // Check if both operands are bit vectors of the same size
  if (left.is_bitvector && right.is_bitvector && left.bits.size() > 1 &&
      right.bits.size() > 1) {

    if (left.bits.size() != right.bits.size()) {
      throw std::runtime_error(
          "Cannot perform XNOR on bit vectors of different sizes");
    }

    // Perform bit-vector XNOR
    result.is_bitvector = true;
    result.bits.resize(left.bits.size());

    for (size_t i = 0; i < left.bits.size(); i++) {
      result.bits[i] = left.bits[i] == right.bits[i];
    }
  } else {
    // Perform single-bit XNOR
    bool leftBit = left.is_bitvector ? left.bits[0] : left.boolean;
    bool rightBit = right.is_bitvector ? right.bits[0] : right.boolean;

    result.is_bitvector = true;
    result.bits.resize(1);
    result.bits[0] = leftBit == rightBit;
    result.boolean = leftBit == rightBit;
  }

  return result;
}

literal Evaluator::performNand(const literal &left, const literal &right) {
  literal result;

  // Check if both operands are bit vectors of the same size
  if (left.is_bitvector && right.is_bitvector && left.bits.size() > 1 &&
      right.bits.size() > 1) {

    if (left.bits.size() != right.bits.size()) {
      throw std::runtime_error(
          "Cannot perform NAND on bit vectors of different sizes");
    }

    // Perform bit-vector NAND
    result.is_bitvector = true;
    result.bits.resize(left.bits.size());

    for (size_t i = 0; i < left.bits.size(); i++) {
      result.bits[i] = !(left.bits[i] && right.bits[i]);
    }
  } else {
    // Perform single-bit NAND
    bool leftBit = left.is_bitvector ? left.bits[0] : left.boolean;
    bool rightBit = right.is_bitvector ? right.bits[0] : right.boolean;

    result.is_bitvector = true;
    result.bits.resize(1);
    result.bits[0] = !(leftBit && rightBit);
    result.boolean = !(leftBit && rightBit);
  }

  return result;
}

literal Evaluator::performNor(const literal &left, const literal &right) {
  literal result;

  // Check if both operands are bit vectors of the same size
  if (left.is_bitvector && right.is_bitvector && left.bits.size() > 1 &&
      right.bits.size() > 1) {

    if (left.bits.size() != right.bits.size()) {
      throw std::runtime_error(
          "Cannot perform NOR on bit vectors of different sizes");
    }

    // Perform bit-vector NOR
    result.is_bitvector = true;
    result.bits.resize(left.bits.size());

    for (size_t i = 0; i < left.bits.size(); i++) {
      result.bits[i] = !(left.bits[i] || right.bits[i]);
    }
  } else {
    // Perform single-bit NOR
    bool leftBit = left.is_bitvector ? left.bits[0] : left.boolean;
    bool rightBit = right.is_bitvector ? right.bits[0] : right.boolean;

    result.is_bitvector = true;
    result.bits.resize(1);
    result.bits[0] = !(leftBit || rightBit);
    result.boolean = !(leftBit || rightBit);
  }

  return result;
}

// Helper for circuit calls
literal Evaluator::executeCircuitCall(const std::shared_ptr<Token> &name,
                                      const std::vector<literal> &arguments) {
  // Get the circuit definition
  std::shared_ptr<CircuitDefStmt> circuit = environment->getCircuit(name);

  // Create a new environment for the circuit execution
  std::shared_ptr<Environment> circuitEnv =
      std::make_shared<Environment>(environment);

  // Bind arguments to parameters
  if (circuit->parameters.size() != arguments.size()) {
    throw RuntimeError(name, "Expected " +
                                 std::to_string(circuit->parameters.size()) +
                                 " arguments but got " +
                                 std::to_string(arguments.size()) + ".");
  }

  for (size_t i = 0; i < circuit->parameters.size(); i++) {
    circuitEnv->define(circuit->parameters[i]->lexeme, arguments[i]);
  }

  // Execute the circuit body
  std::shared_ptr<Environment> previous = environment;
  environment = circuitEnv;

  literal result;
  result.is_bitvector = true;
  result.bits = {false};
  result.boolean = false;

  try {
    // Execute each expression in the circuit body
    for (size_t i = 0; i < circuit->body.size(); i++) {
      auto expr = circuit->body[i];
      result = evaluateExpr(expr);
    }
  } catch (...) {
    // Restore the environment before re-throwing
    environment = previous;
    throw;
  }

  // Restore the environment
  environment = previous;

  return result;
}

// Type checking and error handling
bool Evaluator::isBit(const literal &value) const {
  return value.is_bitvector && value.bits.size() == 1;
}

bool Evaluator::isBitVector(const literal &value) const {
  return value.is_bitvector && value.bits.size() > 1;
}

void Evaluator::checkBitOperand(const std::shared_ptr<Token> &op,
                                const literal &operand) {
  if (!isBit(operand)) {
    throw RuntimeError(op, "Operand must be a bit.");
  }
}

void Evaluator::checkBitVectorOperand(const std::shared_ptr<Token> &op,
                                      const literal &operand) {
  if (!isBitVector(operand)) {
    throw RuntimeError(op, "Operand must be a bit vector.");
  }
}

// ExprVisitor implementation
void *Evaluator::visitLiteralExpr(LiteralExpr *expr) {
  return new literal(expr->value);
}

void *Evaluator::visitVariableExpr(VariableExpr *expr) {
  // Check if this is a circuit call
  if (environment->circuitExists(expr->name->lexeme)) {
    // This is a circuit call
    // For now, we'll treat it as a variable
    // In a complete implementation, we would collect arguments
    // and call executeCircuitCall
    std::vector<literal> arguments;

    // For testing purposes, provide dummy arguments
    literal dummy;
    dummy.is_bitvector = true;
    dummy.bits = {true};
    dummy.boolean = true;

    arguments.push_back(dummy);

    return new literal(executeCircuitCall(expr->name, arguments));
  }

  // Normal variable reference
  literal value = environment->get(expr->name);
  return new literal(value);
}

void *Evaluator::visitUnaryExpr(UnaryExpr *expr) {
  literal right = evaluateExpr(expr->right);

  if (expr->op->type == TokenType::NOT) {
    return new literal(performNot(right));
  }

  throw RuntimeError(expr->op, "Unknown unary operator.");
}

void *Evaluator::visitBinaryExpr(BinaryExpr *expr) {
  literal left = evaluateExpr(expr->left);
  literal right = evaluateExpr(expr->right);

  switch (expr->op->type) {
  case TokenType::XOR:
    return new literal(performXor(left, right));
  case TokenType::XNOR:
    return new literal(performXnor(left, right));
  case TokenType::NAND:
    return new literal(performNand(left, right));
  case TokenType::NOR:
    return new literal(performNor(left, right));
  default:
    throw RuntimeError(expr->op, "Unknown binary operator.");
  }
}

void *Evaluator::visitMultiExpr(MultiExpr *expr) {
  std::vector<literal> operands;

  for (const auto &operand : expr->operands) {
    operands.push_back(evaluateExpr(operand));
  }

  switch (expr->op->type) {
  case TokenType::AND:
    return new literal(performAnd(operands));
  case TokenType::OR:
    return new literal(performOr(operands));
  default:
    throw RuntimeError(expr->op, "Unknown multi-operand operator.");
  }
}

void *Evaluator::visitGroupingExpr(GroupingExpr *expr) {
  return new literal(evaluateExpr(expr->expression));
}

void *Evaluator::visitCallExpr(CallExpr *expr) {
  // Evaluate all arguments
  std::vector<literal> arguments;
  for (const auto &arg : expr->arguments) {
    arguments.push_back(evaluateExpr(arg));
  }

  // Execute the circuit call
  return new literal(executeCircuitCall(expr->callee, arguments));
}

// StmtVisitor implementation
void *Evaluator::visitExpressionStmt(ExpressionStmt *stmt) {
  evaluateExpr(stmt->expression);
  return nullptr;
}

void *Evaluator::visitCircuitDefStmt(CircuitDefStmt *stmt) {
  environment->defineCircuit(
      stmt->name->lexeme,
      std::dynamic_pointer_cast<CircuitDefStmt>(std::shared_ptr<Stmt>(
          stmt, [](Stmt *) {}))); // Create a non-owning shared_ptr
  return nullptr;
}

void *Evaluator::visitBitDefStmt(BitDefStmt *stmt) {
  literal value = evaluateExpr(stmt->initializer);

  // Ensure the value is a bit
  if (!value.is_bitvector || value.bits.size() != 1) {
    throw RuntimeError(stmt->name, "Bit definition requires a bit value.");
  }

  environment->define(stmt->name->lexeme, value);
  return nullptr;
}

void *Evaluator::visitBitVectorDefStmt(BitVectorDefStmt *stmt) {
  literal result;
  result.is_bitvector = true;

  // Evaluate each value in the bit vector
  for (const auto &expr : stmt->values) {
    literal value = evaluateExpr(expr);

    // If it's a bit, add its value
    if (value.is_bitvector && value.bits.size() == 1) {
      result.bits.push_back(value.bits[0]);
    }
    // If it's a bit vector, append all its bits
    else if (value.is_bitvector && value.bits.size() > 1) {
      for (bool bit : value.bits) {
        result.bits.push_back(bit);
      }
    } else {
      throw RuntimeError(stmt->name, "Invalid value in bit vector definition.");
    }
  }

  environment->define(stmt->name->lexeme, result);
  return nullptr;
}

void *Evaluator::visitPrintStmt(PrintStmt *stmt) {
  literal value = evaluateExpr(stmt->expression);

  if (value.is_bitvector) {
    if (value.bits.size() == 1) {
      std::cout << (value.bits[0] ? "true" : "false") << std::endl;
    } else {
      std::cout << "0b";
      for (bool bit : value.bits) {
        std::cout << (bit ? "1" : "0");
      }
      std::cout << std::endl;
    }
  } else {
    std::cout << (value.boolean ? "true" : "false") << std::endl;
  }

  return nullptr;
}

void *Evaluator::visitReturnStmt(ReturnStmt *stmt) {
  // For now, just evaluate the expression
  // In a more complete implementation, we would need to handle this differently
  return new literal(evaluateExpr(stmt->value));
}
