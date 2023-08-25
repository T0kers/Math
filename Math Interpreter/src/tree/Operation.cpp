#include "Operation.h"
#include "ConstVar.h"
#include "Error.h"
#include "Number.h"

#include "Tree.h"


NewOperation::NewOperation(Symbol oper, std::unique_ptr<Expr> lChild, std::unique_ptr<Expr> rChild)
    : oper(symbolToOperator(oper)), lChild(std::move(lChild)), rChild(std::move(rChild)) {}

NewOperation::NewOperation(Operator oper, std::unique_ptr<Expr> lChild, std::unique_ptr<Expr> rChild)
    : oper(oper), lChild(std::move(lChild)), rChild(std::move(rChild)) {}

NewOperation::NewOperation(const NewOperation& obj)
    : oper(obj.oper), lChild(obj.lChild->clone()), rChild(obj.rChild->clone()) {}

NewOperation::Operator NewOperation::symbolToOperator(Symbol sy) {
    switch (sy) {
    case Symbol::PLUS:
        return Operator::addition;
        break;
    case Symbol::MINUS:
        return Operator::subtraction;
        break;
    case Symbol::MULTIPLY:
        return Operator::multiplication;
        break;
    case Symbol::DIVIDE:
        return Operator::division;
        break;
    case Symbol::POWER:
        return Operator::exponentiation;
        break;
    default:
        return Operator::error;
        break;
    }
}

std::unique_ptr<Expr> NewOperation::clone() const {
    return std::make_unique<NewOperation>(*this);
}

std::string NewOperation::getInfo() const {
    std::string info = '[' + lChild->getInfo();

    switch (oper) {
    case NewOperation::Operator::addition:
        info += '+';
        break;
    case NewOperation::Operator::subtraction:
        info += '-';
        break;
    case NewOperation::Operator::multiplication:
        info += '*';
        break;
    case NewOperation::Operator::division:
        info += '/';
        break;
    case NewOperation::Operator::exponentiation:
        info += '^';
        break;
    default:
        info += '?';
        break;
    }
    info += rChild->getInfo() + ']';
    return info;
}

std::unique_ptr<Expr> NewOperation::approximate(const paramArgMap& extraMap) {
    switch (oper)
    {
    case NewOperation::Operator::addition:
        return this->additionApproximate(extraMap);
        break;
    case NewOperation::Operator::subtraction:
        return this->subtractionApproximate(extraMap);
        break;
    case NewOperation::Operator::multiplication:
        return this->multiplicationApproximate(extraMap);
        break;
    case NewOperation::Operator::division:
        return this->divisionApproximate(extraMap);
        break;
    case NewOperation::Operator::exponentiation:
        return this->exponentiationApproximate(extraMap);
        break;
    case NewOperation::Operator::error:
    default:
        throw std::exception("There was an error with an operation.");
        break;
    }
}

std::unique_ptr<Expr> NewOperation::evaluate(const paramArgMap& extraMap) {
    switch (oper)
    {
    case NewOperation::Operator::addition:
        return this->additionEvaluate(extraMap);
        break;
    case NewOperation::Operator::subtraction:
        return this->subtractionEvaluate(extraMap);
        break;
    case NewOperation::Operator::multiplication:
        return this->multiplicationEvaluate(extraMap);
        break;
    case NewOperation::Operator::division:
        return this->divisionEvaluate(extraMap);
        break;
    case NewOperation::Operator::exponentiation:
        return this->exponentiationEvaluate(extraMap);
        break;
    case NewOperation::Operator::error:
    default:
        throw std::exception("There was an error with an operation.");
        break;
    }
}

std::unique_ptr<Expr> NewOperation::additionApproximate(const paramArgMap& extraMap) {
    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate(extraMap).get())->value + static_cast<Number*>(rChild->approximate(extraMap).get())->value);
}
std::unique_ptr<Expr> NewOperation::subtractionApproximate(const paramArgMap& extraMap) {
    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate(extraMap).get())->value - static_cast<Number*>(rChild->approximate(extraMap).get())->value);

}
std::unique_ptr<Expr> NewOperation::multiplicationApproximate(const paramArgMap& extraMap) {
    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate(extraMap).get())->value * static_cast<Number*>(rChild->approximate(extraMap).get())->value);

}
std::unique_ptr<Expr> NewOperation::divisionApproximate(const paramArgMap& extraMap) {
    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate(extraMap).get())->value / static_cast<Number*>(rChild->approximate(extraMap).get())->value);

}
std::unique_ptr<Expr> NewOperation::exponentiationApproximate(const paramArgMap& extraMap) {
    return std::make_unique<Number>(pow(static_cast<Number*>(lChild->approximate(extraMap).get())->value, static_cast<Number*>(rChild->approximate(extraMap).get())->value));
}

std::unique_ptr<Expr> NewOperation::additionEvaluate(const paramArgMap& extraMap) {
    if (dynamic_cast<Number*>(lChild->evaluate(extraMap).get()) && dynamic_cast<Number*>(rChild->evaluate(extraMap).get())) {
        return this->approximate(extraMap);
    }
    return this->clone();
}
std::unique_ptr<Expr> NewOperation::subtractionEvaluate(const paramArgMap& extraMap) {
    if (dynamic_cast<Number*>(lChild->evaluate(extraMap).get()) && dynamic_cast<Number*>(rChild->evaluate(extraMap).get())) {
        return this->approximate(extraMap);
    }
    return this->clone();
}
std::unique_ptr<Expr> NewOperation::multiplicationEvaluate(const paramArgMap& extraMap) {
    if (dynamic_cast<Number*>(lChild->evaluate(extraMap).get()) && dynamic_cast<Number*>(rChild->evaluate(extraMap).get())) {
        return this->approximate(extraMap);
    }
    return this->clone();
}
std::unique_ptr<Expr> NewOperation::divisionEvaluate(const paramArgMap& extraMap) {
    if (dynamic_cast<Number*>(lChild->evaluate(extraMap).get()) && dynamic_cast<Number*>(rChild->evaluate(extraMap).get())) {
        return this->approximate(extraMap);
    }
    return this->clone();
}
std::unique_ptr<Expr> NewOperation::exponentiationEvaluate(const paramArgMap& extraMap) {
    if (dynamic_cast<Number*>(lChild->evaluate(extraMap).get()) && dynamic_cast<Number*>(rChild->evaluate(extraMap).get())) {
        return this->approximate(extraMap);
    }
    return this->clone();
}