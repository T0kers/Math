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
} // m�ske hj�lpefunktioner i singleton, hvor man for eksempel kan have variabel til pr�cision.

Number::Number(double value)
    : value(value) {}

Number::Number(const Number& obj)
    : value(obj.value) {}

std::unique_ptr<Expr> Number::clone() const {
    return std::make_unique<Number>(*this);
}

std::unique_ptr<Expr> Number::approximate() {
    return std::make_unique<Number>(value);
}

std::unique_ptr<Expr> Number::approximate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    return this->approximate();
}

std::unique_ptr<Expr> Number::evaluate() {
    return std::make_unique<Number>(value);
}

std::unique_ptr<Expr> Number::evaluate(std::map<std::string, std::unique_ptr<Expr>>& extraMap) {
    return this->evaluate();
}

std::string Number::getInfo() const {
    return doubleToString(value);
}