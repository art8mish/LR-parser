
#include "parser.hpp"
#include <iostream>

int main() {
    try {
        lr_parser::Parser parser{};

        std::string expression;
        std::getline(std::cin, expression);
        if (!std::cin.good()) {
            std::cout << "Error: incorrect expression" << std::endl;
            return 1;
        }

#ifndef NDEBUG
        auto start_time = std::clock();
#endif
        parser.parse(expression);
#ifndef NDEBUG
        auto duration = std::clock() - start_time;
        std::cout << "Runtime: " << duration << " us" << std::endl;
#endif
        parser.draw_ast();
        return 0;
    } catch (const std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "Unknown error" << std::endl;
        return 2;
    }
}