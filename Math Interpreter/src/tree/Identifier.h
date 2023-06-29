#pragma once
#include "Expr.h"

class Identifier : public Expr {
public:
    virtual const std::string& getName() = 0;
    virtual std::unique_ptr<Expr> clone() const = 0;
    virtual std::unique_ptr<Identifier> cloneIdentifier() const = 0;
};