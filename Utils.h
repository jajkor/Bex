
#pragma once

#include "AstPrinter.h"
#include "Stmt.h"
#include "Token.h"
#include <memory>
#include <string>
#include <vector>

// Function declarations
std::string debugTokenTypeToString(TokenType type);
void printTokenStream(const std::vector<std::shared_ptr<Token>> &tokens);
void printParseResults(const std::vector<std::shared_ptr<Stmt>> &statements);
