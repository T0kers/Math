#pragma once

#include "tree.h"

std::unique_ptr<constant> findNumber(const std::string& input, size_t& index);

int8_t operationImportance(char c);

std::unique_ptr<expr> findPart(const std::string& input, size_t& index, char operation);

std::unique_ptr<expr> generateTree(const std::string& input, size_t& index, size_t end);