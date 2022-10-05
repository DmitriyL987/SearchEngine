#include <thread>
#include <mutex>
#include <gtest/gtest.h>

#include "converterJSON.h"
#include "InvertedIndex.hpp"
#include "searchEngine.hpp"



int main() {

    ConverterJSON Converter;
    //ConverterJSON::GetTextDocuments();
    InvertedIndex inverter;
    std::vector<std::string> texts;
    texts = Converter.GetTextDocuments();
    inverter.UpdateDocumentBase(texts);
    std::vector<std::string> word;
    word = InvertedIndex::separationWord(texts[0]);
    for(int i = 0; i < word.size(); ++i){
        std::cout  << word[i] << " " ;
    }
    return 0;
}
