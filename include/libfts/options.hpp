#pragma once
#include <csv2/reader.hpp>
#include <libfts/indexer.hpp>
#include <libfts/parser.hpp>
#include <libfts/searcher.hpp>

namespace options {
    void indexating(
        const prsr::config& config,
        const std::string& csv_path,
        const std::string& index_path);
    void searching(
        const prsr::config& config,
        const std::string& query,
        const std::string& index_path);
    void searching_interactive(
        const prsr::config& config,
        const std::string& index_path);

}  // namespace options