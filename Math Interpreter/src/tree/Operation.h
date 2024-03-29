#pragma once
#include "Expr.h"

class NewOperation : public Expr {
public:
    enum class Operator {
        error = -1, addition, subtraction, multiplication, division, exponentiation
    };

    std::unique_ptr<Expr> lChild;
    std::unique_ptr<Expr> rChild;
    Operator oper;

    NewOperation(Symbol oper, std::unique_ptr<Expr> lChild, std::unique_ptr<Expr> rChild);
    NewOperation(Operator oper, std::unique_ptr<Expr> lChild, std::unique_ptr<Expr> rChild);
    NewOperation(const NewOperation& obj);

    Operator symbolToOperator(Symbol sy);
    virtual std::unique_ptr<Expr> clone() const override;

    std::string getInfo() const override;

    std::unique_ptr<Expr> approximate(const paramArgMap& extraMap) override;

    std::unique_ptr<Expr> evaluate(const paramArgMap& extraMap) override;

    std::unique_ptr<Expr> additionApproximate(const paramArgMap& extraMap);
    std::unique_ptr<Expr> subtractionApproximate(const paramArgMap& extraMap);
    std::unique_ptr<Expr> multiplicationApproximate(const paramArgMap& extraMap);
    std::unique_ptr<Expr> divisionApproximate(const paramArgMap& extraMap);
    std::unique_ptr<Expr> exponentiationApproximate(const paramArgMap& extraMap);

    std::unique_ptr<Expr> additionEvaluate(const paramArgMap& extraMap);
    std::unique_ptr<Expr> subtractionEvaluate(const paramArgMap& extraMap);
    std::unique_ptr<Expr> multiplicationEvaluate(const paramArgMap& extraMap);
    std::unique_ptr<Expr> divisionEvaluate(const paramArgMap& extraMap);
    std::unique_ptr<Expr> exponentiationEvaluate(const paramArgMap& extraMap);
};