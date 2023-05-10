#include <libfts/parser.hpp>

namespace prsr {

    bool my_ispunct(char symbol) {
        return static_cast<bool>(
            std::ispunct(static_cast<unsigned char>(symbol)));
    }
    // vec.emplace_back(str.begin, str.end)

    void str_to_words(
        std::string& origin,
        char delim,
        std::vector<std::string>& result_vec) {
        size_t begin = 0;
        size_t end = 0;

        while ((begin = origin.find_first_not_of(delim, end)) !=
               std::string::npos) {
            end = origin.find(delim, begin);
            result_vec.emplace_back(origin.substr(begin, end - begin));
        }
    }

    void rm_stop_words(
        std::vector<std::string>& result_vec,
        const std::vector<std::string>& stop_words) {
        std::vector<std::string> tmp_vec;

        for (const auto& vElement : result_vec) {
            auto index =
                std::find(stop_words.begin(), stop_words.end(), vElement);
            if (index == stop_words.end()) {  // annorted ?? vector
                tmp_vec.emplace_back(vElement);
            }
        }
        result_vec = tmp_vec;
    }

    void generate_ngrams(
        const std::vector<std::string>& words,
        const prsr::config& cfg,
        std::vector<prsr::ngrams>& parsed_vec) {
        int index = 0;
        for (const auto& word : words) {
            if (word.length() >= static_cast<unsigned>(cfg.min_ngram_length)) {
                for (auto i = cfg.min_ngram_length; i <=
                     std::min(cfg.max_ngram_length,
                              static_cast<int>(word.length()));
                     i++) {
                    parsed_vec.push_back({word.substr(0, i), index});
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

        origin_copy.erase(
            std::remove_if(origin_copy.begin(), origin_copy.end(), my_ispunct),
            origin_copy.end());

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