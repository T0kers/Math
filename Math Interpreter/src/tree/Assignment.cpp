#include "Tree.h"

Assignment::Assignment(std::unique_ptr<Identifier> var, std::unique_ptr<Expr> val)
    : variable(std::move(var)), value(std::move(val)) {}

Assignment::Assignment(const Assignment& obj)
    : variable(obj.variable->cloneIdentifier()),
    value(obj.value->clone()) {}

std::unique_ptr<Expr> Assignment::clone() const {
    return std::make_unique<Assignment>(*this);
}

std::unique_ptr<Expr> Assignment::approximate(const paramArgMap& extraMap) {
    std::string name = variable->getName();

    if (constantMap.count(name)) {
        throw std::exception("This identifier name is protected.");
    }

    auto result = value->approximate(extraMap);

    if (extraMap.count(name)) {
        ConstVar* varPtr = dynamic_cast<ConstVar*>(extraMap.at(name).get());
        if (varPtr != nullptr) {
            variableMap[varPtr->name] = std::move(result);
            return variableMap.at(name)->clone();
        }
    }
    variableMap[name] = std::move(result);

    return variableMap.at(name)->clone();
}

std::unique_ptr<Expr> Assignment::evaluate(const paramArgMap& extraMap) {
    std::string name = variable->getName();

    if (dynamic_cast<ConstVar*>(variable.get())) {
        if (constantMap.count(name)) {
            throw std::exception("This identifier name is protected.");
        }
        try {
            auto result = value->evaluate(extraMap);
        }
        catch (...) {

        }

        if (extraMap.count(name)) {
            // check if variable is another variable, and then use that instead for creating new variable (i think this is bad way)
            ConstVar* varPtr = dynamic_cast<ConstVar*>(extraMap.at(name).get());
            // make identifier function to get "to the bottom" of variable saved IN extraMap
            // ALWAYS change variable in extraMap, even if error, because it is wrong player input
            if (varPtr != nullptr) {
                variableMap[varPtr->name] = std::move(value); // !!!
                return variableMap.at(name)->clone();
            }
        }
        variableMap[name] = std::move(value); // !!!
        return variableMap.at(name)->clone();
    }
    Function* derivedPtr = dynamic_cast<Function*>(variable.get());
    if (derivedPtr) {
        std::vector<std::string> params;
        bool areParams = true;
        for (const auto& param : derivedPtr->arguments) {
            auto var = dynamic_cast<ConstVar*>(param.get());
            if (var) {
                params.push_back(var->getName());
            }
            else {
                areParams = false;
                break;
            }
        }
        if (areParams) {
            functionMap[name] = {params, std::move(value)};
            return functionMap.at(name).second->clone();
        }
        else {
            throw std::exception("Parameters needs to be variables.");
        }
    }
    else {
        throw std::exception("Illegal.");
    }
    
}

std::string Assignment::getInfo() const {
    return '(' + variable->getInfo() + Symbol::ASSIGN + value->getInfo() + ')';
}