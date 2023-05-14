#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <iomanip>

enum op : char {
    PLUS = '+', MINUS = '-', MULTIPLY = '*', DIVIDE = '/', POWER = '^', lPAREN = '(', rPAREN = ')'
};

std::string operator+(const std::string& str, op op);

class expr {
public:
    virtual std::unique_ptr<expr> evaluate() = 0;
    virtual std::string getInfo() const = 0;
};

class operation : public expr {
public:
    std::unique_ptr<expr> lChild;
    std::unique_ptr<expr> rChild;

    operation(std::unique_ptr<expr> lChild, std::unique_ptr<expr> rChild);
};

std::string doubleToString(double value);

class constant : public expr {
public:
    double value;

    constant(double value);

    std::unique_ptr<expr> evaluate() override;

    std::string getInfo() const override;
};

class plus : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override;

    std::string getInfo() const override;
};

class minus : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override;

    std::string getInfo() const override;
};

class multiply : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override;

    std::string getInfo() const override;
};

class divide : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override;

    std::string getInfo() const override;
};

class power : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override;

    std::string getInfo() const override;
};

std::ostream& operator<<(std::ostream& out, expr const& data);
