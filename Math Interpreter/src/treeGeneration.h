#pragma once

#include "tree/Tree.h"

std::unique_ptr<Number> findNumber(const std::string& input, size_t& index);

int8_t operationImportance(Symbol c);
int8_t operationImportance(std::string ch);
int8_t operationImportance(char ch);

std::unique_ptr<Expr> findPart(const std::string& input, size_t& index, Symbol Operation, const std::string& name = "");



// This function 
std::unique_ptr<Expr> generateFunction(const std::string& input, size_t& index, size_t end, const std::string& funcName);
std::unique_ptr<Expr> generateTree(const std::string& input, size_t& index, size_t end);