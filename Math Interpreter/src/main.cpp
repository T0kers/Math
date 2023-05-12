#include <iostream>
#include <string>
#include <memory>
#include <array>

enum class operators : char {
    PLUS = '+', MINUS = '-', MULTIPLY = '*', DIVIDE = '/', POWER = '^'
};

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
        return "(" + lChild->getInfo() + (char)operators::PLUS + rChild->getInfo() + ")";
    }
};

class minus : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override {
        return std::make_unique<constant>(static_cast<constant*>(lChild->evaluate().get())->value - static_cast<constant*>(rChild->evaluate().get())->value);
    }

    std::string getInfo() const override {
        return "(" + lChild->getInfo() + (char)operators::MINUS + rChild->getInfo() + ")";
    }
};

class multiply : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override {
        return std::make_unique<constant>(static_cast<constant*>(lChild->evaluate().get())->value * static_cast<constant*>(rChild->evaluate().get())->value);
    }

    std::string getInfo() const override {
        return "(" + lChild->getInfo() + (char)operators::MULTIPLY + rChild->getInfo() + ")";
    }
};

class divide : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override {
        return std::make_unique<constant>(static_cast<constant*>(lChild->evaluate().get())->value / static_cast<constant*>(rChild->evaluate().get())->value);
    }

    std::string getInfo() const override {
        return "(" + lChild->getInfo() + (char)operators::DIVIDE + rChild->getInfo() + ")";
    }
};

std::ostream& operator<<(std::ostream& out, expr const& data) {
    out << data.getInfo();
    return out;
}


std::unique_ptr<constant> find_number(const std::string& input, size_t& index) {
    double number = 0;
    for (index; index < input.length() && isdigit(input[index]); index++) {
        number = number * 10 + (input[index] - '0');
    }

    return std::make_unique<constant>(number);
}

std::unique_ptr<expr> generate_tree(const std::string& input, size_t& index, size_t end);

char operation_importance(char c) {
    switch (c)
    {
    case '+':
    case '-':
        return 0;
        break;
    case '*':
    case '/':
        return 1;
        break;
    case '^':
        return 2;
        break;
    default:
        return -1;
        break;
    }
}

std::unique_ptr<expr> find_part(const std::string& input, size_t& index, int& importance) {
    char letter = input[index];
    size_t search_index = index;
    if (letter == '(') {
        index++;
        uint8_t parenthesis_count = 1;
        while (parenthesis_count != 0) {
            search_index++;
            char letter = input[search_index];
            if (letter == '(') {
                parenthesis_count += 1;
            }
            else if (letter == ')') {
                parenthesis_count -= 1;
            }
        }
        search_index++;
        std::cout << search_index << "\n";
        std::unique_ptr<expr> test = generate_tree(input, index, search_index);
        std::cout << test->getInfo() << "\n";
        return test;
    }
    else {
        while (search_index < input.length()) {
            char letter = input[search_index];
            if (letter == '+' || letter == '-' || letter == ')') {
                return generate_tree(input, index, search_index);
            }
            search_index++;
        }
        return generate_tree(input, index, input.length());
    }
}



std::unique_ptr<expr> generate_tree(const std::string& input, size_t& index, size_t end) {
    char letter;
    std::unique_ptr<expr> lExpression;
    std::unique_ptr<expr> rExpression;

    while (index < end) {
        letter = input[index];

        if (isdigit(letter)) {
            if (!lExpression) {
                lExpression = find_number(input, index);
            }
            else {
                rExpression = find_number(input, index);
            }
        }
        else if (letter == '(') {
            if (!lExpression) {
                lExpression = find_part(input, index);
            }
        }

        else if (letter == '+') {
            index++;
            rExpression = find_part(input, index);
            lExpression = std::make_unique<plus>(std::move(lExpression), std::move(rExpression));
            rExpression.reset();
        }

        else if (letter == '-') {
            index++;
            if (!lExpression) {
                lExpression = std::make_unique<minus>(std::make_unique<constant>(0), find_part(input, index));
            }
            else {
                rExpression = find_part(input, index);
                lExpression = std::make_unique<minus>(std::move(lExpression), std::move(rExpression));
                rExpression.reset();
            }
        }

        else if (letter == '*') {
            index++;
            rExpression = find_part(input, index);
            lExpression = std::make_unique<multiply>(std::move(lExpression), std::move(rExpression));
            rExpression.reset();
        }

        else if (letter == '/') {
            index++;
            rExpression = find_part(input, index);
            lExpression = std::make_unique<divide>(std::move(lExpression), std::move(rExpression));
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
        std::cin >> user_input;

        if (user_input == "end") {
            break;
        }

        size_t start = 0;
        std::unique_ptr<expr> tree = generate_tree(user_input, start, user_input.length());
        std::cout << *tree << "\n";

        result = static_cast<constant*>(tree->evaluate().get())->value;

        std::cout << result << "\n";
    }

    return 0;
}