#include <libfts/indexer.hpp>

namespace indexer {

    void IndexBuilder::add_document(
        const size_t document_id,
        const std::string& text) {
        ind.docs.emplace(document_id, text);

        std::vector<prsr::ngrams> vNgrams;

        prsr::parser(text, cfg, vNgrams);

        for (const auto& vElement : vNgrams) {
            const auto entrieRes = ind.entries.find(vElement.word);
            if (entrieRes == ind.entries.end()) {
                ind.entries.emplace(
                    vElement.word,
                    std::unordered_map<size_t, std::vector<int>>{
                        {document_id, std::vector<int>{vElement.token_index}}});
            } else {
                const auto& docRes = entrieRes->second.find(document_id);
                if (docRes == entrieRes->second.end()) {
                    entrieRes->second.emplace(
                        document_id, std::vector<int>{vElement.token_index});
                } else {
                    docRes->second.emplace_back(vElement.token_index);
                }
            }
        }
    }

    Index IndexBuilder::index() {
        return ind;
    }

    void TextIndexWriter::write(const std::string& path, const Index& index) {
        std::ofstream file;

        std::filesystem::create_directories("index/docs");
        std::filesystem::create_directories("index/entries");

        for (const auto& doc : index.docs) {
            file.open(
                path + "/index/docs/" + std::to_string(doc.first) + ".txt");
            file << doc.second;
            file.close();
        }

        for (const auto& entrie : index.entries) {
            std::string hash_ngram;
            picosha2::hash256_hex_string(entrie.first, hash_ngram);
            file.open(
                path + "/index/entries/" +
                std::string(hash_ngram.begin(), hash_ngram.begin() + 6) +
                ".txt");
            file << entrie.first << " " << entrie.second.size() << " ";
            for (const auto& doc : entrie.second) {
                file << std::to_string(doc.first) << " "
                     << std::to_string(doc.second.size()) << " ";
                for (const auto& pos_cnt : doc.second) {
                    file << std::to_string(pos_cnt) << " ";
                }
            }
            file << "\n";
            file.close();
        }
    }
}  // namespace indexer