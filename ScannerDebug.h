
#pragma once

#include "Token.h"
#include <memory>
#include <string>
#include <vector>

// Function declarations
std::string debugTokenTypeToString(TokenType type);
void printTokenStream(const std::vector<std::shared_ptr<Token>> &tokens);
