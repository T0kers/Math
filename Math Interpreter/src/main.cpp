#include "treeGeneration.h"

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
        std::cout << "tree: " << *tree << "\n";

        result = static_cast<number*>(tree->evaluate().get())->value;

        std::cout << "result: " << result << "\n";
    }

    return 0;
}