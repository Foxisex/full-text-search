#include <CLI/CLI.hpp>
#include <iostream>
#include <libfts/indexer.hpp>
#include <libfts/parser.hpp>
#include <libfts/searcher.hpp>

int main() {
    prsr::config config;
    config.stop_words = {
        "a",   "an",    "and",  "are",   "as",    "at",   "be",   "but",
        "by",  "for",   "if",   "in",    "into",  "is",   "it",   "no",
        "not", "of",    "on",   "or",    "s",     "such", "t",    "that",
        "the", "their", "then", "there", "these", "they", "this", "to",
        "was", "will",  "with", "dr",    "mr"};
    config.min_ngram_length = 3;
    config.max_ngram_length = 6;

    indexer::IndexBuilder builder(config);

    builder.add_document(199903, "The Matrix");
    builder.add_document(199904, "The Matrix");
    builder.add_document(199905, "The Matrix");
    builder.add_document(199906, "The Matrix");
    builder.add_document(199907, "The Matrix");
    builder.add_document(199908, "The Matrix");
    builder.add_document(200305, "The Matrix Reloaded Matrix Matrix matrix");
    builder.add_document(200311, "The Matrix Revolution");

    indexer::Index index = builder.index();
    indexer::TextIndexWriter writer;

    writer.write(std::filesystem::current_path(), index);
    searcher::TextIndexAccessor accessor(config);
    std::string query = "Hello Matrix";
    auto result = searcher::search(query, accessor, 8);
    for (const auto& resElement : result) {
        std::cout << "doc: " << resElement.doc_id
                  << " score: " << resElement.score << std::endl;
    }
}