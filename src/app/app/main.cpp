#include <CLI/CLI.hpp>
#include <iostream>
#include <libfts/parser.hpp>

int main() {
    std::string orig = "Dr. Jekyllll and Mr. the the Hyde";
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
    for (auto& ngram : out) {
        std::cout << ngram.word << " " << ngram.token_index << " ";
    }
    std::cout << std::endl;
}