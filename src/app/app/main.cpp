#include <libfts/solver.hpp>

#include <CLI/CLI.hpp>

#include <iostream>

int main(int argc, char** argv) {
    CLI::App app{"App description"};

    double first_operand = 0;
    double second_operand = 0;

    app.add_option("--first", first_operand, "First");
    app.add_option("--second", second_operand, "Second");

    CLI11_PARSE(app, argc, argv);
    double result = fts::sum(first_operand, second_operand);
    std::cout << result << std::endl;

    return 0;
}