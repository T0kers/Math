#include "IdentifierPtr.h"

#include "Tree.h"

IdentifierPtr::IdentifierPtr(std::shared_ptr<Expr> ptr)
    : child(ptr) {}

IdentifierPtr::IdentifierPtr(const IdentifierPtr& obj)
    : child(obj.child) {}

std::unique_ptr<Expr> IdentifierPtr::clone() const {
    return std::make_unique<IdentifierPtr>(*this);
}

//std::unique_ptr<Expr> IdentifierPtr::approximate() {
//    return child->approximate();
//}

std::unique_ptr<Expr> IdentifierPtr::approximate(const paramArgMap& extraMap) {
    return child->approximate(extraMap);
}

//std::unique_ptr<Expr> IdentifierPtr::evaluate() {
//    return child->evaluate();
//}

std::unique_ptr<Expr> IdentifierPtr::evaluate(const paramArgMap& extraMap) {
    return child->evaluate(extraMap);
}

std::string IdentifierPtr::getInfo() const {
    return child->getInfo();
}