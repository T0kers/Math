#pragma once
#include "Identifier.h"

class Function : public Identifier {
public:
    std::string name;

    std::vector<std::unique_ptr<Expr>> arguments;

    Function(const std::string& name, std::vector<std::unique_ptr<Expr>>& input);
    Function(const Function& obj);

    std::unique_ptr<Expr> clone() const override;
    std::unique_ptr<Identifier> cloneIdentifier() const override;

    const std::string& getName() override;

    paramArgMap makeArgMap(std::vector<std::string> params);

    std::unique_ptr<Expr> approximate(const paramArgMap& extraMap) override;

    std::unique_ptr<Expr> evaluate(const paramArgMap& extraMap) override;

    std::string getInfo() const override;

    typedef std::function<std::unique_ptr<Expr>(std::vector<std::unique_ptr<Expr>>& args, const paramArgMap&)> functionType;
    static std::map<std::string, functionType> defaultFunctionMap;

    
    std::unique_ptr<Expr> callDefault(const paramArgMap& extraMap);
};

namespace defaultFunction {
    template <typename... Args>
    void getArguments(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap, Args&... args);

    template <typename Arg, typename... Args>
    void getArgumentsRecursive(std::vector<std::unique_ptr<Expr>>& arguments, size_t& index, const Function::paramArgMap& extraMap, Arg& arg, Args&... args);

    template <typename Arg>
    void getArgumentsRecursive(std::vector<std::unique_ptr<Expr>>& arguments, size_t& index, const Function::paramArgMap& extraMap, Arg& arg);

    bool checkDefaultFunctionArgumentAmount(std::vector<std::unique_ptr<Expr>>& args, size_t amount);

    std::unique_ptr<Expr> sine(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap);
    std::unique_ptr<Expr> cosine(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap);
    std::unique_ptr<Expr> tangent(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap);
    std::unique_ptr<Expr> logb(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap);
    std::unique_ptr<Expr> ln(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap);
    std::unique_ptr<Expr> absolute(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap);
    std::unique_ptr<Expr> nroot(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap);
    std::unique_ptr<Expr> sroot(std::vector<std::unique_ptr<Expr>>& arguments, const Function::paramArgMap& extraMap);
}

extern std::unordered_map<std::string, std::pair<std::vector<std::string>, std::shared_ptr<Expr>>> functionMap;
//                       function name                parameter names       function expression