#include "Number.h"
#include "Tree.h"

std::string doubleToString(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(5) << value;
    std::string str = oss.str();
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    if (str.back() == '.') {
        str.pop_back();
    }
    return str;
} // måske hjælpefunktioner i singleton, hvor man for eksempel kan have variabel til præcision.

Number::Number(double value)
    : value(value) {}

Number::Number(const Number& obj)
    : value(obj.value) {}

std::unique_ptr<Expr> Number::clone() const {
    return std::make_unique<Number>(*this);
}

//std::unique_ptr<Expr> Number::approximate() {
//    return std::make_unique<Number>(value);
//}

std::unique_ptr<Expr> Number::approximate(const paramArgMap& extraMap) {
    return std::make_unique<Number>(value);
}

//std::unique_ptr<Expr> Number::evaluate() {
//    return std::make_unique<Number>(value);
//}

std::unique_ptr<Expr> Number::evaluate(const paramArgMap& extraMap) {
    return std::make_unique<Number>(value);
}

std::string Number::getInfo() const {
    return doubleToString(value);
}