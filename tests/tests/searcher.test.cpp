#include <gtest/gtest.h>
#include <libfts/searcher.hpp>

TEST(TestSEARCHER, test) {
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
    builder.add_document(199904, "The Matrix1");
    builder.add_document(200305, "The Matrix Reloaded Matrix Matrix matrix");
    builder.add_document(200311, "The Matrix Revolution");

    indexer::Index index = builder.index();
    indexer::TextIndexWriter writer;

    writer.write(std::filesystem::current_path(), index);

    searcher::TextIndexAccessor accessor(config);
    std::string query = "Hello Matrix";

    auto result = searcher::search(query, accessor);

    const std::vector<std::string> expected_result = {
        "The Matrix Reloaded Matrix Matrix matrix",
        "The Matrix",
        "The Matrix1",
        "The Matrix Revolution"};
    std::vector<std::string> resVec;
    resVec.reserve(expected_result.size());
    for (const auto& resElement : result) {
        resVec.emplace_back(
            accessor.load_document(std::to_string(resElement.doc_id)));
    }
    for (size_t i = 0; i < expected_result.size(); i++) {
        ASSERT_EQ(resVec[i], expected_result[i]);
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
