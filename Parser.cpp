#include "Parser.h"
#include <iostream>

Parser::Parser(const std::vector<std::shared_ptr<Token>> &tokens)
    : tokens(tokens), current(0) {}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
  std::vector<std::shared_ptr<Stmt>> statements;

  // Parse statements until we reach the end of the file
  while (!isAtEnd()) {
    try {
      // Skip any tokens that might be causing issues
      while (!isAtEnd() && !check(TokenType::LEFT_PAREN)) {
        advance();
      }

      if (!isAtEnd()) {
        statements.push_back(statement());
      }
    } catch (ParseError &error) {
      // Report the error and try to recover
      synchronize();
    }
  }

  return statements;
}

// Utility methods
std::shared_ptr<Token> Parser::peek() { return tokens[current]; }

std::shared_ptr<Token> Parser::previous() { return tokens[current - 1]; }

bool Parser::isAtEnd() { return peek()->type == TokenType::ENDOFFILE; }

std::shared_ptr<Token> Parser::advance() {
  if (!isAtEnd())
    current++;
  return previous();
}

bool Parser::check(TokenType type) {
  if (isAtEnd())
    return false;
  return peek()->type == type;
}

bool Parser::match(TokenType type) {
  if (check(type)) {
    advance();
    return true;
  }
  return false;
}

bool Parser::match(std::initializer_list<TokenType> types) {
  for (auto type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }
  return false;
}

std::shared_ptr<Token> Parser::consume(TokenType type,
                                       const std::string &message) {
  if (check(type))
    return advance();

  throw error(peek(), message);
}

ParseError Parser::error(std::shared_ptr<Token> token,
                         const std::string &message) {
  std::cerr << "[line " << token->line << "] Error";

  if (token->type == TokenType::ENDOFFILE) {
    std::cerr << " at end";
  } else {
    std::cerr << " at '" << token->lexeme << "'";
  }

  std::cerr << ": " << message << std::endl;

  return ParseError(message);
}

void Parser::synchronize() {
  advance();

  while (!isAtEnd()) {
    if (previous()->type == TokenType::RIGHT_PAREN)
      return;

    switch (peek()->type) {
    case TokenType::CIRCUIT:
    case TokenType::BIT:
    case TokenType::BIT_VECTOR:
    case TokenType::PRINT:
    case TokenType::RETURN:
    case TokenType::LEFT_PAREN:
      return;
    default:
      break;
    }

    advance();
  }
}

// Grammar rules
std::vector<std::shared_ptr<Stmt>> Parser::program() {
  std::vector<std::shared_ptr<Stmt>> statements;

  while (!isAtEnd()) {
    try {
      statements.push_back(statement());
    } catch (ParseError &error) {
      synchronize();
    }
  }

  return statements;
}

std::shared_ptr<Stmt> Parser::statement() {
  // Skip any whitespace or unexpected tokens
  while (!isAtEnd() && !check(TokenType::LEFT_PAREN)) {
    advance();
  }

  // All statements start with a left parenthesis in this language
  if (isAtEnd() || !match(TokenType::LEFT_PAREN)) {
    throw error(peek(), "Expected '(' at the start of a statement.");
  }

  // Determine the type of statement based on the next token
  if (match(TokenType::BIT)) {
    return bitDef();
  } else if (match(TokenType::BIT_VECTOR)) {
    return bitVectorDef();
  } else if (match(TokenType::CIRCUIT)) {
    return circuitDef();
  } else if (match(TokenType::PRINT)) {
    return printStatement();
  } else if (match(TokenType::RETURN)) {
    return returnStatement();
  } else {
    // It's an expression statement
    current--; // Move back to allow the expression parser to see the left paren
    return expressionStatement();
  }
}

std::shared_ptr<Stmt> Parser::definition() {
  // Token type was already consumed in statement()
  TokenType defType = previous()->type;

  if (defType == TokenType::CIRCUIT) {
    return circuitDef();
  } else if (defType == TokenType::BIT) {
    return bitDef();
  } else if (defType == TokenType::BIT_VECTOR) {
    return bitVectorDef();
  }

  throw error(previous(), "Expected definition type.");
}

