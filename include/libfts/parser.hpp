#pragma once
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

namespace prsr {
    struct config {
        std::unordered_set<std::string> stop_words;
        int min_ngram_length;
        int max_ngram_length;
    };

    struct ngrams {
        std::string word;
        int token_index;
    };

    bool my_ispunct(char symbol);
    void str_to_words(
        std::string& origin,
        char delim,
        std::vector<std::string>& result_vec);
    void rm_stop_words(
        std::vector<std::string>& result_vec,
        const std::unordered_set<std::string>& stop_words);
    void generate_ngrams(
        const std::vector<std::string>& words,
        const prsr::config& cfg,
        std::vector<prsr::ngrams>& parsed_vec);
    void parser(
        const std::string& origin,
        const prsr::config& cfg,
        std::vector<prsr::ngrams>& parsed_vec);
}  // namespace prsr