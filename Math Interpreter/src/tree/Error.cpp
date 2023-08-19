/*#include "Error.h"

#include "Tree.h"

Error::Error(std::string msg)
    : msg(msg) {}

Error::Error(const Error& obj)
    : msg(obj.msg) {}

std::unique_ptr<Expr> Error::clone() const {
    return std::make_unique<Error>(*this);
}

std::unique_ptr<Expr> Error::approximate(const paramArgMap& extraMap) {
    return std::make_unique<Error>(msg);
}

std::unique_ptr<Expr> Error::evaluate(const paramArgMap& extraMap) {
    return std::make_unique<Error>(msg);
}

std::string Error::getInfo() const {
    return msg;
}

bool isError(std::unique_ptr<Expr>& textExpr) {
    return dynamic_cast<Error*>(textExpr.get()) != nullptr;
}*/