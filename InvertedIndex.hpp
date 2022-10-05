//
// Created by LDm on 23.08.2022.
//

#pragma once

#include <sstream>
#include <map>
#include <thread>
#include <mutex>
#include "converterJSON.h"

struct  Entry{
    size_t doc_id, count;
    bool operator ==(const Entry &other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};
//-----------------------------------------------------

class InvertedIndex {
public:
    InvertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs);

    std::vector<Entry> GetWordCount(const std::string& word);

    static std::vector<std::string> separationWord(std::string& text);

private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};

//-----------------------------------------------------

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    if (docs.empty()){
        docs = input_docs;
    }

    if (freq_dictionary.empty()){

        std::vector<Entry> entryList;

        for(int i = 0; i < docs.size(); ++i){

            freq_dictionary.emplace(std::make_pair("_T1 &&__x", entryList));
        }
    } else {

    }
}

//-----------------------------------------------------

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    std::vector<Entry> entry;

    int countWord = 0;
    for (int i = 0; i < docs.size(); ++i){
        int count = 0;
        int t = -1;

        for(int j = 0; j < docs[i].size(); ++j){
            t = docs[i].find(word,j);
            if(t >= 0) {
                j = j + word.size();
                ++count;
            }
        }
        entry[countWord].doc_id = i;
        entry[countWord].count = count;
    }

    return entry;
}

//-----------------------------------------------------

std::vector<std::string> InvertedIndex::separationWord(std::string& text) {
    std::istringstream stream(text);
    std::vector<std::string> words;
    while(!stream.eof()){
        std::string str;
        stream >> str;// знаки и скобки часть слов
        words.push_back(str);
    }
    //std::cout << "words.size(): " <<words.size() << std::endl;
    return words;
}