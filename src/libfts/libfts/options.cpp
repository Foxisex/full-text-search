#include <libfts/options.hpp>

namespace options {
    void indexating(
        const prsr::config& config,
        const std::string& csv_path,
        const std::string& index_path) {
        csv2::Reader<
            csv2::delimiter<','>,
            csv2::quote_character<'"'>,
            csv2::first_row_is_header<true>,
            csv2::trim_policy::trim_whitespace>
            csv;
        std::vector<std::string> strings;

        csv.mmap(csv_path);
        for (const auto row : csv) {
            size_t ind = 0;
            for (const auto cell : row) {
                if (ind == 0 || ind == 1) {
                    std::string value;
                    cell.read_value(value);
                    strings.emplace_back(value);
                }
                ind++;
            }
        }

        indexer::IndexBuilder builder(config);

        for (size_t i = 0; i < strings.size(); i += 2) {
            builder.add_document(std::stoul(strings[i]), strings[i + 1]);
        }
        indexer::Index index = builder.index();
        indexer::TextIndexWriter writer;

        writer.write(index_path, index);
    }

    void searching(
        const prsr::config& config,
        const std::string& query,
        const std::string& index_path) {
        searcher::TextIndexAccessor accessor(config, index_path);
        auto result = searcher::search(query, accessor);
        size_t count = 0;
        for (const auto& resElement : result) {
            std::cout << count << "\t" << resElement.score << "\t"
                      << resElement.doc_id << "\t"
                      << accessor.load_document(
                             std::to_string(resElement.doc_id))
                      << std::endl;
            count++;
        }
    }

    void searching_interactive(
        const prsr::config& config,
        const std::string& index_path) {
        std::string query;
        searcher::TextIndexAccessor accessor(config, index_path);
        std::getline(std::cin, query);
        auto result = searcher::search(query, accessor);
        size_t count = 0;
        for (const auto& resElement : result) {
            std::cout << count << "\t" << resElement.score << "\t"
                      << resElement.doc_id << "\t"
                      << accessor.load_document(
                             std::to_string(resElement.doc_id))
                      << std::endl;
            count++;
        }
    }
}  // namespace options