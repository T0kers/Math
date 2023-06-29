#include "Error.h"

#include "Tree.h"

Error::Error(std::string msg)
    : msg(msg) {}

Error::Error(const Error& obj)
    : msg(obj.msg) {}

std::unique_ptr<Expr> Error::clone() const {
    return std::make_unique<Error>(*this);
}

std::unique_ptr<Expr> Error::approximate() {
    return std::make_unique<Error>(msg);
}

std::unique_ptr<Expr> Error::approximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    return this->approximate();
}

std::unique_ptr<Expr> Error::evaluate() {
    return std::make_unique<Error>(msg);
}

std::unique_ptr<Expr> Error::evaluate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    return this->evaluate();
}

std::string Error::getInfo() const {
    return msg;
}

bool isError(std::unique_ptr<Expr>& textExpr) {
    return dynamic_cast<Error*>(textExpr.get()) != nullptr;
}