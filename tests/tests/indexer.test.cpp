#include <gtest/gtest.h>
#include <libfts/indexer.hpp>

TEST(TestINDEXER, indexBuilder) {
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
    builder.add_document(19999, "The Matrix Matrix");
    builder.add_document(20000, "The Matrix Reloaded Matrix Reloading");

    indexer::Index index = builder.index();

    std::unordered_map<size_t, std::string> expected_docs = {{19999, "The Matrix Matrix"}, {20000, "The Matrix Reloaded Matrix Reloading"}};
    std::unordered_map<std::string, std::unordered_map<size_t, std::vector<int>>> expected_entries =
    {{"reloa", {{20000, {1, 3}}}},
     {"relo", {{20000, {1, 3}}}},
     {"rel", {{20000, {1, 3}}}},
     {"reload", {{20000, {1, 3}}}},
     {"matrix", {{20000, {0, 2}}, {19999, {0,1}}}},
     {"matri", {{20000, {0, 2}}, {19999, {0,1}}}},
     {"matr", {{20000, {0, 2}}, {19999, {0,1}}}},
     {"mat", {{20000, {0, 2}},{19999, {0, 1}}}}};
    ASSERT_EQ(expected_docs, index.docs);
    ASSERT_EQ(expected_entries, index.entries);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
