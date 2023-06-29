#pragma once
#include "Expr.h"

class Error : public Expr {
public:
    std::string msg;

    Error(std::string msg);
    Error(const Error& obj);

    std::unique_ptr<Expr> clone() const override;

    std::unique_ptr<Expr> approximate() override;
    std::unique_ptr<Expr> approximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;

    std::unique_ptr<Expr> evaluate() override;
    std::unique_ptr<Expr> evaluate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;

    std::string getInfo() const override;
};

bool isError(std::unique_ptr<Expr>& textExpr);