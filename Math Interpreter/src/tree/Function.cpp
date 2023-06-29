#include "Function.h"
#include "Error.h"

#include "Tree.h"

Function::Function(std::string& name, std::vector<std::unique_ptr<Expr>>& input)
    : name(name), arguments(std::move(input)) {}

Function::Function(const Function& obj)
    : name(obj.name)
{
    arguments.reserve(obj.arguments.size());
    for (const auto& arg : obj.arguments) {
        arguments.push_back(arg->clone());
    }
}

std::unique_ptr<Expr> Function::clone() const {
    return std::make_unique<Function>(*this);
}

std::unique_ptr<Identifier> Function::cloneIdentifier() const {
    return std::make_unique<Function>(*this);
}

const std::string& Function::getName() {
    return name;
}

std::map<std::string, std::unique_ptr<Expr>> Function::makeArgMap(std::vector<std::string> params) {
    std::map<std::string, std::unique_ptr<Expr>> functionArgsMap;
    for (size_t i = 0; i < params.size(); ++i) {
        std::string& param = params[i];
        std::unique_ptr<Expr>& arg = arguments[i];

        functionArgsMap[param] = std::move(arg);
    }
    return functionArgsMap;
}

std::unique_ptr<Expr> Function::approximate() {
    if (functionMap.find(name) != functionMap.end()) {
        std::pair<std::vector<std::string>, std::shared_ptr<Expr>> parametersAndExpr = functionMap.at(name);
        std::vector<std::string> parameters = parametersAndExpr.first;

        if (parameters.size() != arguments.size()) {
            return std::make_unique<Error>("Wrong amount of arguments provided for function '" + name + "'. The function takes in " + std::to_string(parameters.size()) + " arguments.");
        }
        auto functionArgsMap = makeArgMap(parameters);
        return parametersAndExpr.second->approximate(functionArgsMap);
    }

    return std::make_unique<Error>("Function '" + name + "' does not excist.");
}

std::unique_ptr<Expr> Function::approximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    if (functionMap.find(name) != functionMap.end()) {
        std::pair<std::vector<std::string>, std::shared_ptr<Expr>> parametersAndExpr = functionMap.at(name);
        std::vector<std::string> parameters = parametersAndExpr.first;

        if (parameters.size() != arguments.size()) {
            return std::make_unique<Error>("Wrong amount of arguments provided for function '" + name + "'. The function takes in " + std::to_string(parameters.size()) + " arguments.");
        }
        std::map<std::string, std::unique_ptr<Expr>> functionArgsMap = makeArgMap(parameters);

        //functionArgsMap.insert(extraMap.begin(), extraMap.end());
        functionArgsMap.merge(extraMap);
        return parametersAndExpr.second->approximate(functionArgsMap);
    }

    return std::make_unique<Error>("Function '" + name + "' does not exist.");
}

std::unique_ptr<Expr> Function::evaluate() {
    auto result = this->approximate(); // ik brug approx her tror jeg
    if (isError(result)) {
        return std::make_unique<Function>(name, arguments);
    }
    return result;

}

std::unique_ptr<Expr> Function::evaluate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    auto result = this->approximate(extraMap); // ik brug approx her tror jeg
    if (isError(result)) {
        return std::make_unique<Function>(name, arguments);
    }
    return result;
}

std::string Function::getInfo() const {
    std::string info = name + '(';
    for (const auto& arg : arguments) {
        info += arg->getInfo() + ", ";
    }
    info += ')';
    return info;
}

std::unordered_map<std::string, std::pair<std::vector<std::string>, std::shared_ptr<Expr>>> functionMap;