// Fixed circuit definition implementation
std::shared_ptr<Stmt> Parser::circuitDef() {
  // 'circuit' token already consumed
  std::shared_ptr<Token> name =
      consume(TokenType::IDENTIFIER, "Expected circuit name.");

  // Parse parameters
  consume(TokenType::LEFT_PAREN, "Expected '(' after circuit name.");

  std::vector<std::shared_ptr<Token>> parameters;
  if (!check(TokenType::RIGHT_PAREN)) {
    do {
      parameters.push_back(
          consume(TokenType::IDENTIFIER, "Expected parameter name."));
    } while (!check(TokenType::RIGHT_PAREN) && !isAtEnd());
  }

  consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters.");

  std::vector<std::shared_ptr<Expr>> body;

  // Parse the body statements, but don't require a closing parenthesis for each
  // circuit
  while (!check(TokenType::RIGHT_PAREN) && !isAtEnd()) {
    try {
      if (check(TokenType::LEFT_PAREN)) {
        // Found a statement
        std::shared_ptr<Stmt> stmt = statement();

        // Add expressions and return values to the body
        if (auto exprStmt = std::dynamic_pointer_cast<ExpressionStmt>(stmt)) {
          body.push_back(exprStmt->expression);
        } else if (auto returnStmt =
                       std::dynamic_pointer_cast<ReturnStmt>(stmt)) {
          body.push_back(returnStmt->value);
        } else if (auto bitDefStmt =
                       std::dynamic_pointer_cast<BitDefStmt>(stmt)) {
          // For bit definitions in circuits, we'd store them in an environment
          // but for now we'll just add them as expressions
          body.push_back(std::make_shared<VariableExpr>(bitDefStmt->name));
        }
      } else {
        // Skip unexpected tokens
        advance();
      }
    } catch (ParseError &error) {
      // More lenient error recovery for circuit bodies
      while (!isAtEnd() && !check(TokenType::LEFT_PAREN) &&
             !check(TokenType::RIGHT_PAREN)) {
        advance();
      }
    }
  }

  // Only consume the closing parenthesis if it exists
  if (check(TokenType::RIGHT_PAREN)) {
    consume(TokenType::RIGHT_PAREN, "Expected ')' after circuit definition.");
  } else if (isAtEnd()) {
    // Reached end of file without a closing parenthesis
    std::cerr << "Warning: Missing closing parenthesis for circuit '"
              << name->lexeme << "'" << std::endl;
  }

  return std::make_shared<CircuitDefStmt>(name, parameters, body);
}

std::shared_ptr<Stmt> Parser::bitDef() {
  // 'bit' token already consumed
  std::shared_ptr<Token> name =
      consume(TokenType::IDENTIFIER, "Expected bit name.");

  // Parse the initializer
  std::shared_ptr<Expr> initializer;

  // If the next token is a literal or identifier, parse it as the initializer
  if (check(TokenType::TRUE) || check(TokenType::FALSE) ||
      check(TokenType::BOOL) || check(TokenType::IDENTIFIER)) {
    initializer = expression();
  } else if (check(TokenType::LEFT_PAREN)) {
    // Handle nested expressions
    initializer = expression();
  } else {
    throw error(peek(), "Expected expression for bit initializer.");
  }

  consume(TokenType::RIGHT_PAREN, "Expected ')' after bit definition.");

  return std::make_shared<BitDefStmt>(name, initializer);
}

std::shared_ptr<Stmt> Parser::bitVectorDef() {
  // 'bit_vector' token already consumed

  // Check if there's an identifier
  if (check(TokenType::IDENTIFIER)) {
    std::shared_ptr<Token> name =
        consume(TokenType::IDENTIFIER, "Expected bit_vector name.");

    std::vector<std::shared_ptr<Expr>> values;

    // Parse one or more expressions
    do {
      values.push_back(expression());
    } while (!check(TokenType::RIGHT_PAREN) && !isAtEnd());

    consume(TokenType::RIGHT_PAREN,
            "Expected ')' after bit_vector definition.");

    return std::make_shared<BitVectorDefStmt>(name, values);
  } else if (check(TokenType::BIT_VECTOR) || check(TokenType::BOOL)) {
    // This is a literal bit vector like 0b0101
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::RIGHT_PAREN,
            "Expected ')' after bit_vector definition.");

    // Since we don't have a name, we'll create one
    struct literal empty_lit;
    empty_lit.boolean = false;
    empty_lit.is_bitvector = false;

    std::shared_ptr<Token> emptyName = std::make_shared<Token>(
        TokenType::IDENTIFIER, "", empty_lit, peek()->line);

    std::vector<std::shared_ptr<Expr>> vec_values = {expr};
    return std::make_shared<BitVectorDefStmt>(emptyName, vec_values);
  }

  throw error(peek(),
              "Expected identifier or bit vector literal after 'bit_vector'.");
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
  std::shared_ptr<Expr> expr = expression();

  // Check if we need to consume the closing parenthesis
  if (previous()->type == TokenType::LEFT_PAREN) {
    consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
  }

  return std::make_shared<ExpressionStmt>(expr);
}

