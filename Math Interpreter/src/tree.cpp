#include "tree.h"

std::unordered_map<op, std::string> opToStr = {
    {PLUS, "+"},
    {MINUS, "-"},
    {MULTIPLY, "*"},
    {DIVIDE, "/"},
    {POWER, "^"},
    {lPAREN, "("},
    {rPAREN, ")"},
    {ASSIGN, ":="},
    {EQUAL, "="}
};

std::unordered_map<std::string, op> strToOp = {
    {"+", PLUS},
    {"-", MINUS},
    {"*", MULTIPLY},
    {"/", DIVIDE},
    {"^", POWER},
    {"(", lPAREN},
    {")", rPAREN},
    {":=", ASSIGN},
    {"=", EQUAL}
};

bool isOpEqual(char letter, op oper) {
    return opToStr.at(oper) == std::string(1, letter);
}

std::string operator+(const std::string& str, op oper) {
    return str + opToStr.at(oper);
}

error::error(std::string msg)
    : msg(msg) {}

std::unique_ptr<expr> error::approximate() {
    return std::make_unique<error>(msg);
}

std::string error::getInfo() const {
    return msg;
}

bool isError(std::unique_ptr<expr>& textExpr) {
    return dynamic_cast<error*>(textExpr.get()) != nullptr;
}

identifierPtr::identifierPtr(std::shared_ptr<expr> ptr)
    : child(ptr) {}

std::unique_ptr<expr> identifierPtr::approximate() {
    return child->approximate();
}

std::string identifierPtr::getInfo() const {
    return child->getInfo();
}

identifier::identifier(std::string name)
    : name(name) {}

std::unique_ptr<expr> identifier::approximate() {
    if (constantMap.find(name) != constantMap.end()) {
        return std::make_unique<number>(constantMap.at(name));
    }
    else if (variableMap.find(name) != variableMap.end()) {
        return variableMap.at(name)->approximate();
    }
    return std::make_unique<error>("Identifier does not exist");
}

std::string identifier::getInfo() const {
    return name;
}

std::unordered_map<std::string, double> constantMap{
    {"pi", 3.1415926535},
    {"e", 2.71828182845},
};

std::unordered_map<std::string, std::shared_ptr<expr>> variableMap;

assignment::assignment(std::unique_ptr<identifier> var, std::unique_ptr<expr> val)
    : variable(std::move(var)), value(std::move(val)) {}

std::unique_ptr<expr> assignment::approximate() {
    std::string name = variable->getInfo();
    if (constantMap.find(name) != constantMap.end()) {
        return std::make_unique<error>("This identifier name is protected.");
    }
    variableMap[name] = std::move(value);
    
    for (const auto& i : variableMap) {
        std::cout << i.first + " " + i.second->getInfo() << "\n";
    }

    return std::make_unique<identifierPtr>(variableMap.at(name));
}

std::string assignment::getInfo() const {
    return variable->getInfo() + " " + op::ASSIGN + " " + value->getInfo();
}

operation::operation(std::unique_ptr<expr> lChild, std::unique_ptr<expr> rChild)
    : lChild(std::move(lChild)), rChild(std::move(rChild)) {}

std::unique_ptr<expr> operation::approximate() {
    std::string errorMsg = "";

    std::unique_ptr<expr> left = lChild->approximate();
    std::unique_ptr<expr> right = rChild->approximate();

    if (isError(left)) {
        errorMsg += left->getInfo() + "\n";
    }
    if (isError(right)) {
        errorMsg += right->getInfo() + "\n";
    }
    if (errorMsg.empty()) {
        return this->calcApproximate();
    }
    else {
        return std::make_unique<error>(errorMsg);
    }
}

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

std::unique_ptr<expr> number::approximate() {
    return std::make_unique<number>(value);
}

std::string number::getInfo() const {
    return doubleToString(value);
}

std::unique_ptr<expr> plus::calcApproximate() {
    return std::make_unique<number>(static_cast<number*>(lChild->approximate().get())->value + static_cast<number*>(rChild->approximate().get())->value);
}

std::string plus::getInfo() const {
    return "(" + lChild->getInfo() + op::PLUS + rChild->getInfo() + ")";
}

std::unique_ptr<expr> minus::calcApproximate() {
    return std::make_unique<number>(static_cast<number*>(lChild->approximate().get())->value - static_cast<number*>(rChild->approximate().get())->value);
}

std::string minus::getInfo() const {
    return "(" + lChild->getInfo() + op::MINUS + rChild->getInfo() + ")";
}

std::unique_ptr<expr> multiply::calcApproximate() {
    return std::make_unique<number>(static_cast<number*>(lChild->approximate().get())->value * static_cast<number*>(rChild->approximate().get())->value);
}

std::string multiply::getInfo() const {
    return "(" + lChild->getInfo() + op::MULTIPLY + rChild->getInfo() + ")";
}

std::unique_ptr<expr> divide::calcApproximate() {
    return std::make_unique<number>(static_cast<number*>(lChild->approximate().get())->value / static_cast<number*>(rChild->approximate().get())->value);
}

std::string divide::getInfo() const {
    return "(" + lChild->getInfo() + op::DIVIDE + rChild->getInfo() + ")";
}

std::unique_ptr<expr> power::calcApproximate() {
    return std::make_unique<number>(pow(static_cast<number*>(lChild->approximate().get())->value, static_cast<number*>(rChild->approximate().get())->value));
}

std::string power::getInfo() const {
    return "(" + lChild->getInfo() + op::POWER + rChild->getInfo() + ")";
}

std::ostream& operator<<(std::ostream& out, expr const& data) {
    out << data.getInfo();
    return out;
}