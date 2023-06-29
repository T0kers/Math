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

enum class Op {
    ERROR = -1, PLUS = 0, MINUS, MULTIPLY, DIVIDE, POWER, lPAREN, rPAREN, ASSIGN, EQUAL
};

extern std::unordered_map<Op, std::string> opToStr;
extern std::unordered_map<std::string, Op> strToOp;

bool isOpEqual(char letter, Op oper);

std::string operator+(const std::string& str, Op Op);

class Expr {
public:
    virtual std::unique_ptr<Expr> clone() const = 0;

    virtual std::unique_ptr<Expr> approximate() = 0;
    virtual std::unique_ptr<Expr> approximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) = 0;

    virtual std::unique_ptr<Expr> evaluate() = 0;
    virtual std::unique_ptr<Expr> evaluate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) = 0;

    virtual std::string getInfo() const = 0;
};

std::ostream& operator<<(std::ostream& out, Expr const& data);