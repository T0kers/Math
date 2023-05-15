#include "treeGeneration.h"

std::unique_ptr<number> findNumber(const std::string& input, size_t& index) {
    double num = 0;
    char val;
    bool isDecimal = false;
    double decimal = 10;
    while (index < input.length()) {
        val = input[index];
        if (isdigit(val)) {
            if (isDecimal) {
                num = num + (val - static_cast<double>('0')) / decimal;
                decimal = decimal * 10;
            }
            else {
                num = num * 10 + (val - '0');
            }
        }
        else if (val == '.') {
            isDecimal = true;
        }
        else {
            break;
        }
        index++;
    }

    return std::make_unique<number>(num);
}

std::unique_ptr<constant> findConstant(const std::string& input, size_t& index) {
    std::string name = "";
    char letter;
    std::unique_ptr<constant> con;
    while (index < input.length()) {
        letter = input[index];
        if (isalpha(letter)) {
            name = name + letter;
        }
        else {
            std::cout << "Const: " << name << "\n";
            if (name == "pi") {
                con = std::make_unique<constant>("pi", 3.1415926535);
            }
            break;
        }
        index++;
    }

    return con;
}

int8_t operationImportance(char c) {
    switch (c)
    {
    case op::PLUS:
    case op::MINUS:
        return 0;
        break;
    case op::MULTIPLY:
    case op::DIVIDE:
        return 1;
        break;
    case op::POWER:
        return 2;
        break;
    case op::lPAREN:
    case op::rPAREN:
        return 3;
        break;
    default:
        return 10;
        break;
    }
}

std::unique_ptr<expr> findPart(const std::string& input, size_t& index, char operation) {
    std::cout << "operation: " << operation << "\n";
    char letter;
    size_t search_index = index;
    int8_t importance = operationImportance(operation);
    while (search_index < input.length()) {
        letter = input[search_index];
        if (letter == op::lPAREN) {
            if (search_index == index) {
                index++;
            }
            uint8_t parenthesis_count = 1;
            while (parenthesis_count != 0) {
                search_index++;
                try {
                    if (search_index > input.length()) {
                        throw std::string("Parentheses are not matching.");
                    }
                    else {
                        letter = input[search_index];
                    }
                }
                catch(std::string& msg) {
                    std::cout << "ERROR:\n";
                    return std::make_unique<error>(msg);
                }
                
                if (letter == op::lPAREN) {
                    parenthesis_count += 1;
                }
                else if (letter == op::rPAREN) {
                    parenthesis_count -= 1;
                }
            }
        }
        if (letter == op::rPAREN) {
            return generateTree(input, index, search_index);
        }
        else if (importance >= operationImportance(letter)) {
            if (letter == op::MINUS && operationImportance(input[search_index - 1]) < 3) {
                std::cout << "hello\n";
            }
            else {
                return generateTree(input, index, search_index);
            }
        }
        search_index++;
    }
    std::cout << index << " " << search_index << "\n";
    return generateTree(input, index, search_index);
}

bool isError(std::unique_ptr<expr>& textExpr) {
    return dynamic_cast<error*>(textExpr.get()) != nullptr;
}

std::unique_ptr<expr> generateTree(const std::string& input, size_t& index, size_t end) {
    char letter;
    std::unique_ptr<expr> lExpression;
    std::unique_ptr<expr> rExpression;

    while (index < end) {
        letter = input[index];

        if (isdigit(letter) || letter == '.') {
            if (!lExpression) {
                lExpression = findNumber(input, index);
            }
            else {
                rExpression = findNumber(input, index);
            }
        }
        else if (isalpha(letter)) {
            if (!lExpression) {
                lExpression = findConstant(input, index);
            }
            else {
                rExpression = findConstant(input, index);
            }
        }
        else if (letter == op::lPAREN) {
            if (!lExpression) {
                lExpression = findPart(input, index, op::lPAREN);
                if (isError(lExpression)) {
                    return std::move(lExpression);
                }
            }
        }
        else if (letter == op::PLUS) {
            index++;
            rExpression = findPart(input, index, op::PLUS);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            lExpression = std::make_unique<plus>(std::move(lExpression), std::move(rExpression));
            rExpression.reset();
        }
        else if (letter == op::MINUS) {
            index++;
            if (!lExpression) {
                lExpression = findPart(input, index, op::MULTIPLY);
                if (isError(lExpression)) {
                    return std::move(lExpression);
                }
                lExpression = std::make_unique<multiply>(std::make_unique<number>(-1), std::move(lExpression));
            }
            else {
                rExpression = findPart(input, index, op::MINUS);
                if (isError(rExpression)) {
                    return std::move(rExpression);
                }
                lExpression = std::make_unique<minus>(std::move(lExpression), std::move(rExpression));
                rExpression.reset();
            }
        }
        else if (letter == op::MULTIPLY) {
            index++;
            rExpression = findPart(input, index, op::MULTIPLY);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            lExpression = std::make_unique<multiply>(std::move(lExpression), std::move(rExpression));
            rExpression.reset();
        }
        else if (letter == op::DIVIDE) {
            index++;
            rExpression = findPart(input, index, op::DIVIDE);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            lExpression = std::make_unique<divide>(std::move(lExpression), std::move(rExpression));
            rExpression.reset();
        }
        else if (letter == op::POWER) {
            index++;
            rExpression = findPart(input, index, op::POWER);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            lExpression = std::make_unique<power>(std::move(lExpression), std::move(rExpression));
            rExpression.reset();
        }
        else {
            index++;
        }
    }
    return std::move(lExpression);
}