#include "treeGeneration.h"

std::unique_ptr<constant> findNumber(const std::string& input, size_t& index) {
    double number = 0;
    for (index; index < input.length() && isdigit(input[index]); index++) {
        number = number * 10 + (input[index] - '0');
    }

    return std::make_unique<constant>(number);
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
    default:
        return 10;
        break;
    }
}

std::unique_ptr<expr> findPart(const std::string& input, size_t& index, char operation) {
    char letter;
    size_t search_index = index;
    int8_t importance = operationImportance(operation);
    while (search_index < input.length()) {
        letter = input[search_index];
        if (letter == op::lPAREN) {
            index++;
            uint8_t parenthesis_count = 1;
            while (parenthesis_count != 0) {
                search_index++;
                letter = input[search_index];
                if (letter == op::lPAREN) {
                    parenthesis_count += 1;
                }
                else if (letter == op::rPAREN) {
                    parenthesis_count -= 1;
                }
            }
        }
        else if (importance >= operationImportance(letter)) {
            return generateTree(input, index, search_index);
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

        if (isdigit(letter)) {
            if (!lExpression) {
                lExpression = findNumber(input, index);
            }
            else {
                rExpression = findNumber(input, index);
            }
        }
        else if (letter == op::lPAREN) {
            if (!lExpression) {
                lExpression = findPart(input, index, op::lPAREN);
            }
        }

        else if (letter == op::PLUS) {
            index++;
            rExpression = findPart(input, index, op::PLUS);
            lExpression = std::make_unique<plus>(std::move(lExpression), std::move(rExpression));
            rExpression.reset();
        }

        else if (letter == op::MINUS) {
            index++;
            if (!lExpression) {
                lExpression = std::make_unique<minus>(std::make_unique<constant>(0), findPart(input, index, op::MINUS));
            }
            else {
                rExpression = findPart(input, index, op::MINUS);
                lExpression = std::make_unique<minus>(std::move(lExpression), std::move(rExpression));
                rExpression.reset();
            }
        }

        else if (letter == op::MULTIPLY) {
            index++;
            rExpression = findPart(input, index, op::MULTIPLY);
            lExpression = std::make_unique<multiply>(std::move(lExpression), std::move(rExpression));
            rExpression.reset();
        }

        else if (letter == op::DIVIDE) {
            index++;
            rExpression = findPart(input, index, op::DIVIDE);
            lExpression = std::make_unique<divide>(std::move(lExpression), std::move(rExpression));
            rExpression.reset();
        }
        else if (letter == op::POWER) {
            index++;
            rExpression = findPart(input, index, op::POWER);
            lExpression = std::make_unique<power>(std::move(lExpression), std::move(rExpression));
            rExpression.reset();
        }

        else {
            index++;
        }
    }
    return lExpression;

}