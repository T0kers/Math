#include <iostream>
#include <string>
#include <memory>
#include <array>

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
        return "(" + lChild->getInfo() + "+" + rChild->getInfo() + ")";
    }
};

class minus : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override {
        return std::make_unique<constant>(static_cast<constant*>(lChild->evaluate().get())->value - static_cast<constant*>(rChild->evaluate().get())->value);
    }

    std::string getInfo() const override {
        return "(" + lChild->getInfo() + "-" + rChild->getInfo() + ")";
    }
};

class multiply : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override {
        return std::make_unique<constant>(static_cast<constant*>(lChild->evaluate().get())->value * static_cast<constant*>(rChild->evaluate().get())->value);
    }

    std::string getInfo() const override {
        return "(" + lChild->getInfo() + "*" + rChild->getInfo() + ")";
    }
};

class divide : public operation {
public:
    using operation::operation;

    std::unique_ptr<expr> evaluate() override {
        return std::make_unique<constant>(static_cast<constant*>(lChild->evaluate().get())->value / static_cast<constant*>(rChild->evaluate().get())->value);
    }

    std::string getInfo() const override {
        return "(" + lChild->getInfo() + "/" + rChild->getInfo() + ")";
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

std::unique_ptr<expr> generate_tree(const std::string& input, size_t& index) {
    char letter;
    std::unique_ptr<expr> lExpression;
    std::unique_ptr<expr> rExpression;

    while (index < input.length()) {
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
            index++;
            if (!lExpression) {
                lExpression = generate_tree(input, index);
            }
            else {
                rExpression = generate_tree(input, index);
            }
        }

        else if (letter == ')') {
            index++;
            return lExpression;
        }

        else if (letter == '+') {
            index++;
            if (rExpression) {
                lExpression = std::make_unique<plus>(std::move(lExpression), std::move(rExpression));
            }
            rExpression = generate_tree(input, index);
        }

        else if (letter == '-') {
            index++;
            if (rExpression) {
                lExpression = std::make_unique<minus>(std::move(lExpression), std::move(rExpression));
            }
            rExpression = generate_tree(input, index);
        }
        else {
            index++;
        }

        if (rExpression) {
            if (letter == '+') {
                lExpression = std::make_unique<plus>(std::move(lExpression), std::move(rExpression));
            }
            else if (letter == '-') {
                lExpression = std::make_unique<minus>(std::move(lExpression), std::move(rExpression));
            }
            rExpression.reset();
        }
    }
    return lExpression;

}


int main() {
    std::string user_input;
    std::cout << "Type math question:\n> ";
    std::cin >> user_input;

    size_t start = 0;
    std::unique_ptr<expr> tree = generate_tree(user_input, start);
    std::cout << *tree << "\n";

    double result = static_cast<constant*>(tree->evaluate().get())->value;

    std::cout << result << "\n";

    std::string end;
    std::cin >> end;
    return 0;
}