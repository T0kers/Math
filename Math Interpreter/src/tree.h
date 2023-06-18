#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>

enum op {
    ERROR = -1, PLUS = 0, MINUS, MULTIPLY, DIVIDE, POWER, lPAREN, rPAREN, ASSIGN, EQUAL
};

extern std::unordered_map<op, std::string> opToStr;
extern std::unordered_map<std::string, op> strToOp;

bool isOpEqual(char letter, op oper);

std::string operator+(const std::string& str, op op);

class expr {
public:
    virtual std::unique_ptr<expr> approximate() = 0; // the method you call to calculate, its purpose is to check for errors.
    virtual std::unique_ptr<expr> calcApproximate() = 0; // after error checks approximate() calls calcApproximate()
    virtual std::string getInfo() const = 0;
};

class error : public expr {
public:
    std::string msg;

    error(std::string msg);

    std::unique_ptr<expr> approximate() override;
    std::unique_ptr<expr> calcApproximate() override;

    std::string getInfo() const override;
};

bool isError(std::unique_ptr<expr>& textExpr);

class identifierPtr : public expr {
public:
    std::shared_ptr<expr> child;

    identifierPtr(std::shared_ptr<expr> ptr);

    std::unique_ptr<expr> approximate() override;
    std::unique_ptr<expr> calcApproximate() override;

    std::string getInfo() const override;
};

class identifier : public expr {
public:
    std::string name;

    identifier(std::string name);

    std::unique_ptr<expr> approximate() override;
    std::unique_ptr<expr> calcApproximate() override;

    std::string getInfo() const override;
};

extern std::unordered_map<std::string, double> constantMap;
extern std::unordered_map<std::string, std::shared_ptr<expr>> variableMap;

class assignment : public expr {
public:
    std::unique_ptr<identifier> variable;
    std::unique_ptr<expr> value;

    assignment(std::unique_ptr<identifier> var, std::unique_ptr<expr> val);

    std::unique_ptr<expr> approximate() override;
    std::unique_ptr<expr> calcApproximate() override;

    std::string getInfo() const override;
};

class operation : public expr {
public:
    std::unique_ptr<expr> lChild;
    std::unique_ptr<expr> rChild;

    operation(std::unique_ptr<expr> lChild, std::unique_ptr<expr> rChild);

    std::unique_ptr<expr> approximate() override;
};

std::string doubleToString(double value);

class number : public expr {
public:
    double value;

    number(double value);

    std::unique_ptr<expr> approximate() override;
    std::unique_ptr<expr> calcApproximate() override;

    std::string getInfo() const override;
};

class plus : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> calcApproximate() override;

    std::string getInfo() const override;
};

class minus : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> calcApproximate() override;

    std::string getInfo() const override;
};

class multiply : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> calcApproximate() override;

    std::string getInfo() const override;
};

class divide : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> calcApproximate() override;

    std::string getInfo() const override;
};

class power : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> calcApproximate() override;

    std::string getInfo() const override;
};

std::ostream& operator<<(std::ostream& out, expr const& data);