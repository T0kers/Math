#include "treeGeneration.h"

std::unique_ptr<Number> findNumber(const std::string& input, size_t& index) {
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

    return std::make_unique<Number>(num);
}

std::unique_ptr<Expr> findIdentifier(const std::string& input, size_t& index) {
    std::string name = "";
    char letter;
    while (index < input.length()) {
        letter = input[index];
        if (isalpha(letter)) {
            name = name + letter;
        }
        else {
            break;
        }
        index++;
    }

    return std::make_unique<ConstVar>(name);
}

int8_t operationImportance(Op c) {
    switch (c)
    {
    case Op::ASSIGN:
        return -1;
        break;
    case Op::PLUS:
    case Op::MINUS:
        return 0;
        break;
    case Op::MULTIPLY:
    case Op::DIVIDE:
        return 1;
        break;
    case Op::POWER:
        return 2;
        break;
    case Op::lPAREN:
    case Op::rPAREN:
        return 3;
        break;
    default:
        return 10;
        break;
    }
}

int8_t operationImportance(std::string ch) {
    if (strToOp.find(ch) != strToOp.end()) {
        return operationImportance(strToOp.at(ch));
    }
    else {
        return operationImportance(Op::ERROR);
    }
}

int8_t operationImportance(char ch) {
    return operationImportance(std::string(1, ch));
}

std::unique_ptr<Expr> findPart(const std::string& input, size_t& index, Op Operation) {
    std::cout << "operation: " << opToStr.at(Operation) << "\n";
    char letter;
    size_t search_index = index;
    int8_t importance = operationImportance(Operation);
    while (search_index < input.length()) {
        letter = input[search_index];
        if (isOpEqual(letter, Op::lPAREN)) {
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
                    return std::make_unique<Error>(msg);
                }
                
                if (isOpEqual(letter, Op::lPAREN)) {
                    parenthesis_count += 1;
                }
                else if (isOpEqual(letter, Op::rPAREN)) {
                    parenthesis_count -= 1;
                }
            }
        }
        if (isOpEqual(letter, Op::rPAREN)) {
            return generateTree(input, index, search_index);
        }
        else if (importance >= operationImportance(letter)) {
            if (!(isOpEqual(letter, Op::MINUS) && operationImportance(input[search_index - 1]) < 3)) {
                return generateTree(input, index, search_index);
            }
        }
        search_index++;
    }
    return generateTree(input, index, search_index);
}

std::unique_ptr<Expr> generateTree(const std::string& input, size_t& index, size_t end) {
    char letter;
    std::unique_ptr<Expr> lExpression;
    std::unique_ptr<Expr> rExpression;

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
                lExpression = findIdentifier(input, index);
                if (isError(lExpression)) {
                    return std::move(lExpression);
                }
            }
            else {
                rExpression = findIdentifier(input, index);
                if (isError(rExpression)) {
                    return std::move(rExpression);
                }
            }
        }
        else if (isOpEqual(letter, Op::lPAREN)) {
            if (!lExpression) {
                lExpression = findPart(input, index, Op::lPAREN);
                if (isError(lExpression)) {
                    return std::move(lExpression);
                }
            }
        }
        else if (isOpEqual(letter, Op::PLUS)) {
            index++;
            rExpression = findPart(input, index, Op::PLUS);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            lExpression = std::make_unique<Plus>(std::move(lExpression), std::move(rExpression));
        }
        else if (isOpEqual(letter, Op::MINUS)) {
            index++;
            if (!lExpression) {
                lExpression = findPart(input, index, Op::MULTIPLY);
                if (isError(lExpression)) {
                    return std::move(lExpression);
                }
                lExpression = std::make_unique<Multiply>(std::make_unique<Number>(-1), std::move(lExpression));
            }
            else {
                rExpression = findPart(input, index, Op::MINUS);
                if (isError(rExpression)) {
                    return std::move(rExpression);
                }
                lExpression = std::make_unique<Minus>(std::move(lExpression), std::move(rExpression));
            }
        }
        else if (isOpEqual(letter, Op::MULTIPLY)) {
            index++;
            rExpression = findPart(input, index, Op::MULTIPLY);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            lExpression = std::make_unique<Multiply>(std::move(lExpression), std::move(rExpression));
            rExpression.reset();
        }
        else if (isOpEqual(letter, Op::DIVIDE)) {
            index++;
            rExpression = findPart(input, index, Op::DIVIDE);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            lExpression = std::make_unique<Divide>(std::move(lExpression), std::move(rExpression));
        }
        else if (isOpEqual(letter, Op::POWER)) {
            index++;
            rExpression = findPart(input, index, Op::POWER);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            lExpression = std::make_unique<Power>(std::move(lExpression), std::move(rExpression));
        }
        else if (input.substr(index, opToStr.at(Op::ASSIGN).length()) == opToStr.at(Op::ASSIGN)) {
            index += opToStr.at(Op::ASSIGN).length();
            rExpression = findPart(input, index, Op::ASSIGN);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            auto lIdentifier = dynamic_cast<Identifier*>(lExpression.get());
            if (lIdentifier) {
                // lExpression is an identifier
                lExpression = std::make_unique<Assignment>(std::unique_ptr<Identifier>(static_cast<Identifier*>(lExpression.release())), std::move(rExpression));
            }
            else {
                // lExpression is not an identifier
                return std::make_unique<Error>("Left side of assignment is not an identifier.");
            }
        }
        else {
            index++;
        }
    }
    return std::move(lExpression);
}