#pragma once

#include <cmath>
#include <filesystem>
#include <iostream>
#include <libfts/indexer.hpp>
#include <libfts/parser.hpp>
#include <map>
#include <set>

namespace searcher {

    struct TermInfos {
        std::string term;
        std::map<size_t, size_t> doc_info;
    };

    struct Result {
        size_t doc_id;
        double score;

        Result(size_t doc_id_ = 0, double score_ = 0.0)
            : doc_id(doc_id_), score(score_){};

        bool operator<(const Result& rhs) const {
            if (score != rhs.score) {
                return score > rhs.score;
            } else {
                return doc_id < rhs.doc_id;
            }
        }

        bool operator==(const Result& rhs) const {
            return doc_id == rhs.doc_id;
        }
    };

    class TextIndexAccessor {
        friend std::set<Result> search(const std::string&, TextIndexAccessor&);

       private:
        std::string path;
        double doc_count;
        prsr::config cfg;

       public:
        searcher::TermInfos get_term_infos(const std::string& term);
        std::string load_document(const std::string& doc_id);

        TextIndexAccessor(prsr::config config) {
            path = std::filesystem::current_path();
            path += "/index";
            cfg = config;
            doc_count = stod(load_document("DocCount"));
        }
    };

    std::set<Result> search(
        const std::string& query,
        TextIndexAccessor& accessor);

}  // namespace searcher