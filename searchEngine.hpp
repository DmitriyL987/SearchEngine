#pragma once

#include "converterJSON.h"
#include "invertedIndex.hpp"

struct RelativeIndex{
    size_t doc_id;
    float rank;
    bool operator == (const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};



class SearchServer
{
public:
    SearchServer(invertedIndex &ind): _index(ind){};

    std::vector<std::vector<RelativeIndex>> search (const std::vector<std::string>& queries_input);

private:
    invertedIndex _index;

    std::vector<std::vector<std::string>> orderedWordsList(std::vector<std::vector<std::string>>& _list);

    std::pair<std::string, std::vector<Entry>> findRarestWord(std::vector<std::string>& _list);

    std::vector<RelativeIndex> getRank(std::vector<std::string> &_list, std::vector<size_t> &docList);

    void sortRelativeVector(std::vector<RelativeIndex> &vec);
};


//------------------------------

std::vector<std::vector<RelativeIndex>> SearchServer::search
                             (const std::vector<std::string>& queries_input)
{
    //**********************************************************
    std::vector<std::vector<std::string>> fragmentedRequest;
    for (auto it : queries_input){
        fragmentedRequest.emplace_back(_index.separationWord(it));
    } // разбили по отдельным словам
    //********************************************************

    std::vector<std::vector<std::string>> uniqueWordsList;

    for(size_t c = 0; c < fragmentedRequest.size(); ++c){
        for(size_t i = 0; i < fragmentedRequest[c].size(); ++i){

            auto check_match = [&uniqueWordsList, &fragmentedRequest](size_t m, size_t n){
                bool have_match = false;
                for (size_t j = 0; j < uniqueWordsList.size(); ++j)
                    for(size_t a = 0; a < uniqueWordsList[j].size(); ++a)
                        if(fragmentedRequest[m][n] == uniqueWordsList[j][a] ) have_match = true;
                return have_match;
            };
            if(!check_match(c,i))
                uniqueWordsList[c].emplace_back(fragmentedRequest[c][i]); //

        }
    } // сформировали списки уникальных слов uniqueWordsList

    //*****************************
    std::vector<size_t> docList;
    std::vector <Entry> entry;
    std::vector<std::vector<RelativeIndex>> relativeIndex;

    std::map<size_t, std::vector<size_t>> orderedList;
    size_t accumulate = 0;

    //*****************************************************

    for(size_t i = 0; i < uniqueWordsList.size(); ++i){         // цикл по запросам
        // ищем самое редкое слово в запросе
        for(size_t j = 0; j < uniqueWordsList[i].size(); ++j){// цикл по словам
            std::string word = uniqueWordsList[i][j];
            auto iter_dict = _index.getDictionary().find(word);
            if(iter_dict != _index.getDictionary().end()){

                for(size_t k = 0; k < iter_dict->second.size(); ++k){
                    orderedList.emplace( iter_dict->second[k].count, iter_dict->second[k].doc_id);
                }
            }
        }
        // ищем совпадение последующих слов из данного запроса в документах определённых для 1-го слова

        // считаем абсолютную релевантность
    }
    return relativeIndex;
}
//---------------------------------???

std::vector<std::vector<std::string>> SearchServer::orderedWordsList(std::vector<std::vector<std::string>>& _list){
    size_t min = 0;
    size_t ind = 0;
    std::vector<bool> have_in_docs;
    std::vector<std::vector<std::string>> result;
    for (size_t i = 0; i < _list.size(); ++i){
        for(size_t j = 0; j< _list[i].size(); ++j){
            std::vector<Entry> vectorWord = _index.getDictionary().find(_list[i][j])->second;
            std::map<size_t, size_t> orderWord;
            if(vectorWord.empty()) {
                have_in_docs[i] =  false;
                continue;
            } else {
                have_in_docs[i] = true;
                for(size_t k = 0; k < vectorWord.size(); ++k){
                    if(vectorWord[k].count)
                        orderWord.emplace(std::make_pair(vectorWord[k].count, vectorWord[k].doc_id));
                }
            }
        }
        if(!have_in_docs[i]){}
    }
    return result;
}

//------------------------------------------!??
std::pair<std::string, std::vector<Entry>>  SearchServer::findRarestWord(std::vector<std::string>& _list){
    size_t  min = _index.getCountDocs(),
            rarestWordIndex = 0;
    //std::vector<std::string> orderedWord;
    std::pair<std::string, std::vector<Entry>> wordInDocs;
    for(size_t i = 0; i < _list.size(); ++i){
        size_t count = 0;
        auto it = _index.getDictionary().find(_list[i]);
        if(it != _index.getDictionary().end()) {
            ++count;
            if(it->second.size() < min) {
                min = it->second.size();
                rarestWordIndex = i;
            }
        }
    } // нашли самое редкое слово -> rarestWord
    auto it = _index.getDictionary().find(_list[rarestWordIndex]);
    wordInDocs.first = it->first;
    for (size_t i = 0; i < it->second.size(); ++i){
        wordInDocs.second[i].count = it->second[i].count;
        wordInDocs.second[i].doc_id = it->second[i].doc_id;
    }
    return wordInDocs;
}
//--------------------------------------------!!!

std::vector<RelativeIndex> SearchServer::getRank(std::vector<std::string>& _list, std::vector<size_t>& docList){
    if(docList.empty()) return  std::vector<RelativeIndex>();
    std::vector <std::vector<size_t>> rankTableAbs(_list.size());
    for(size_t i = 0; i < docList.size(); ++i){
        for(size_t j = 0; j < _list.size(); ++j){
            auto it = _index.getDictionary().find(_list[i]);
            for( size_t k = 0; k < docList.size(); ++k) {
                if(docList[i] == it->second[k].doc_id) {
                    rankTableAbs[i][k] = it->second[j].count;
                }
            }
        }
    }
    std::vector <size_t> rankAbs;
    size_t maxRank = 0;
    for(size_t i = 0; i < rankTableAbs.size(); ++i){
        for(size_t j = 0; j < rankTableAbs[i].size(); ++j){
            rankAbs[i] += rankTableAbs[i][j];
            if(maxRank < rankAbs[i]) maxRank = rankAbs[i];
        }
    }
    std::vector<RelativeIndex> relativeIndex;
    for(int i = 0; i < rankAbs.size(); ++i){
        relativeIndex[i].rank = rankAbs[i]/maxRank;
        relativeIndex[i].doc_id = docList[i];
    }
    sortRelativeVector(relativeIndex);
    return relativeIndex;
}
//--------------------------------------!!!
void SearchServer::sortRelativeVector(std::vector<RelativeIndex> &vec){
    for(int i= 0; i < vec.size(); ++i) {
        for (int j = 1; j < vec.size(); ++j) {
            if (vec[j].rank > vec[i].rank) {
                float sw = vec[j].rank;
                vec[j].rank = vec[i].rank;
                vec[i].rank = sw;
            }
        }
    }
}
