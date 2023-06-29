#pragma once
#include "Identifier.h"

class ConstVar : public Identifier {
public:
    std::string name;

    ConstVar(std::string& name);
    ConstVar(const ConstVar& obj);

    std::unique_ptr<Expr> clone() const override;
    std::unique_ptr<Identifier> cloneIdentifier() const override;

    const std::string& getName() override;

    std::unique_ptr<Expr> approximate() override;
    std::unique_ptr<Expr> approximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;

    std::unique_ptr<Expr> evaluate() override;
    std::unique_ptr<Expr> evaluate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;

    std::string getInfo() const override;
};

extern std::unordered_map<std::string, double> constantMap;
extern std::unordered_map<std::string, std::shared_ptr<Expr>> variableMap;