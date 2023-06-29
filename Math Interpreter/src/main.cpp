#include "treeGeneration.h"

int main() {
    variableMap.emplace("test", std::make_unique<Number>(20));

    std::string user_input;
    std::unique_ptr<Expr> tree;
    std::unique_ptr<Expr> result;

    while (true) {
        std::cout << "Type math question:\n> ";
        std::getline(std::cin, user_input);

        if (user_input == "end") {
            break;
        }

        size_t start = 0;
        tree = generateTree(user_input, start, user_input.length());
        std::cout << "tree: " << *tree << "\n";

        result = tree->approximate();
        std::cout << result->getInfo() << "\n";
    }

    return 0;
}