#pragma once
#include "Expr.h"

class Operation : public Expr {
public:
    std::unique_ptr<Expr> lChild;
    std::unique_ptr<Expr> rChild;

    Operation(std::unique_ptr<Expr> lChild, std::unique_ptr<Expr> rChild);
    Operation(const Operation& obj);

    bool hasConst();
    std::unique_ptr<Expr> approximate() override;
    std::unique_ptr<Expr> approximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;
    virtual std::unique_ptr<Expr> calcApproximate() = 0;
    virtual std::unique_ptr<Expr> calcApproximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) = 0;

    std::unique_ptr<Expr> evaluate() override;
    std::unique_ptr<Expr> evaluate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;
};

class Plus : public Operation {
public:
    using Operation::Operation;

    std::unique_ptr<Expr> clone() const override;

    std::unique_ptr<Expr> calcApproximate() override;
    std::unique_ptr<Expr> calcApproximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;


    std::string getInfo() const override;
};

class Minus : public Operation {
public:
    using Operation::Operation;

    std::unique_ptr<Expr> clone() const override;

    std::unique_ptr<Expr> calcApproximate() override;
    std::unique_ptr<Expr> calcApproximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;

    std::string getInfo() const override;
};

class Multiply : public Operation {
public:
    using Operation::Operation;

    std::unique_ptr<Expr> clone() const override;

    std::unique_ptr<Expr> calcApproximate() override;
    std::unique_ptr<Expr> calcApproximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;

    std::string getInfo() const override;
};

class Divide : public Operation {
public:
    using Operation::Operation;

    std::unique_ptr<Expr> clone() const override;

    std::unique_ptr<Expr> calcApproximate() override;
    std::unique_ptr<Expr> calcApproximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;

    std::string getInfo() const override;
};

class Power : public Operation {
public:
    using Operation::Operation;

    std::unique_ptr<Expr> clone() const override;

    std::unique_ptr<Expr> calcApproximate() override;
    std::unique_ptr<Expr> calcApproximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) override;

    std::string getInfo() const override;
};