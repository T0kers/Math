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
            if (isSymbolEqual(letter, Symbol::lPAREN)) {
                return findPart(input, index, Symbol::FUNCTION, name);
            }
            break;
        }
        index++;
    }

    return std::make_unique<ConstVar>(name);
}

int8_t operationImportance(Symbol c) {
    switch (c)
    {
    case Symbol::ASSIGN:
        return -1;
        break;
    case Symbol::PLUS:
    case Symbol::MINUS:
        return 0;
        break;
    case Symbol::MULTIPLY:
    case Symbol::DIVIDE:
        return 1;
        break;
    case Symbol::POWER:
        return 2;
        break;
    case Symbol::lPAREN:
    case Symbol::rPAREN:
        return 3;
        break;
    default:
        return 10;
        break;
    }
}

int8_t operationImportance(std::string ch) {
    return operationImportance(convertSymbol(ch));
}

int8_t operationImportance(char ch) {
    return operationImportance(std::string(1, ch));
}

std::unique_ptr<Expr> findPart(const std::string& input, size_t& index, Symbol Operation, const std::string& name) {
    std::cout << "operation: " << convertSymbol(Operation) << "\n";
    char letter;
    size_t search_index = index;
    int8_t importance = operationImportance(Operation);
    while (search_index < input.length()) {
        letter = input[search_index];
        if (isSymbolEqual(letter, Symbol::lPAREN)) {
            if (search_index == index) {
                index++;
            }
            uint8_t parenthesis_count = 1;
            while (parenthesis_count != 0) {
                search_index++;
                if (search_index > input.length()) {
                    throw std::exception("Parentheses are not matching.");
                }
                else {
                    letter = input[search_index];
                }
                
                if (isSymbolEqual(letter, Symbol::lPAREN)) {
                    parenthesis_count += 1;
                }
                else if (isSymbolEqual(letter, Symbol::rPAREN)) {
                    parenthesis_count -= 1;
                }
            }
        }
        if (isSymbolEqual(letter, Symbol::rPAREN)) {
            if (Operation == Symbol::FUNCTION) {
                return generateFunction(input, index, search_index, name);
            }
            return generateTree(input, index, search_index);
        }
        else if (importance >= operationImportance(letter)) {
            if (!(isSymbolEqual(letter, Symbol::MINUS) && operationImportance(input[search_index - 1]) < 3)) {
                return generateTree(input, index, search_index);
            }
        }
        search_index++;
    }
    return generateTree(input, index, search_index);
}

std::unique_ptr<Expr> generateFunction(const std::string& input, size_t& index, size_t end, const std::string& funcName) {
    size_t i = 0;
    std::vector<std::unique_ptr<Expr>> args;
    std::unique_ptr<Expr> arg;
    std::stringstream ss(input.substr(index, end - index));
    std::string sArg;
    while (!ss.eof()) {
        getline(ss, sArg, ','); // TODO: Get comma out of here
        i = 0;
        arg = generateTree(sArg, i, sArg.length());
        if (arg) {
            args.push_back(std::move(arg));
        }
        else {
            std::ostringstream e;
            e << "Illegal arguments in function: '" << funcName << "'";
            auto er = e.str();
            throw std::exception(er.c_str());
        }
    }
    index = end;

    for (const auto& i : args) {
        std::cout << i->getInfo() << std::endl;
    }

    return std::make_unique<Function>(funcName, args);
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
            }
            else {
                rExpression = findIdentifier(input, index);
            }
        }
        else if (isSymbolEqual(letter, Symbol::lPAREN)) {
            if (!lExpression) {
                lExpression = findPart(input, index, Symbol::lPAREN);
            }
        }
        else if (isSymbolEqual(letter, Symbol::PLUS)) {
            index++;
            rExpression = findPart(input, index, Symbol::PLUS);
            lExpression = std::make_unique<NewOperation>(NewOperation::Operator::addition, std::move(lExpression), std::move(rExpression));
        }
        else if (isSymbolEqual(letter, Symbol::MINUS)) {
            index++;
            if (!lExpression) {
                lExpression = findPart(input, index, Symbol::MULTIPLY);
                lExpression = std::make_unique<NewOperation>(NewOperation::Operator::multiplication, std::make_unique<Number>(-1), std::move(lExpression));
            }
            else {
                rExpression = findPart(input, index, Symbol::MINUS);
                lExpression = std::make_unique<NewOperation>(NewOperation::Operator::subtraction, std::move(lExpression), std::move(rExpression));
            }
        }
        else if (isSymbolEqual(letter, Symbol::MULTIPLY)) {
            index++;
            rExpression = findPart(input, index, Symbol::MULTIPLY);
            lExpression = std::make_unique<NewOperation>(NewOperation::Operator::multiplication, std::move(lExpression), std::move(rExpression));
        }
        else if (isSymbolEqual(letter, Symbol::DIVIDE)) {
            index++;
            rExpression = findPart(input, index, Symbol::DIVIDE);
            lExpression = std::make_unique<NewOperation>(NewOperation::Operator::division, std::move(lExpression), std::move(rExpression));
        }
        else if (isSymbolEqual(letter, Symbol::POWER)) {
            index++;
            rExpression = findPart(input, index, Symbol::POWER);
            lExpression = std::make_unique<NewOperation>(NewOperation::Operator::exponentiation, std::move(lExpression), std::move(rExpression));
        }
        else if (input.substr(index, convertSymbol(Symbol::ASSIGN).length()) == convertSymbol(Symbol::ASSIGN)) {
            index += convertSymbol(Symbol::ASSIGN).length();
            rExpression = findPart(input, index, Symbol::ASSIGN);
            auto lIdentifier = dynamic_cast<Identifier*>(lExpression.get());
            if (lIdentifier) {
                // lExpression is an identifier
                lExpression = std::make_unique<Assignment>(std::unique_ptr<Identifier>(static_cast<Identifier*>(lExpression.release())), std::move(rExpression));
            }
            else {
                // lExpression is not an identifier
                throw std::exception("Left side of assignment is not an identifier.");
            }
        }
        else {
            index++;
        }
    }
    return std::move(lExpression);
}