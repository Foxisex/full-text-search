#include <libfts/searcher.hpp>

namespace searcher {
    std::string TextIndexAccessor::load_document(const std::string& doc_id) {
        std::ifstream file;
        std::string result;
        file.open((path + "/docs/" + doc_id + ".txt"));
        if (!file.is_open()) {
            std::cout << "File is not found!" << std::endl;
        } else {
            std::getline(file, result);
        }
        file.close();
        return result;
    }
}  // namespace searcher