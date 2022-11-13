//
// Created by LDm on 23.08.2022.
//
#pragma once

#include <sstream>
#include <map>
#include <future>
#include <memory>
#include <thread>
#include <regex>
#include "converterJSON.h"

struct  Entry{
    size_t doc_id, count;
    bool operator ==(const Entry &other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};
//-----------------------------------------------------

class invertedIndex {
public:
    invertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs);

    std::vector<Entry> GetWordCount(const std::string& word);

    std::vector<Entry> get_count_source_word_in_dictionary(std::string &); // для теста

    std::vector<std::string> separationWord(std::string& text);

    std::map<std::string, std::vector<Entry>> getDictionary();

    size_t getCountDocs();
private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};

//-----------------------------------------------------

void invertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {

    docs = input_docs;
    freq_dictionary.clear();
    std::vector<std::vector<Entry>> entryList(docs.size());
    std::vector <std::vector<std::string>> fragmentedText;
    std::shared_future<void>future;
    for(int i = 0; i < docs.size(); ++i) fragmentedText.emplace_back(separationWord(docs[i]));
    for(int c = 0; c < docs.size(); ++c){
        future = std::async(std::launch::async, [this, &fragmentedText](int i)
        {
            std::cout << i << " thread ID :" << std::this_thread::get_id() << std::endl;
            for(const auto& word : fragmentedText[i])
            {
                //std::cout << "Async invoke! thread_id: " << std::this_thread::get_id() << std::endl;
                auto it = freq_dictionary.find(word);
                if(it != freq_dictionary.end()){
                    continue;
                } else {
                    freq_dictionary.emplace(std::make_pair(word, GetWordCount(word)));
                }
            }
        }, c);
    }
    future.wait();
    std::cout << "Update base - ok!\n";
}

//-----------------------------------------------------

std::vector<Entry> invertedIndex::GetWordCount(const std::string& word) {
    std::vector<Entry> entryList;
    if(!docs.empty()) {
        Entry entry;

        for (size_t i = 0; i < docs.size(); ++i) {
            size_t count = 0;
            int foundPos = -1;

            for (size_t j = 0; j < docs[i].size(); ++j) {
                foundPos = -1;
                foundPos = docs[i].find(word, j);
                if (foundPos >= 0) {
                    j = foundPos + word.size();
                    ++count;
                }
            }
            entry.doc_id =  i;
            entry.count = count;
            entryList.emplace_back(entry);
            std::cout << "in docs #" << i << " founded " << count << " words:    " << word << std::endl;
        }
    }
        return entryList;
}

std::vector<std::string> invertedIndex::separationWord(std::string &text) {
    std::vector<std::string> words;
    std::regex wordRegex("(\\w+)");
    auto words_begin = std::sregex_iterator (text.begin(),text.end(),wordRegex);
    auto words_end = std::sregex_iterator ();
    int N = 2; // выборка слов содержащих более 2-х символов
    for(std::sregex_iterator i = words_begin; i != words_end; ++i){
        std::smatch match = *i;
        std::string match_str = match.str();
        if(match_str.size() > N) words.push_back(match_str);
    }
    return words;
}

//-----------------------------------------------------

std::vector<Entry> invertedIndex::get_count_source_word_in_dictionary(std::string &word) {
    return freq_dictionary[word];
}
//-------------------------------------

std::map<std::string, std::vector<Entry>> invertedIndex::getDictionary(){
    return freq_dictionary;
}
//-------------------------------------

size_t invertedIndex::getCountDocs() {
    if(docs.empty()) return 0;
    else return docs.size();
}