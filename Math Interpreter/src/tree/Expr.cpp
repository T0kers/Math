#include "Expr.h"

Symbol convertSymbol(std::string name) {
    static std::map<std::string, Symbol> strToOp = {
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
    if (strToOp.count(name)) {
        return strToOp.at(name);
    }
    return Symbol::ERROR;
}

std::string convertSymbol(Symbol sym) {
    static std::map<Symbol, std::string> opToStr = {
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
    if (opToStr.count(sym)) {
        return opToStr.at(sym);
    }
    return std::string();
}

bool isSymbolEqual(char letter, Symbol oper) {
    return convertSymbol(oper) == std::string(1, letter);
}

std::string operator+(const std::string& str, Symbol oper) {
    return str + convertSymbol(oper);
}

std::ostream& operator<<(std::ostream& out, Expr const& data) {
    out << data.getInfo();
    return out;
}