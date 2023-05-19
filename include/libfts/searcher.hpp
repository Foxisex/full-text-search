#pragma once

#include <filesystem>
#include <iostream>
#include <map>

#include <libfts/indexer.hpp>
#include <libfts/parser.hpp>

namespace searcher {

    struct TermInfos {
        std::string term;
        std::map<size_t, size_t> doc_info;
    };

    class TextIndexAccessor {
       private:
        std::string path;
        prsr::config cfg;

       public:
        TextIndexAccessor(prsr::config config) {
            path = std::filesystem::current_path();
            path += "/index";
            cfg = config;
        }

        searcher::TermInfos get_term_infos(const std::string& term);
        std::string load_document(const std::string& doc_id);
    };
}  // namespace searcher