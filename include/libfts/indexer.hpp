#pragma once
#include <libfts/picosha2.h>
#include <filesystem>
#include <fstream>
#include <libfts/parser.hpp>
#include <unordered_map>

namespace indexer {

    class Index {
       public:
        std::unordered_map<size_t, std::string> docs;
        std::unordered_map<
            std::string,
            std::unordered_map<size_t, std::vector<int>>>
            entries;
    };

    class IndexBuilder {
       private:
        Index ind;
        prsr::config cfg;

       public:
        explicit IndexBuilder(prsr::config& config) : cfg(config) {
        }

        void add_document(const size_t document_id, const std::string& text);

        Index index();
    };

    class TextIndexWriter {
       public:
        void write(const std::string& path, const Index& index);
    };
}  // namespace indexer