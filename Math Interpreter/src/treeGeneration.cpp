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

std::unique_ptr<expr> findIdentifier(const std::string& input, size_t& index) {
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

    return std::make_unique<identifier>(name);
}

int8_t operationImportance(op c) {
    switch (c)
    {
    case op::ASSIGN:
        return -1;
        break;
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

int8_t operationImportance(std::string ch) {
    if (strToOp.find(ch) != strToOp.end()) {
        return operationImportance(strToOp.at(ch));
    }
    else {
        return operationImportance(op::ERROR);
    }
}

int8_t operationImportance(char ch) {
    return operationImportance(std::string(1, ch));
}

std::unique_ptr<expr> findPart(const std::string& input, size_t& index, op operation) {
    std::cout << "operation: " << opToStr.at(operation) << "\n";
    char letter;
    size_t search_index = index;
    int8_t importance = operationImportance(operation);
    while (search_index < input.length()) {
        letter = input[search_index];
        if (isOpEqual(letter, op::lPAREN)) {
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
                
                if (isOpEqual(letter, op::lPAREN)) {
                    parenthesis_count += 1;
                }
                else if (isOpEqual(letter, op::rPAREN)) {
                    parenthesis_count -= 1;
                }
            }
        }
        if (isOpEqual(letter, op::rPAREN)) {
            return generateTree(input, index, search_index);
        }
        else if (importance >= operationImportance(letter)) {
            if (!(isOpEqual(letter, op::MINUS) && operationImportance(input[search_index - 1]) < 3)) {
                return generateTree(input, index, search_index);
            }
        }
        search_index++;
    }
    return generateTree(input, index, search_index);
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
        else if (isOpEqual(letter, op::lPAREN)) {
            if (!lExpression) {
                lExpression = findPart(input, index, op::lPAREN);
                if (isError(lExpression)) {
                    return std::move(lExpression);
                }
            }
        }
        else if (isOpEqual(letter, op::PLUS)) {
            index++;
            rExpression = findPart(input, index, op::PLUS);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            lExpression = std::make_unique<plus>(std::move(lExpression), std::move(rExpression));
        }
        else if (isOpEqual(letter, op::MINUS)) {
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
            }
        }
        else if (isOpEqual(letter, op::MULTIPLY)) {
            index++;
            rExpression = findPart(input, index, op::MULTIPLY);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            lExpression = std::make_unique<multiply>(std::move(lExpression), std::move(rExpression));
            rExpression.reset();
        }
        else if (isOpEqual(letter, op::DIVIDE)) {
            index++;
            rExpression = findPart(input, index, op::DIVIDE);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            lExpression = std::make_unique<divide>(std::move(lExpression), std::move(rExpression));
        }
        else if (isOpEqual(letter, op::POWER)) {
            index++;
            rExpression = findPart(input, index, op::POWER);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            lExpression = std::make_unique<power>(std::move(lExpression), std::move(rExpression));
        }
        else if (input.substr(index, opToStr.at(op::ASSIGN).length()) == opToStr.at(op::ASSIGN)) {
            index += opToStr.at(ASSIGN).length();
            rExpression = findPart(input, index, op::ASSIGN);
            if (isError(rExpression)) {
                return std::move(rExpression);
            }
            auto lIdentifier = dynamic_cast<identifier*>(lExpression.get());
            if (lIdentifier) {
                // lExpression is an identifier
                lExpression = std::make_unique<assignment>(std::unique_ptr<identifier>(static_cast<identifier*>(lExpression.release())), std::move(rExpression));
            }
            else {
                // lExpression is not an identifier
                return std::make_unique<error>("Left side of assignment is not an identifier.");
            }
        }
        else {
            index++;
        }
    }
    return std::move(lExpression);
}