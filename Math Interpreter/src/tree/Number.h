#pragma once
#include "Expr.h"

std::string doubleToString(double value);

class Number : public Expr {
public:
    double value;

    Number(double value);
    Number(const Number& obj);

    std::unique_ptr<Expr> clone() const override;

    std::unique_ptr<Expr> approximate() override;
    std::unique_ptr<Expr> approximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;

    std::unique_ptr<Expr> evaluate() override;
    std::unique_ptr<Expr> evaluate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;

    std::string getInfo() const override;
};