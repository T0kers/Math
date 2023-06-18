#pragma once

#include "tree.h"

std::unique_ptr<number> findNumber(const std::string& input, size_t& index);

int8_t operationImportance(op c);
int8_t operationImportance(std::string ch);
int8_t operationImportance(char ch);

std::unique_ptr<expr> findPart(const std::string& input, size_t& index, op operation);

std::unique_ptr<expr> generateTree(const std::string& input, size_t& index, size_t end);