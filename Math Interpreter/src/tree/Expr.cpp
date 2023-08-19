#include "Expr.h"

#include "Tree.h"

std::unordered_map<Symbol, std::string> opToStr = {
    {Symbol::PLUS, "+"},
    {Symbol::MINUS, "-"},
    {Symbol::MULTIPLY, "*"},
    {Symbol::DIVIDE, "/"},
    {Symbol::POWER, "^"},
    {Symbol::lPAREN, "("},
    {Symbol::rPAREN, ")"},
    {Symbol::ASSIGN, ":="},
    {Symbol::EQUAL, "="},
    {Symbol::FUNCTION, "()"},
    {Symbol::COMMA, ","}
};

std::unordered_map<std::string, Symbol> strToOp = {
    {"+", Symbol::PLUS},
    {"-", Symbol::MINUS},
    {"*", Symbol::MULTIPLY},
    {"/", Symbol::DIVIDE},
    {"^", Symbol::POWER},
    {"(", Symbol::lPAREN},
    {")", Symbol::rPAREN},
    {":=", Symbol::ASSIGN},
    {"=", Symbol::EQUAL},
    {"()", Symbol::FUNCTION},
    {",", Symbol::COMMA}
};

bool isSymbolEqual(char letter, Symbol oper) {
    return opToStr.at(oper) == std::string(1, letter);
}

std::string operator+(const std::string& str, Symbol oper) {
    return str + opToStr.at(oper);
}

std::ostream& operator<<(std::ostream& out, Expr const& data) {
    out << data.getInfo();
    return out;
}