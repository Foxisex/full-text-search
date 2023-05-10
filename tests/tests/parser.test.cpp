#include <gtest/gtest.h>
#include <libfts/parser.hpp>

TEST(TestPARSER, myIspunct1) {
    ASSERT_TRUE(prsr::my_ispunct('+'));
}

TEST(TestPARSER, myIspunct2) {
    ASSERT_FALSE(prsr::my_ispunct('b'));
}

TEST(TestPARSER, str2words) {
    const std::vector<std::string> expected_result = {
        "byli", "u", "babusi", "dva", "veselyh", "gusya"};
    std::string original_str = "byli u babusi dva veselyh gusya";
    std::vector<std::string> out;
    prsr::str_to_words(original_str, ' ', out);
    ASSERT_EQ(out, expected_result);
}

TEST(TestPARSER, rmStopWords) {
    const std::vector<std::string> stop_words = {
        "a",   "an",    "and",  "are",   "as",    "at",   "be",   "but",
        "by",  "for",   "if",   "in",    "into",  "is",   "it",   "no",
        "not", "of",    "on",   "or",    "s",     "such", "t",    "that",
        "the", "their", "then", "there", "these", "they", "this", "to",
        "was", "will",  "with", "dr",    "mr"};
    std::vector<std::string> test_words = {
        "a",     "an",   "Alexey,",  "and",   "Kirill", "are",  "as",
        "at",    "be",   "but",      "by",    "for",    "if",   "in",
        "into",  "is",   "Valentin", "it",    "no",     "not",  "of",
        "on",    "or",   "s",        "such",  "t",      "that", "the",
        "their", "then", "there",    "these", "they",   "this", "to",
        "was",   "will", "with",     "dr",    "mr"};
    const std::vector<std::string> expected_result = {
        "Alexey,", "Kirill", "Valentin"};

    prsr::rm_stop_words(test_words, stop_words);
    ASSERT_EQ(test_words, expected_result);
}

TEST(TestPARSER, generateNgrams) {
    std::vector<prsr::ngrams> expected_result = {
        {"jek", 0},
        {"jeky", 0},
        {"jekyl", 0},
        {"jekyll", 0},
        {"hyd", 1},
        {"hyde", 1}};
    std::vector<std::string> words = {"jekyll", "hyde"};
    std::vector<prsr::ngrams> out;
    prsr::config cfg;
    cfg.min_ngram_length = 3;
    cfg.max_ngram_length = 6;
    prsr::generate_ngrams(words, cfg, out);
    ASSERT_EQ(out.size(), expected_result.size());
    for (size_t i = 0; i < out.size(); i++) {
        ASSERT_EQ(out.at(i).word, expected_result.at(i).word);
        ASSERT_EQ(out.at(i).token_index, expected_result.at(i).token_index);
    }
}

TEST(TestPARSER, parser) {
    std::vector<prsr::ngrams> expected_result = {
        {"jek", 0},
        {"jeky", 0},
        {"jekyl", 0},
        {"jekyll", 0},
        {"hyd", 1},
        {"hyde", 1}};
    std::string original_str = "Dr. Jekyll and Mr. Hyde";
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
    prsr::parser(original_str, config, out);
    ASSERT_EQ(out.size(), expected_result.size());
    for (size_t i = 0; i < out.size(); i++) {
        ASSERT_EQ(out.at(i).word, expected_result.at(i).word);
        ASSERT_EQ(out.at(i).token_index, expected_result.at(i).token_index);
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}