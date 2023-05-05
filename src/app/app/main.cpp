#include <CLI/CLI.hpp>
#include <libfts/parser.hpp>
#include <libfts/solver.hpp>

#include <iostream>

// int main(int argc, char** argv) {
//     CLI::App app{"App description"};

//     double first_operand = 0;
//     double second_operand = 0;

//     app.add_option("--first", first_operand, "First");
//     app.add_option("--second", second_operand, "Second");

//     CLI11_PARSE(app, argc, argv);
//     double result = fts::sum(first_operand, second_operand);
//     std::cout << result << std::endl;

//     return 0;
// }

int main() {
    std::string orig = "Dr. Jekyll and Mr. Hyde";
    std::vector<prsr::ngrams> out;

    prsr::config config;
    config.stop_words = {
        "a",   "an",    "and",  "are",   "as",    "at",   "be",   "but",
        "by",  "for",   "if",   "in",    "into",  "is",   "it",   "no",
        "not", "of",    "on",   "or",    "s",     "such", "t",    "that",
        "the", "their", "then", "there", "these", "they", "this", "to",
        "was", "will",  "with", "dr",    "mr"};
    config.min_ngram_length = 3;
    config.max_ngram_length = 6;

    prsr::parser(orig, config, out);
    for (auto i = out.begin(); i != out.end(); i++)
        std::cout << i->word << " " << i->token_index << " ";
    std::cout << std::endl;
}