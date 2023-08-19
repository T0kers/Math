#include "Operation.h"
#include "ConstVar.h"
#include "Error.h"
#include "Number.h"

#include "Tree.h"


NewOperation::NewOperation(Symbol oper, std::unique_ptr<Expr> lChild, std::unique_ptr<Expr> rChild)
    : oper(symbolToOperator(oper)), lChild(std::move(lChild)), rChild(std::move(rChild)) {
    setOperator(this->oper);
}

NewOperation::NewOperation(Operator oper, std::unique_ptr<Expr> lChild, std::unique_ptr<Expr> rChild)
    : oper(oper), lChild(std::move(lChild)), rChild(std::move(rChild)) {
    setOperator(oper);
}

NewOperation::NewOperation(const NewOperation& obj)
    : oper(obj.oper),
      lChild(obj.lChild->clone()),
      rChild(obj.rChild->clone()) {
    setOperator(obj.oper);
}

void NewOperation::setOperator(Operator op) {
    switch (op) {
    case Operator::addition:
        operationApproximate.f = [this](const paramArgMap& extraMap) { return additionApproximate(extraMap); };
        operationEvaluate.f = [this](const paramArgMap& extraMap) { return additionEvaluate(extraMap); };
        break;
    case Operator::subtraction:
        operationApproximate.f = [this](const paramArgMap& extraMap) { return subtractionApproximate(extraMap); };
        operationEvaluate.f = [this](const paramArgMap& extraMap) { return subtractionEvaluate(extraMap); };
        break;
    case Operator::multiplication:
        operationApproximate.f = [this](const paramArgMap& extraMap) { return multiplicationApproximate(extraMap); };
        operationEvaluate.f = [this](const paramArgMap& extraMap) { return multiplicationEvaluate(extraMap); };
        break;
    case Operator::division:
        operationApproximate.f = [this](const paramArgMap& extraMap) { return divisionApproximate(extraMap); };
        operationEvaluate.f = [this](const paramArgMap& extraMap) { return divisionEvaluate(extraMap); };
        break;
    case Operator::exponentiation:
        operationApproximate.f = [this](const paramArgMap& extraMap) { return exponentiationApproximate(extraMap); };
        operationEvaluate.f = [this](const paramArgMap& extraMap) { return exponentiationEvaluate(extraMap); };
        break;
    default:
        break;
    }
}

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

bool NewOperation::hasConst() { // this only checks if it is const OR var. I think it only should return true if is constant
    return (dynamic_cast<ConstVar*>(lChild.get()) != nullptr) || (dynamic_cast<ConstVar*>(rChild.get()) != nullptr);
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
    return this->operationApproximate(extraMap);
}

