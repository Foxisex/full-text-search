#include <libfts/parser.hpp>

namespace prsr {

    bool my_ispunct(char symbol) {
        return static_cast<bool>(
            std::ispunct(static_cast<unsigned char>(symbol)));
    }

    void str_to_words(
        std::string& origin,
        char delim,
        std::vector<std::string>& result_vec) {
        size_t begin = 0;
        size_t end = 0;

        while ((begin = origin.find_first_not_of(delim, end)) !=
               std::string::npos) {
            end = origin.find(delim, begin);
            result_vec.push_back(origin.substr(begin, end - begin));
        }
    }

    void rm_stop_words(
        std::vector<std::string>& result_vec,
        const std::vector<std::string>& stop_words) {
        for (const auto& vElement : stop_words) {
            auto index =
                std::find(result_vec.begin(), result_vec.end(), vElement);
            if (index != result_vec.end()) {
                result_vec.erase(index);
            }
        }
    }

    void generate_ngrams(
        const std::vector<std::string>& words,
        const prsr::config& cfg,
        std::vector<prsr::ngrams>& parsed_vec) {
        int index = 0;
        for (const auto& word : words) {
            if (word.length() >= static_cast<unsigned>(cfg.min_ngram_length)) {
                for (auto i = cfg.min_ngram_length; i <= cfg.max_ngram_length;
                     i++) {
                    if (static_cast<int>(word.length()) >= i) {
                        parsed_vec.push_back({word.substr(0, i), index});
                    }
                }
            }
            index++;
        }
    }

    void parser(
        const std::string& origin,
        const prsr::config& cfg,
        std::vector<prsr::ngrams>& parsed_vec) {
        std::string origin_copy(origin);

        static_cast<void>(
            std::remove_if(origin_copy.begin(), origin_copy.end(), my_ispunct));

        std::transform(
            origin_copy.begin(),
            origin_copy.end(),
            origin_copy.begin(),
            tolower);

        std::vector<std::string> words;
        str_to_words(origin_copy, ' ', words);

        rm_stop_words(words, cfg.stop_words);

        generate_ngrams(words, cfg, parsed_vec);
    }

}  // namespace prsr