#include <iostream>
#include <string>
#include <memory>
#include <array>
#include <math.h>


enum op : char {
    PLUS = '+', MINUS = '-', MULTIPLY = '*', DIVIDE = '/', POWER = '^', lPAREN = '(', rPAREN = ')'
};

std::string operator+(const std::string& str, op op) {
    return str + static_cast<char>(op);
}

class expr {
public:
    virtual std::unique_ptr<expr> evaluate() = 0;
    virtual std::string getInfo() const = 0;
};

class operation : public expr {
public:
    std::unique_ptr<expr> lChild;
    std::unique_ptr<expr> rChild;

    operation(std::unique_ptr<expr> lChild, std::unique_ptr<expr> rChild)
        : lChild(std::move(lChild)), rChild(std::move(rChild)) {}

};

class constant : public expr {
public:
    double value;

    constant(double value)
        : value(value) {}

    std::unique_ptr<expr> evaluate() override {
        return std::make_unique<constant>(value);
    }

    std::string getInfo() const override {
        return std::to_string(value);
    }
};

class plus : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override {
        return std::make_unique<constant>(static_cast<constant*>(lChild->evaluate().get())->value + static_cast<constant*>(rChild->evaluate().get())->value);
    }

    std::string getInfo() const override {
        return "(" + lChild->getInfo() + op::PLUS + rChild->getInfo() + ")";
    }
};

class minus : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override {
        return std::make_unique<constant>(static_cast<constant*>(lChild->evaluate().get())->value - static_cast<constant*>(rChild->evaluate().get())->value);
    }

    std::string getInfo() const override {
        return "(" + lChild->getInfo() + op::MINUS + rChild->getInfo() + ")";
    }
};

class multiply : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override {
        return std::make_unique<constant>(static_cast<constant*>(lChild->evaluate().get())->value * static_cast<constant*>(rChild->evaluate().get())->value);
    }

    std::string getInfo() const override {
        return "(" + lChild->getInfo() + op::MULTIPLY + rChild->getInfo() + ")";
    }
};

class divide : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override {
        return std::make_unique<constant>(static_cast<constant*>(lChild->evaluate().get())->value / static_cast<constant*>(rChild->evaluate().get())->value);
    }

    std::string getInfo() const override {
        return "(" + lChild->getInfo() + op::DIVIDE + rChild->getInfo() + ")";
    }
};

class power : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override {
        return std::make_unique<constant>(pow(static_cast<constant*>(lChild->evaluate().get())->value, static_cast<constant*>(rChild->evaluate().get())->value));
    }

    std::string getInfo() const override {
        return "(" + lChild->getInfo() + op::POWER + rChild->getInfo() + ")";
    }
};

std::ostream& operator<<(std::ostream& out, expr const& data) {
    out << data.getInfo();
    return out;
}


std::unique_ptr<constant> findNumber(const std::string& input, size_t& index) {
    double number = 0;
    for (index; index < input.length() && isdigit(input[index]); index++) {
        number = number * 10 + (input[index] - '0');
    }

    return std::make_unique<constant>(number);
}

std::unique_ptr<expr> generateTree(const std::string& input, size_t& index, size_t end);

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

/*std::unique_ptr<expr> findPart(const std::string& input, size_t& index, char&& operation) {
    char letter = input[index];
    size_t search_index = index;
    if (letter == '(') {
        index++;
        uint8_t parenthesis_count = 1;
        while (parenthesis_count != 0) {
            search_index++;
            letter = input[search_index];
            if (letter == '(') {
                parenthesis_count += 1;
            }
            else if (letter == ')') {
                parenthesis_count -= 1;
            }
        }
        search_index++;
        return generateTree(input, index, search_index);
    }
    else {
        while (search_index < input.length()) {
            letter = input[search_index];
            if (letter == '+' || letter == '-' || letter == ')') {
                return generateTree(input, index, search_index);
            }
            search_index++;
        }
        return generateTree(input, index, input.length());
        }
    }
}*/

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


int main() {
    std::string user_input;
    double result;

    while (true) {
        std::cout << "Type math question:\n> ";
        std::getline(std::cin, user_input);

        if (user_input == "end") {
            break;
        }

        size_t start = 0;
        std::unique_ptr<expr> tree = generateTree(user_input, start, user_input.length());
        std::cout << *tree << "\n";

        result = static_cast<constant*>(tree->evaluate().get())->value;

        std::cout << result << "\n";
    }

    return 0;
}