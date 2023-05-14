#include "tree.h"

std::string operator+(const std::string& str, op op) {
    return str + static_cast<char>(op);
}

error::error(std::string msg)
    : msg(msg) {}

std::unique_ptr<expr> error::evaluate() {
    return std::make_unique<error>(msg);
}

std::string error::getInfo() const {
    return msg;
}

operation::operation(std::unique_ptr<expr> lChild, std::unique_ptr<expr> rChild)
    : lChild(std::move(lChild)), rChild(std::move(rChild)) {}

std::string doubleToString(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    std::string str = oss.str();
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    if (str.back() == '.') {
        str.pop_back();
    }
    return str;
}

number::number(double value)
    : value(value) {}

std::unique_ptr<expr> number::evaluate() {
    return std::make_unique<number>(value);
}

std::string number::getInfo() const {
    return doubleToString(value);
}

std::unique_ptr<expr> plus::evaluate() {
    return std::make_unique<number>(static_cast<number*>(lChild->evaluate().get())->value + static_cast<number*>(rChild->evaluate().get())->value);
}

std::string plus::getInfo() const {
    return "(" + lChild->getInfo() + op::PLUS + rChild->getInfo() + ")";
}

std::unique_ptr<expr> minus::evaluate() {
    return std::make_unique<number>(static_cast<number*>(lChild->evaluate().get())->value - static_cast<number*>(rChild->evaluate().get())->value);
}

std::string minus::getInfo() const {
    return "(" + lChild->getInfo() + op::MINUS + rChild->getInfo() + ")";
}

std::unique_ptr<expr> multiply::evaluate() {
    return std::make_unique<number>(static_cast<number*>(lChild->evaluate().get())->value * static_cast<number*>(rChild->evaluate().get())->value);
}

std::string multiply::getInfo() const {
    return "(" + lChild->getInfo() + op::MULTIPLY + rChild->getInfo() + ")";
}

std::unique_ptr<expr> divide::evaluate() {
    return std::make_unique<number>(static_cast<number*>(lChild->evaluate().get())->value / static_cast<number*>(rChild->evaluate().get())->value);
}

std::string divide::getInfo() const {
    return "(" + lChild->getInfo() + op::DIVIDE + rChild->getInfo() + ")";
}

std::unique_ptr<expr> power::evaluate() {
    return std::make_unique<number>(pow(static_cast<number*>(lChild->evaluate().get())->value, static_cast<number*>(rChild->evaluate().get())->value));
}

std::string power::getInfo() const {
    return "(" + lChild->getInfo() + op::POWER + rChild->getInfo() + ")";
}

std::ostream& operator<<(std::ostream& out, expr const& data) {
    out << data.getInfo();
    return out;
}