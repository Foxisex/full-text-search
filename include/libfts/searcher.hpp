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

        Result(size_t docId, double Score) : doc_id(docId), score(Score){};
    };

    class TextIndexAccessor {
        friend std::vector<Result> search(
            const std::string&,
            TextIndexAccessor&);

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
            doc_count = stod(load_document("_DocCount_"));
        }

        TextIndexAccessor(prsr::config config, std::string idx_path) {
            path = idx_path + "/index";
            cfg = config;
            doc_count = stod(load_document("_DocCount_"));
        }
    };

    std::vector<Result> search(
        const std::string& query,
        TextIndexAccessor& accessor);

}  // namespace searcher
