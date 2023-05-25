#include <CLI/CLI.hpp>
#include <csv2/reader.hpp>
#include <iostream>
#include <libfts/indexer.hpp>
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

        if (!csv_path.empty()) {
            csv2::Reader<
                csv2::delimiter<','>,
                csv2::quote_character<'"'>,
                csv2::first_row_is_header<true>,
                csv2::trim_policy::trim_whitespace>
                csv;
            std::vector<std::string> strings;

            csv.mmap("books.csv");
            for (const auto row : csv) {
                size_t ind = 0;
                for (const auto cell : row) {
                    if (ind == 0 || ind == 1) {
                        std::string value;
                        cell.read_value(value);
                        strings.emplace_back(value);
                    }
                    ind++;
                }
            }

            indexer::IndexBuilder builder(config);

            for (size_t i = 0; i < strings.size(); i += 2) {
                builder.add_document(std::stoul(strings[i]), strings[i + 1]);
            }

            indexer::Index index = builder.index();
            indexer::TextIndexWriter writer;

            writer.write(std::filesystem::current_path(), index);
        }

        if (csv_path.empty() && !query.empty()) {
            searcher::TextIndexAccessor accessor(config);
            auto result = searcher::search(query, accessor);
            for (const auto& resElement : result) {
                std::cout << "doc: " << resElement.doc_id << " "
                          << accessor.load_document(
                                 std::to_string(resElement.doc_id))
                          << " score: " << resElement.score << std::endl;
            }
        } else {
            std::getline(std::cin, query);
            searcher::TextIndexAccessor accessor(config);
            auto result = searcher::search(query, accessor);
            for (const auto& resElement : result) {
                std::cout << "doc: " << resElement.doc_id << " "
                          << accessor.load_document(
                                 std::to_string(resElement.doc_id))
                          << " score: " << resElement.score << std::endl;
            }
        }

    } catch (const std::exception& error) {
        std::cout << error.what() << "\n";
        return 1;
    }

    return 0;
}