#include <libfts/options.hpp>
#include <replxx.hxx>

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
        std::string prompt = "\x1b[1;32mquery\x1b[0m> ";
        std::string input;

        replxx::Replxx rxx;

        rxx.install_window_change_handler();

        rxx.set_word_break_characters(" \n\t.,-%!;:=*~^'\"/?<>|[](){}");
        rxx.set_no_color(false);
        rxx.bind_key_internal(
            replxx::Replxx::KEY::BACKSPACE, "delete_character_left_of_cursor");
        rxx.bind_key_internal(
            replxx::Replxx::KEY::DELETE, "delete_character_under_cursor");
        rxx.bind_key_internal(replxx::Replxx::KEY::LEFT, "move_cursor_left");
        rxx.bind_key_internal(replxx::Replxx::KEY::RIGHT, "move_cursor_right");
        rxx.bind_key_internal(
            replxx::Replxx::KEY::HOME, "move_cursor_to_begining_of_line");
        rxx.bind_key_internal(
            replxx::Replxx::KEY::END, "move_cursor_to_end_of_line");
        rxx.bind_key_internal(
            replxx::Replxx::KEY::control(replxx::Replxx::KEY::LEFT),
            "move_cursor_one_word_left");
        rxx.bind_key_internal(
            replxx::Replxx::KEY::control(replxx::Replxx::KEY::RIGHT),
            "move_cursor_one_word_right");
        rxx.bind_key_internal(
            replxx::Replxx::KEY::control(replxx::Replxx::KEY::ENTER),
            "commit_line");
        rxx.bind_key_internal(
            replxx::Replxx::KEY::meta(replxx::Replxx::KEY::BACKSPACE),
            "kill_to_whitespace_on_left");
        rxx.bind_key_internal(
            replxx::Replxx::KEY::INSERT, "toggle_overwrite_mode");

        searcher::TextIndexAccessor accessor(config, index_path);

        while (true) {
            char const* cinput{nullptr};

            do {
                cinput = rxx.input(prompt);
            } while ((cinput == nullptr) && (errno == EAGAIN));

            if (cinput == nullptr) {
                break;
            }
            input = cinput;
            if (!input.empty()) {
                //Завершение
                if (input == " ") {
                    break;
                }

                auto result = searcher::search(input, accessor);
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
        }
    }
}  // namespace options