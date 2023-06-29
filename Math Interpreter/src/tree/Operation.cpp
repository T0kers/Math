#include "Operation.h"
#include "ConstVar.h"
#include "Error.h"
#include "Number.h"

#include "Tree.h"

Operation::Operation(std::unique_ptr<Expr> lChild, std::unique_ptr<Expr> rChild)
    : lChild(std::move(lChild)), rChild(std::move(rChild)) {}

Operation::Operation(const Operation& obj)
    : lChild(obj.lChild->clone()),
    rChild(obj.rChild->clone()) {}

bool Operation::hasConst() {
    return (dynamic_cast<ConstVar*>(lChild.get()) != nullptr) || (dynamic_cast<ConstVar*>(rChild.get()) != nullptr);
}

std::unique_ptr<Expr> Operation::approximate() {
    std::string errorMsg = "";

    std::unique_ptr<Expr> left = lChild->approximate();
    std::unique_ptr<Expr> right = rChild->approximate();

    if (isError(left)) {
        errorMsg += left->getInfo() + "\n";
    }
    if (isError(right)) {
        errorMsg += right->getInfo() + "\n";
    }

    if (errorMsg.empty()) {
        return this->calcApproximate();
    }
    else {
        return std::make_unique<Error>(errorMsg);
    }
}

std::unique_ptr<Expr> Operation::approximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    std::string errorMsg = "";

    std::unique_ptr<Expr> left = lChild->approximate();
    std::unique_ptr<Expr> right = rChild->approximate();

    if (isError(left)) {
        errorMsg += left->getInfo() + "\n";
    }
    if (isError(right)) {
        errorMsg += right->getInfo() + "\n";
    }

    if (errorMsg.empty()) {
        return this->calcApproximate(extraMap);
    }
    else {
        return std::make_unique<Error>(errorMsg);
    }
}

std::unique_ptr<Expr> Operation::evaluate() {
    return this->evaluate(); // bruh, stack overflow
}

std::unique_ptr<Expr> Operation::evaluate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    return this->evaluate(extraMap); // bruh, stack overflow
} // der skal laves en calcEvaluate i hver subclass, ELLER ændre operation, til at den ikke har subclasses.

std::unique_ptr<Expr> Plus::clone() const {
    return std::make_unique<Plus>(*this);
}

std::unique_ptr<Expr> Plus::calcApproximate() {
    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate().get())->value + static_cast<Number*>(rChild->approximate().get())->value);
}

std::unique_ptr<Expr> Plus::calcApproximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate(extraMap).get())->value + static_cast<Number*>(rChild->approximate(extraMap).get())->value);
}

std::string Plus::getInfo() const {
    return "(" + lChild->getInfo() + Op::PLUS + rChild->getInfo() + ")";
}

std::unique_ptr<Expr> Minus::clone() const {
    return std::make_unique<Minus>(*this);
}

std::unique_ptr<Expr> Minus::calcApproximate() {
    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate().get())->value - static_cast<Number*>(rChild->approximate().get())->value);
}

std::unique_ptr<Expr> Minus::calcApproximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate(extraMap).get())->value + static_cast<Number*>(rChild->approximate(extraMap).get())->value);
}

std::string Minus::getInfo() const {
    return "(" + lChild->getInfo() + Op::MINUS + rChild->getInfo() + ")";
}

std::unique_ptr<Expr> Multiply::clone() const {
    return std::make_unique<Multiply>(*this);
}

std::unique_ptr<Expr> Multiply::calcApproximate() {
    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate().get())->value * static_cast<Number*>(rChild->approximate().get())->value);
}

std::unique_ptr<Expr> Multiply::calcApproximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate(extraMap).get())->value + static_cast<Number*>(rChild->approximate(extraMap).get())->value);
}

std::string Multiply::getInfo() const {
    return "(" + lChild->getInfo() + Op::MULTIPLY + rChild->getInfo() + ")";
}

std::unique_ptr<Expr> Divide::clone() const {
    return std::make_unique<Divide>(*this);
}

std::unique_ptr<Expr> Divide::calcApproximate() {
    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate().get())->value / static_cast<Number*>(rChild->approximate().get())->value);
}

std::unique_ptr<Expr> Divide::calcApproximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate(extraMap).get())->value + static_cast<Number*>(rChild->approximate(extraMap).get())->value);
}

std::string Divide::getInfo() const {
    return "(" + lChild->getInfo() + Op::DIVIDE + rChild->getInfo() + ")";
}

std::unique_ptr<Expr> Power::clone() const {
    return std::make_unique<Power>(*this);
}

std::unique_ptr<Expr> Power::calcApproximate() {
    return std::make_unique<Number>(pow(static_cast<Number*>(lChild->approximate().get())->value, static_cast<Number*>(rChild->approximate().get())->value));
}

std::unique_ptr<Expr> Power::calcApproximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate(extraMap).get())->value + static_cast<Number*>(rChild->approximate(extraMap).get())->value);
}

std::string Power::getInfo() const {
    return "(" + lChild->getInfo() + Op::POWER + rChild->getInfo() + ")";
}