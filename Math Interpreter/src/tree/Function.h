#pragma once
#include "Identifier.h"

class Function : public Identifier {
public:
    std::string name;

    std::vector<std::unique_ptr<Expr>> arguments;

    Function(const std::string& name, std::vector<std::unique_ptr<Expr>>& input);
    //Function(const std::string&& name, std::vector<std::unique_ptr<Expr>>&& input);
    Function(const Function& obj);

    std::unique_ptr<Expr> clone() const override;
    std::unique_ptr<Identifier> cloneIdentifier() const override;

    const std::string& getName() override;

    paramArgMap makeArgMap(std::vector<std::string> params);

    //std::unique_ptr<Expr> approximate() override;
    std::unique_ptr<Expr> approximate(const paramArgMap& extraMap) override;

    //std::unique_ptr<Expr> evaluate() override;
    std::unique_ptr<Expr> evaluate(const paramArgMap& extraMap) override;

    std::string getInfo() const override;
};

extern std::unordered_map<std::string, std::pair<std::vector<std::string>, std::shared_ptr<Expr>>> functionMap;
//                       function name                parameter names       function expression