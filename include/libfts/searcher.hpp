#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

#include <libfts/parser.hpp>

namespace searcher {

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

        void get_term_infos(const std::string& term);
        std::string load_document(const std::string& doc_id);
    };
}  // namespace searcher