std::shared_ptr<Stmt> Parser::printStatement() {
  // 'print' token already consumed
  std::shared_ptr<Expr> value = expression();

  consume(TokenType::RIGHT_PAREN, "Expected ')' after print statement.");

  return std::make_shared<PrintStmt>(value);
}

std::shared_ptr<Stmt> Parser::returnStatement() {
  // 'return' token already consumed

  // The return statement can have either an identifier or an expression
  std::shared_ptr<Expr> value;

  if (check(TokenType::LEFT_PAREN)) {
    // It's a parenthesized expression
    value = expression();
  } else if (check(TokenType::IDENTIFIER)) {
    // It's a variable reference
    advance();
    value = std::make_shared<VariableExpr>(previous());
  } else {
    throw error(peek(), "Expected expression or identifier after 'return'.");
  }

  consume(TokenType::RIGHT_PAREN, "Expected ')' after return statement.");

  return std::make_shared<ReturnStmt>(value);
}

std::shared_ptr<Expr> Parser::expression() {
  if (check(TokenType::TRUE) || check(TokenType::FALSE) ||
      check(TokenType::BOOL) || check(TokenType::BIT_VECTOR)) {
    advance(); // Consume the token
    return literal();
  }

  if (check(TokenType::IDENTIFIER)) {
    advance(); // Consume the identifier
    return variableRef();
  }

  if (check(TokenType::LEFT_PAREN)) {
    advance(); // Consume '('

    // Check if it's an operation
    if (check(TokenType::NOT) || check(TokenType::AND) ||
        check(TokenType::OR) || check(TokenType::NAND) ||
        check(TokenType::NOR) || check(TokenType::XOR) ||
        check(TokenType::XNOR)) {

      TokenType opType = peek()->type;
      advance(); // Consume operation token

      if (opType == TokenType::NOT) {
        // Unary operation
        std::shared_ptr<Token> op = previous();
        std::shared_ptr<Expr> right = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after 'not' expression.");
        return std::make_shared<UnaryExpr>(op, right);
      } else if (opType == TokenType::AND || opType == TokenType::OR) {
        // Multi-operand operation
        std::shared_ptr<Token> op = previous();
        std::vector<std::shared_ptr<Expr>> operands;

        while (!check(TokenType::RIGHT_PAREN) && !isAtEnd()) {
          operands.push_back(expression());
        }

        consume(TokenType::RIGHT_PAREN,
                "Expected ')' after multi-operand expression.");
        return std::make_shared<MultiExpr>(op, operands);
      } else {
        // Binary operation
        std::shared_ptr<Token> op = previous();
        std::shared_ptr<Expr> left = expression();
        std::shared_ptr<Expr> right = expression();
        consume(TokenType::RIGHT_PAREN,
                "Expected ')' after binary expression.");
        return std::make_shared<BinaryExpr>(op, left, right);
      }
    } else if (check(TokenType::IDENTIFIER)) {
      // It's a function call like (HALF_ADDER A B)
      std::shared_ptr<Token> funcName =
          consume(TokenType::IDENTIFIER, "Expected function name.");

      // Parse arguments
      std::vector<std::shared_ptr<Expr>> args;
      while (!check(TokenType::RIGHT_PAREN) && !isAtEnd()) {
        if (check(TokenType::IDENTIFIER)) {
          advance(); // Consume identifier
          args.push_back(std::make_shared<VariableExpr>(previous()));
        } else if (check(TokenType::LEFT_PAREN)) {
          args.push_back(expression());
        } else if (check(TokenType::TRUE) || check(TokenType::FALSE) ||
                   check(TokenType::BOOL) || check(TokenType::BIT_VECTOR)) {
          advance(); // Consume token
          args.push_back(literal());
        } else {
          // Skip invalid tokens
          advance();
        }
      }

      consume(TokenType::RIGHT_PAREN, "Expected ')' after function arguments.");

      // Create a call expression
      return std::make_shared<CallExpr>(funcName, args);
    }

    // It's a grouped expression
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
    return std::make_shared<GroupingExpr>(expr);
  }

  throw error(peek(), "Expected expression.");
}

std::shared_ptr<Expr> Parser::literal() {
  // TRUE, FALSE, BOOL or BIT_VECTOR token already consumed
  struct literal lit_value;

  if (previous()->type == TokenType::TRUE) {
    lit_value.boolean = true;
    lit_value.is_bitvector = true;
    lit_value.bits = {true};
  } else if (previous()->type == TokenType::FALSE) {
    lit_value.boolean = false;
    lit_value.is_bitvector = true;
    lit_value.bits = {false};
  } else if (previous()->type == TokenType::BIT_VECTOR) {
    // Handle bit vector literal
    lit_value = previous()->lit;
  } else {
    // It's a BOOL token (0, 1, or bit vector)
    lit_value = previous()->lit;
  }

  return std::make_shared<LiteralExpr>(lit_value);
}

