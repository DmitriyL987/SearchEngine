//
// Created by LDm on 23.08.2022.
//

#ifndef SEARCH_ENGINE_INVERTEDINDEX_HPP
#define SEARCH_ENGINE_INVERTEDINDEX_HPP

struct  Entry}{
    size_t doc_id, count;
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs);

    std::vector<Entry> GetWordCount(const std::string& word);
private:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный
    словарь
};

void InvertedIndex::UpdateDocumentBase(int input_docs) {

}

std::vector<Entry> InvertedIndex::GetWordCount(const int &word) {


    return Entry();
}
#endif //SEARCH_ENGINE_INVERTEDINDEX_HPP
