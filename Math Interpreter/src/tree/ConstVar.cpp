#include "ConstVar.h"
#include "Number.h"
#include "Error.h"

#include "Tree.h"

ConstVar::ConstVar(std::string&& name)
    : name(name) {}

ConstVar::ConstVar(std::string& name)
    : name(name) {}

ConstVar::ConstVar(const ConstVar& obj)
    : name(obj.name) {}

//std::unique_ptr<Expr> ConstVar::approximate() {
//    if (constantMap.find(name) != constantMap.end()) {
//        return std::make_unique<Number>(constantMap.at(name));
//    }
//    else if (variableMap.find(name) != variableMap.end()) {
//        return variableMap.at(name)->approximate();
//    }
//    return std::make_unique<Error>("Identifier does not exist");
//}

std::unique_ptr<Expr> ConstVar::approximate(const paramArgMap& extraMap) {
    if (constantMap.find(name) != constantMap.end()) {
        return std::make_unique<Number>(constantMap.at(name));
    }
    else if (extraMap.find(name) != extraMap.end()) {
        std::unique_ptr<Expr> result = extraMap.at(name)->clone();
        ConstVar* constVar = dynamic_cast<ConstVar*>(result.get());
        if (constVar) {
            if (variableMap.find(name) != variableMap.end()) {
                return variableMap.at(name)->approximate(extraMap);
            }
        }
        return result->approximate(extraMap);
    }
    else if (variableMap.find(name) != variableMap.end()) {
        return variableMap.at(name)->approximate(extraMap);
    }
    throw std::exception("Identifier does not exist");
}

std::unique_ptr<Expr> ConstVar::clone() const {
    return std::make_unique<ConstVar>(*this);
}

std::unique_ptr<Identifier> ConstVar::cloneIdentifier() const {
    return std::make_unique<ConstVar>(*this);
}

const std::string& ConstVar::getName() {
    return name;
}

//std::unique_ptr<Expr> ConstVar::evaluate() {
//    if (constantMap.find(name) != constantMap.end()) {
//        return std::make_unique<ConstVar>(name);
//    }
//    else if (variableMap.find(name) != variableMap.end()) {
//        return variableMap.at(name)->evaluate();
//    }
//    return std::make_unique<ConstVar>(name);
//}

std::unique_ptr<Expr> ConstVar::evaluate(const paramArgMap& extraMap) {
    if (constantMap.find(name) != constantMap.end()) {
        return std::make_unique<ConstVar>(name);
    }
    else if (extraMap.find(name) != extraMap.end()) {
        return extraMap.at(name)->evaluate(extraMap);
    }
    else if (variableMap.find(name) != variableMap.end()) {
        return variableMap.at(name)->evaluate(extraMap);
    }
    return std::make_unique<ConstVar>(name);
}

std::string ConstVar::getInfo() const {
    return name;
}

std::unordered_map<std::string, double> constantMap{
    {"pi", 3.1415926535},
    {"e", 2.71828182845},
};

std::unordered_map<std::string, std::shared_ptr<Expr>> variableMap;