std::shared_ptr<Expr> Parser::variableRef() {
  // IDENTIFIER token already consumed
  std::shared_ptr<Token> name = previous();

  // Check if this is a function call
  if (check(TokenType::LEFT_PAREN)) {
    advance(); // Consume the left parenthesis

    std::vector<std::shared_ptr<Expr>> arguments;

    // Parse arguments
    while (!check(TokenType::RIGHT_PAREN) && !isAtEnd()) {
      if (check(TokenType::IDENTIFIER)) {
        advance(); // Consume the identifier
        arguments.push_back(std::make_shared<VariableExpr>(previous()));
      } else if (check(TokenType::LEFT_PAREN)) {
        arguments.push_back(expression());
      } else if (check(TokenType::TRUE) || check(TokenType::FALSE) ||
                 check(TokenType::BOOL) || check(TokenType::BIT_VECTOR)) {
        advance(); // Consume token
        arguments.push_back(literal());
      } else {
        // Skip invalid tokens
        advance();
      }
    }

    consume(TokenType::RIGHT_PAREN, "Expected ')' after function arguments.");

    // Create a call expression
    return std::make_shared<CallExpr>(name, arguments);
  }

  return std::make_shared<VariableExpr>(name);
}

std::shared_ptr<Expr> Parser::operation() {
  // Operation token is already consumed or is about to be consumed
  if (check(TokenType::NOT)) {
    advance(); // Consume the NOT token
    return unaryOp();
  } else if (check(TokenType::XOR) || check(TokenType::XNOR) ||
             check(TokenType::NAND) || check(TokenType::NOR)) {
    advance(); // Consume the operator token
    return binaryOp();
  } else if (check(TokenType::AND) || check(TokenType::OR)) {
    advance(); // Consume the operator token
    return multiOp();
  } else if (previous()->type == TokenType::NOT) {
    // NOT was already consumed
    return unaryOp();
  } else if (previous()->type == TokenType::XOR ||
             previous()->type == TokenType::XNOR ||
             previous()->type == TokenType::NAND ||
             previous()->type == TokenType::NOR) {
    // Binary operator was already consumed
    return binaryOp();
  } else if (previous()->type == TokenType::AND ||
             previous()->type == TokenType::OR) {
    // Multi operator was already consumed
    return multiOp();
  }

  throw error(peek(), "Expected operation type (not, and, or, etc.).");
}

std::shared_ptr<Expr> Parser::unaryOp() {
  // 'not' token already consumed
  std::shared_ptr<Token> op = previous();
  std::shared_ptr<Expr> right = expression();

  // Only consume the closing parenthesis if we're inside parentheses
  if (current > 0 && tokens[current - 1]->type != TokenType::RIGHT_PAREN &&
      tokens[current - 2]->type != TokenType::RIGHT_PAREN &&
      current < tokens.size() && check(TokenType::RIGHT_PAREN)) {
    consume(TokenType::RIGHT_PAREN, "Expected ')' after 'not' expression.");
  }

  return std::make_shared<UnaryExpr>(op, right);
}

std::shared_ptr<Expr> Parser::binaryOp() {
  // Binary operator token already consumed
  std::shared_ptr<Token> op = previous();

  std::shared_ptr<Expr> left = expression();
  std::shared_ptr<Expr> right = expression();

  // Only consume the closing parenthesis if we're inside parentheses
  if (current > 0 && tokens[current - 1]->type != TokenType::RIGHT_PAREN &&
      tokens[current - 2]->type != TokenType::RIGHT_PAREN &&
      current < tokens.size() && check(TokenType::RIGHT_PAREN)) {
    consume(TokenType::RIGHT_PAREN, "Expected ')' after binary expression.");
  }

  return std::make_shared<BinaryExpr>(op, left, right);
}

std::shared_ptr<Expr> Parser::multiOp() {
  // Multi operator token already consumed
  std::shared_ptr<Token> op = previous();

  std::vector<std::shared_ptr<Expr>> operands;

  // Parse one or more expressions
  do {
    operands.push_back(expression());
  } while (!check(TokenType::RIGHT_PAREN) && !isAtEnd());

  // Only consume the closing parenthesis if we're inside parentheses
  if (current > 0 && tokens[current - 1]->type != TokenType::RIGHT_PAREN &&
      tokens[current - 2]->type != TokenType::RIGHT_PAREN &&
      current < tokens.size() && check(TokenType::RIGHT_PAREN)) {
    consume(TokenType::RIGHT_PAREN,
            "Expected ')' after multi-operand expression.");
  }

  return std::make_shared<MultiExpr>(op, operands);
}
