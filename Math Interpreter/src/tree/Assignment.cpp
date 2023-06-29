#include "Tree.h"

Assignment::Assignment(std::unique_ptr<Identifier> var, std::unique_ptr<Expr> val)
    : variable(std::move(var)), value(std::move(val)) {}

Assignment::Assignment(const Assignment& obj)
    : variable(obj.variable->cloneIdentifier()),
    value(obj.value->clone()) {}

std::unique_ptr<Expr> Assignment::clone() const {
    return std::make_unique<Assignment>(*this);
}

std::unique_ptr<Expr> Assignment::approximate() {
    std::string name = variable->getName();
    if (constantMap.find(name) != constantMap.end()) {
        return std::make_unique<Error>("This identifier name is protected.");
    }
    variableMap[name] = value->evaluate();

    for (const auto& i : variableMap) {
        std::cout << i.first + " " + i.second->getInfo() << "\n";
    }

    return variableMap.at(name)->approximate();
}

std::unique_ptr<Expr> Assignment::approximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    std::string name = variable->getName();

    if (constantMap.find(name) != constantMap.end()) {
        return std::make_unique<Error>("This identifier name is protected.");
    }

    if (extraMap.find(name) != extraMap.end()) {
        ConstVar* varPtr = dynamic_cast<ConstVar*>(extraMap.at(name).get());
        if (varPtr != nullptr) {
            variableMap[varPtr->name] = value->evaluate(extraMap);
            return variableMap.at(name)->approximate();
        }
    }
    variableMap[name] = value->evaluate();

    return variableMap.at(name)->approximate();
}

std::unique_ptr<Expr> Assignment::evaluate() {
    auto result = this->approximate(); // ik brug approx her tror jeg
    if (isError(result)) {
        return std::make_unique<Assignment>(variable->cloneIdentifier(), value->clone());
    }
    return result->clone();
}

std::unique_ptr<Expr> Assignment::evaluate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    auto result = this->approximate(extraMap); // ik brug approx her tror jeg
    if (isError(result)) {
        return std::make_unique<Assignment>(variable->cloneIdentifier(), value->clone());
    }
    return result->clone();
}

std::string Assignment::getInfo() const {
    return variable->getInfo() + " " + Op::ASSIGN + " " + value->getInfo();
}