#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

enum op : char {
    PLUS = '+', MINUS = '-', MULTIPLY = '*', DIVIDE = '/', POWER = '^', lPAREN = '(', rPAREN = ')'
};

std::string operator+(const std::string& str, op op);

class expr {
public:
    virtual std::unique_ptr<expr> approximate() = 0;
    virtual std::string getInfo() const = 0;
};

class error : public expr {
public:
    std::string msg;

    error(std::string msg);

    std::unique_ptr<expr> approximate() override;

    std::string getInfo() const override;
};

class operation : public expr {
public:
    std::unique_ptr<expr> lChild;
    std::unique_ptr<expr> rChild;

    operation(std::unique_ptr<expr> lChild, std::unique_ptr<expr> rChild);
};

std::string doubleToString(double value);

class number : public expr {
public:
    double value;

    number(double value);

    std::unique_ptr<expr> approximate() override;

    std::string getInfo() const override;
};

class constant : public expr {
public:
    std::string name;
    double value;

    constant(std::string name, double value);

    std::unique_ptr<expr> approximate() override;

    std::string getInfo() const override;

};

class plus : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> approximate() override;

    std::string getInfo() const override;
};

class minus : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> approximate() override;

    std::string getInfo() const override;
};

class multiply : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> approximate() override;

    std::string getInfo() const override;
};

class divide : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> approximate() override;

    std::string getInfo() const override;
};

class power : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> approximate() override;

    std::string getInfo() const override;
};

std::ostream& operator<<(std::ostream& out, expr const& data);
