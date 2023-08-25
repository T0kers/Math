#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include <typeinfo>
#include <typeindex>
#include <cassert>
#include <cmath>

enum class Symbol {
    ERROR = -1, PLUS = 0, MINUS, MULTIPLY, DIVIDE, POWER, lPAREN, rPAREN, ASSIGN, EQUAL, FUNCTION, COMMA
};

extern std::unordered_map<Symbol, std::string> opToStr;
extern std::unordered_map<std::string, Symbol> strToOp;

bool isSymbolEqual(char letter, Symbol oper);

std::string operator+(const std::string& str, Symbol Symbol);

class Expr {
public:
    typedef std::map<std::string, std::unique_ptr<Expr>> paramArgMap;
    virtual std::unique_ptr<Expr> clone() const = 0;

    virtual std::unique_ptr<Expr> approximate(const paramArgMap& extraMap) = 0;
    virtual std::unique_ptr<Expr> evaluate(const paramArgMap& extraMap) = 0;

    virtual std::string getInfo() const = 0;
};

std::ostream& operator<<(std::ostream& out, Expr const& data);