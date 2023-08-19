#pragma once
#include "Expr.h"

class IdentifierPtr : public Expr {
public:
    std::shared_ptr<Expr> child;

    IdentifierPtr(std::shared_ptr<Expr> ptr);
    IdentifierPtr(const IdentifierPtr& obj);

    std::unique_ptr<Expr> clone() const override;

    //std::unique_ptr<Expr> approximate() override;
    std::unique_ptr<Expr> approximate(const paramArgMap& extraMap) override;

    //std::unique_ptr<Expr> evaluate() override;
    std::unique_ptr<Expr> evaluate(const paramArgMap& extraMap) override;

    std::string getInfo() const override;
};