std::unique_ptr<Expr> NewOperation::evaluate(const paramArgMap& extraMap) {
    return this->operationEvaluate(extraMap);
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




//Operation::Operation(std::unique_ptr<Expr> lChild, std::unique_ptr<Expr> rChild)
//    : lChild(std::move(lChild)), rChild(std::move(rChild)) {}
//
//Operation::Operation(const Operation& obj)
//    : lChild(obj.lChild->clone()),
//    rChild(obj.rChild->clone()) {}
//
//bool Operation::hasConst() {
//    return (dynamic_cast<ConstVar*>(lChild.get()) != nullptr) || (dynamic_cast<ConstVar*>(rChild.get()) != nullptr);
//}
//
////std::unique_ptr<Expr> Operation::approximate() {
////    std::string errorMsg = "";
////
////    std::unique_ptr<Expr> left = lChild->approximate();
////    std::unique_ptr<Expr> right = rChild->approximate();
////
////    if (isError(left)) {
////        errorMsg += left->getInfo() + "\n";
////    }
////    if (isError(right)) {
////        errorMsg += right->getInfo() + "\n";
////    }
////
////    if (errorMsg.empty()) {
////        return this->calcApproximate();
////    }
////    else {
////        return std::make_unique<Error>(errorMsg);
////    }
////}
//
//std::unique_ptr<Expr> Operation::approximate(const paramArgMap& extraMap) {
//    std::string errorMsg = "";
//
//    std::unique_ptr<Expr> left = lChild->approximate(extraMap);
//    std::unique_ptr<Expr> right = rChild->approximate(extraMap);
//
//    /*if (isError(left)) {
//        errorMsg += left->getInfo() + "\n";
//    }
//    if (isError(right)) {
//        errorMsg += right->getInfo() + "\n";
//    }
//
//    if (errorMsg.empty()) {
//        return this->calcApproximate(extraMap);
//    }
//    else {
//        return std::make_unique<Error>(errorMsg);
//    }*/
//    return this->calcApproximate(extraMap);
//}
//
////std::unique_ptr<Expr> Operation::evaluate() {
////    return this->evaluate(); // bruh, stack overflow
////}
//
//std::unique_ptr<Expr> Operation::evaluate(const paramArgMap& extraMap) {
//    return this->evaluate(extraMap); // bruh, stack overflow
//} // der skal laves en calcEvaluate i hver subclass, ELLER ændre operation, til at den ikke har subclasses.
//
//std::unique_ptr<Expr> Plus::clone() const {
//    return std::make_unique<Plus>(*this);
//}
//
////std::unique_ptr<Expr> Plus::calcApproximate() {
////    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate().get())->value + static_cast<Number*>(rChild->approximate().get())->value);
////}
//
//std::unique_ptr<Expr> Plus::calcapproximate(const paramArgMap& extraMap) {
//    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate(extraMap).get())->value + static_cast<Number*>(rChild->approximate(extraMap).get())->value);
//}
//
//std::string Plus::getInfo() const {
//    return "(" + lChild->getInfo() + Symbol::PLUS + rChild->getInfo() + ")";
//}
//
//std::unique_ptr<Expr> Minus::clone() const {
//    return std::make_unique<Minus>(*this);
//}
//
////std::unique_ptr<Expr> Minus::calcApproximate() {
////    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate().get())->value - static_cast<Number*>(rChild->approximate().get())->value);
////}
//
//std::unique_ptr<Expr> Minus::calcapproximate(const paramArgMap& extraMap) {
//    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate(extraMap).get())->value - static_cast<Number*>(rChild->approximate(extraMap).get())->value);
//}
//
//std::string Minus::getInfo() const {
//    return "(" + lChild->getInfo() + Symbol::MINUS + rChild->getInfo() + ")";
//}
//
//std::unique_ptr<Expr> Multiply::clone() const {
//    return std::make_unique<Multiply>(*this);
//}
//
////std::unique_ptr<Expr> Multiply::calcApproximate() {
////    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate().get())->value * static_cast<Number*>(rChild->approximate().get())->value);
////}
//
//std::unique_ptr<Expr> Multiply::calcapproximate(const paramArgMap& extraMap) {
//    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate(extraMap).get())->value * static_cast<Number*>(rChild->approximate(extraMap).get())->value);
//}
//
//std::string Multiply::getInfo() const {
//    return "(" + lChild->getInfo() + Symbol::MULTIPLY + rChild->getInfo() + ")";
//}
//
//std::unique_ptr<Expr> Divide::clone() const {
//    return std::make_unique<Divide>(*this);
//}
//
////std::unique_ptr<Expr> Divide::calcApproximate() {
////    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate().get())->value / static_cast<Number*>(rChild->approximate().get())->value);
////}
//
//std::unique_ptr<Expr> Divide::calcapproximate(const paramArgMap& extraMap) {
//    return std::make_unique<Number>(static_cast<Number*>(lChild->approximate(extraMap).get())->value / static_cast<Number*>(rChild->approximate(extraMap).get())->value);
//}
//
//std::string Divide::getInfo() const {
//    return "(" + lChild->getInfo() + Symbol::DIVIDE + rChild->getInfo() + ")";
//}
//
//std::unique_ptr<Expr> Power::clone() const {
//    return std::make_unique<Power>(*this);
//}
//
////std::unique_ptr<Expr> Power::calcApproximate() {
////    return std::make_unique<Number>(pow(static_cast<Number*>(lChild->approximate().get())->value, static_cast<Number*>(rChild->approximate().get())->value));
////}
//
//std::unique_ptr<Expr> Power::calcapproximate(const paramArgMap& extraMap) {
//    return std::make_unique<Number>(pow(static_cast<Number*>(lChild->approximate(extraMap).get())->value, static_cast<Number*>(rChild->approximate(extraMap).get())->value));
//}
//
//std::string Power::getInfo() const {
//    return "(" + lChild->getInfo() + Symbol::POWER + rChild->getInfo() + ")";
//}