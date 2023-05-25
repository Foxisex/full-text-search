#include <CLI/CLI.hpp>
#include <iostream>
#include <libfts/indexer.hpp>
#include <libfts/options.hpp>
#include <libfts/parser.hpp>
#include <libfts/searcher.hpp>
int main(int argc, char** argv) {
    prsr::config config;
    config.stop_words = {
        "a",   "an",    "and",  "are",   "as",    "at",   "be",   "but",
        "by",  "for",   "if",   "in",    "into",  "is",   "it",   "no",
        "not", "of",    "on",   "or",    "s",     "such", "t",    "that",
        "the", "their", "then", "there", "these", "they", "this", "to",
        "was", "will",  "with", "dr",    "mr"};
    config.min_ngram_length = 3;
    config.max_ngram_length = 6;

    try {
        CLI::App app("Full-text search");

        std::string csv_path;
        std::string ind_path;
        std::string query;

        app.add_option("--csv", csv_path, "Path to scv file.");
        app.add_option("--index, -i", ind_path, "Path to index lib")
            ->required();
        app.add_option("--query, -q", query, "Your query");

        app.parse(argc, argv);

        std::string index_path = std::filesystem::current_path();

        if (ind_path != ".") {
            index_path += "/" + ind_path;
        }

        if (!csv_path.empty()) {
            options::indexating(config, csv_path, index_path);
        }

        if (csv_path.empty() && !query.empty()) {
            options::searching(config, query, index_path);
        } else if (csv_path.empty() && query.empty()) {
            options::searching_interactive(config, index_path);
        }

    } catch (const std::exception& error) {
        std::cout << error.what() << "\n";
        return 1;
    }

    return 0;
}