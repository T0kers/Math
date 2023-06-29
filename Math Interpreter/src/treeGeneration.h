#pragma once

#include "tree/Tree.h"

std::unique_ptr<Number> findNumber(const std::string& input, size_t& index);

int8_t operationImportance(Op c);
int8_t operationImportance(std::string ch);
int8_t operationImportance(char ch);

std::unique_ptr<Expr> findPart(const std::string& input, size_t& index, Op Operation);

std::unique_ptr<Expr> generateTree(const std::string& input, size_t& index, size_t end);