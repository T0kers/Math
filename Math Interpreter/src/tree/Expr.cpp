#include "Expr.h"

#include "Tree.h"

std::unordered_map<Op, std::string> opToStr = {
    {Op::PLUS, "+"},
    {Op::MINUS, "-"},
    {Op::MULTIPLY, "*"},
    {Op::DIVIDE, "/"},
    {Op::POWER, "^"},
    {Op::lPAREN, "("},
    {Op::rPAREN, ")"},
    {Op::ASSIGN, ":="},
    {Op::EQUAL, "="}
};

std::unordered_map<std::string, Op> strToOp = {
    {"+", Op::PLUS},
    {"-", Op::MINUS},
    {"*", Op::MULTIPLY},
    {"/", Op::DIVIDE},
    {"^", Op::POWER},
    {"(", Op::lPAREN},
    {")", Op::rPAREN},
    {":=", Op::ASSIGN},
    {"=", Op::EQUAL}
};

bool isOpEqual(char letter, Op oper) {
    return opToStr.at(oper) == std::string(1, letter);
}

std::string operator+(const std::string& str, Op oper) {
    return str + opToStr.at(oper);
}

std::ostream& operator<<(std::ostream& out, Expr const& data) {
    out << data.getInfo();
    return out;
}