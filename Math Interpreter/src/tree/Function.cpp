#include "Function.h"
#include "Error.h"

#include "Tree.h"

Function::Function(const std::string& name, std::vector<std::unique_ptr<Expr>>& input)
    : name(name) {
    arguments.reserve(input.size());
    for (const auto& arg : input) {
        arguments.push_back(arg->clone());
    }
}

//Function::Function(const std::string&& name, std::vector<std::unique_ptr<Expr>>&& input)
//    : name(name) {
//    arguments.reserve(input.size());
//    for (const auto& arg : input) {
//        arguments.push_back(arg->clone());
//    }
//}

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

        functionArgsMap[param] = arg->clone();
    }
    return functionArgsMap;
}

//std::unique_ptr<Expr> Function::approximate() {
//    if (functionMap.find(name) != functionMap.end()) {
//        std::pair<std::vector<std::string>, std::shared_ptr<Expr>> parametersAndExpr = functionMap.at(name);
//        std::vector<std::string> parameters = parametersAndExpr.first;
//
//        if (parameters.size() != arguments.size()) {
//            return std::make_unique<Error>("Wrong amount of arguments provided for function '" + name + "'. The function takes in " + std::to_string(parameters.size()) + " arguments.");
//        }
//        auto functionArgsMap = makeArgMap(parameters);
//        return parametersAndExpr.second->approximate(functionArgsMap);
//    }
//
//    return std::make_unique<Error>("Function '" + name + "' does not excist.");
//}

std::unique_ptr<Expr> Function::approximate(const paramArgMap& extraMap) {
    if (functionMap.find(name) != functionMap.end()) {
        std::pair<std::vector<std::string>, std::shared_ptr<Expr>> parametersAndExpr = functionMap.at(name);
        std::vector<std::string> parameters = parametersAndExpr.first;

        if (parameters.size() != arguments.size()) {
            std::ostringstream e;
            e << "Wrong amount of arguments provided for function '" << name << "'. The function takes in " << parameters.size() << " arguments.";
            auto er = e.str();
            throw std::exception(er.c_str());
        }
        paramArgMap functionArgsMap = makeArgMap(parameters);

        for (const auto& item : extraMap) {
            const std::string& param = item.first;
            const std::unique_ptr<Expr>& arg = item.second;

            if (functionArgsMap.find(param) != functionArgsMap.end()) {
                continue;
            }
            else {
                functionArgsMap[param] = arg->clone();
            }
        }
        return parametersAndExpr.second->approximate(functionArgsMap);
    }
    std::ostringstream e;
    e << "Function '" << name << "' does not exist.";
    auto er = e.str();
    throw std::exception(er.c_str());
}

//std::unique_ptr<Expr> Function::evaluate() {
//    auto result = this->approximate(); // ik brug approx her tror jeg
//    if (isError(result)) {
//        return std::make_unique<Function>(name, arguments);
//    }
//    return result;
//
//}

std::unique_ptr<Expr> Function::evaluate(const paramArgMap& extraMap) {
    try {
        auto result = this->approximate(extraMap); // TODO: ik brug approx her
        return result;
    }
    catch (const std::exception& e) {
        std::cout << "Warning!! " << e.what() << std::endl;
        return std::make_unique<Function>(name, arguments);
    }
}

std::string Function::getInfo() const {
    std::string info = name + '(';
    for (const auto& arg : arguments) {
        info += arg->getInfo() + ", ";
    }
    info = info.substr(0, info.size() - 2) + ')';
    return info;
}

std::unordered_map<std::string, std::pair<std::vector<std::string>, std::shared_ptr<Expr>>> functionMap = {
    {std::string("gaming"), {{std::string("x")}, std::make_unique<NewOperation>(NewOperation::Operator::addition, std::make_unique<ConstVar>(std::string("x")), std::make_unique<Number>(2))}},
};