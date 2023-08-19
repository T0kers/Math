#pragma once
#include "Expr.h"

class Assignment : public Expr {
public:
    std::unique_ptr<Identifier> variable;
    std::unique_ptr<Expr> value;

    Assignment(std::unique_ptr<Identifier> var, std::unique_ptr<Expr> val);
    Assignment(const Assignment& obj);

    std::unique_ptr<Expr> clone() const override;

    //std::unique_ptr<Expr> approximate() override;
    std::unique_ptr<Expr> approximate(const paramArgMap& extraMap) override;

    //std::unique_ptr<Expr> evaluate() override;
    std::unique_ptr<Expr> evaluate(const paramArgMap& extraMap) override;

    std::string getInfo() const override;
};