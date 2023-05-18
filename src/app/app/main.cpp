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
    builder.add_document(200305, "The Matrix Reloaded");
    builder.add_document(200311, "The Matrix Revolution");

    indexer::Index index = builder.index();
    indexer::TextIndexWriter writer;

    writer.write(std::filesystem::current_path(), index);
    searcher::TextIndexAccessor accessor(config);
    std::string str = "199903";
    std::string file = accessor.load_document("199903");
    std::cout << file << std::endl;
}