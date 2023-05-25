#include <libfts/searcher.hpp>

namespace searcher {
    std::string TextIndexAccessor::load_document(const std::string& doc_id) {
        std::ifstream file;
        std::string result;
        file.open((path + "/docs/" + doc_id + ".txt"));
        if (!file.is_open()) {
            return result;
        }
        std::getline(file, result);
        file.close();
        return result;
    }

    searcher::TermInfos TextIndexAccessor::get_term_infos(
        const std::string& term) {
        std::ifstream file;
        std::string hash_term;
        TermInfos term_infos;
        picosha2::hash256_hex_string(term, hash_term);
        file.open(
            path + "/entries/" +
            std::string(hash_term.begin(), hash_term.begin() + 6) + ".txt");
        if (!file.is_open()) {
            return term_infos;
        }

        for (std::string read_line; std::getline(file, read_line);) {
            if (read_line.find(term) != std::string::npos) {
                std::array<size_t, 2> iter = {0, 0};

                iter[0] = read_line.find(' ', 0);
                term_infos.term = read_line.substr(0, iter[0]);

                iter[0]++;
                iter[1] = read_line.find(' ', iter[0]);
                size_t doc_count =
                    stoul(read_line.substr(iter[0], iter[1] - iter[0]));

                for (size_t i = 0; i < doc_count; i++) {
                    iter[1]++;
                    iter[0] = read_line.find(' ', iter[1]);
                    size_t doc_id =
                        stoul(read_line.substr(iter[1], iter[0] - iter[1]));

                    iter[0]++;
                    iter[1] = read_line.find(' ', iter[0]);
                    size_t pos_count =
                        stoul(read_line.substr(iter[0], iter[1] - iter[0]));

                    term_infos.doc_info.emplace(doc_id, pos_count);

                    for (size_t j = 0; j < pos_count; j++) {
                        iter[1]++;
                        iter[1] = read_line.find(' ', iter[1]);
                    }
                }
            }
        }

        file.close();
        return term_infos;
    }

    bool stability(const Result& first, const Result& second) {
        return first.doc_id < second.doc_id;
    }

    bool more_by_score(const Result& first, const Result& second) {
        return first.score > second.score;
    }

    std::vector<Result> search(
        const std::string& query,
        TextIndexAccessor& accessor) {
        std::vector<prsr::ngrams> query_ngrams;
        prsr::parser(query, accessor.cfg, query_ngrams);
        std::vector<TermInfos> term_infos;
        std::unordered_map<size_t, double> result;

        for (const auto& query_ngram : query_ngrams) {
            TermInfos term_info = accessor.get_term_infos(query_ngram.word);
            if (!term_info.doc_info.empty()) {
                term_infos.emplace_back(term_info);
            }
        }

        for (const auto& vElement : term_infos) {
            for (const auto& doc_info_element : vElement.doc_info) {
                double score = static_cast<double>(doc_info_element.second) *
                    std::log((accessor.doc_count + 1.0) /
                             static_cast<double>(vElement.doc_info.size()));

                auto res_found = result.find(doc_info_element.first);
                // std::cout<<doc_info_element.first<< " " <<score << " " <<
                // res_found->doc_id<<std::endl;
                if (res_found == result.end()) {
                    result.emplace(doc_info_element.first, score);
                } else {
                    res_found->second += score;
                    std::cout << doc_info_element.first << " + " << score
                              << std::endl;
                }
            }
        }
        std::vector<Result> vRes;
        for (const auto& mElement : result) {
            vRes.emplace_back(mElement.first, mElement.second);
        }

        sort(vRes.begin(), vRes.end(), stability);

        sort(vRes.begin(), vRes.end(), more_by_score);

        return vRes;
    }

}  // namespace searcher