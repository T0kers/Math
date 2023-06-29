#pragma once
#include "Identifier.h"

class Function : public Identifier {
public:
    std::string name;

    std::vector<std::unique_ptr<Expr>> arguments;

    Function(std::string& name, std::vector<std::unique_ptr<Expr>>& input);
    Function(const Function& obj);

    std::unique_ptr<Expr> clone() const override;
    std::unique_ptr<Identifier> cloneIdentifier() const override;

    const std::string& getName() override;

    std::map<std::string, std::unique_ptr<Expr>> makeArgMap(std::vector<std::string> params);

    std::unique_ptr<Expr> approximate() override;
    std::unique_ptr<Expr> approximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;

    std::unique_ptr<Expr> evaluate() override;
    std::unique_ptr<Expr> evaluate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;

    std::string getInfo() const override;
};

extern std::unordered_map<std::string, std::pair<std::vector<std::string>, std::shared_ptr<Expr>>> functionMap;
//                       function name                     input vars names     function expression