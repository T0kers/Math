#include "treeGeneration.h"

int main() {
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

        try {
            tree = generateTree(user_input, start, user_input.length());
            std::cout << "tree: " << *tree << "\n";
            Expr::paramArgMap extraMap;
            result = tree->evaluate(extraMap);
            std::cout << result->getInfo() << "\n";
        }
        catch(const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
        
    }

    return 0;
}