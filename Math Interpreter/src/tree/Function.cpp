#include "Function.h"
#include "Operation.h"
#include "Number.h"
#include "ConstVar.h"


Function::Function(const std::string& name, std::vector<std::unique_ptr<Expr>>& input)
    : name(name) {
    arguments.reserve(input.size());
    for (const auto& arg : input) {
        arguments.push_back(arg->clone());
    }
}

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

std::unique_ptr<Expr> Function::approximate(const paramArgMap& extraMap) {
    if (defaultFunctionMap.count(name)) {
        return callDefault(extraMap);
    }
    else if (functionMap.find(name) != functionMap.end()) {
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

            if (functionArgsMap.count(param)) {
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

std::map<std::string, Function::functionType> Function::defaultFunctionMap = { 
    {"sin", &defaultFunction::sine },
    {"cos", &defaultFunction::cosine},
    {"log", &defaultFunction::logb},
    {"ln", &defaultFunction::ln},
};

std::unique_ptr<Expr> Function::callDefault(const paramArgMap& extraMap) {
    if (defaultFunctionMap.count(name)) {
        return defaultFunctionMap.at(name)(arguments, extraMap);
    }
    throw std::exception("Function does not exist");
}

bool defaultFunction::checkDefaultFunctionArgumentAmount(std::vector<std::unique_ptr<Expr>>& args, size_t amount) {
    if (args.size() == amount) {
        return true;
    }
    return false;
}

template <typename... Args>
void defaultFunction::getArguments(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap, Args&... args) {
    if (arguments.size() != sizeof...(Args)) {
        throw std::runtime_error("Incorrect number of arguments provided.");
    }
    size_t index = 0;
    defaultFunction::getArgumentsRecursive(arguments, index, extraMap, args...);
}

template <typename Arg, typename... Args>
void defaultFunction::getArgumentsRecursive(std::vector<std::unique_ptr<Expr>>& arguments, size_t& index, const Function::paramArgMap& extraMap, Arg& arg, Args&... args) { // line 135
    if (arguments.size() >= index) {
        arg = arguments[index]->evaluate(extraMap);
        index++;
        defaultFunction::getArgumentsRecursive(arguments, index, extraMap, args...); // line 139
    }
}

template <typename Arg>
void defaultFunction::getArgumentsRecursive(std::vector<std::unique_ptr<Expr>>& arguments, size_t& index, const Function::paramArgMap& extraMap, Arg& arg) {
    if (arguments.size() >= index) {
        arg = arguments[index]->evaluate(extraMap);
        index++;
    }
}

std::unique_ptr<Expr> defaultFunction::sine(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap) {
    std::unique_ptr<Expr> arg;
    getArguments(arguments, extraMap, arg);

    Number* result = dynamic_cast<Number*>(arg.get());
    if (result) {
        return std::make_unique<Number>(sin(result->value));
    }
    throw std::exception("Function error!!!");
}

std::unique_ptr<Expr> defaultFunction::cosine(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap) {
    std::unique_ptr<Expr> arg;
    getArguments(arguments, extraMap, arg);

    Number* result = dynamic_cast<Number*>(arg.get());
    if (result) {
        return std::make_unique<Number>(cos(result->value));
    }
    throw std::exception("Function error!!!");
}

std::unique_ptr<Expr> defaultFunction::logb(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap) {
    std::unique_ptr<Expr> b, a;
    getArguments(arguments, extraMap, b, a);

    Number* bRes = dynamic_cast<Number*>(b.get());
    Number* aRes = dynamic_cast<Number*>(a.get());
    if (bRes && aRes) {
        return std::make_unique<Number>(log(aRes->value) / log(bRes->value));
    }
    throw std::exception("Function error!!!");
}

std::unique_ptr<Expr> defaultFunction::ln(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap) {
    std::unique_ptr<Expr> a;
    getArguments(arguments, extraMap, a);

    Number* aRes = dynamic_cast<Number*>(a.get());
    if (aRes) {
        return std::make_unique<Number>(log(aRes->value));
    }
    throw std::exception("Function error!!!");
}