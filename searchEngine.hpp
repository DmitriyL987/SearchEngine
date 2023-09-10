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
    explicit SearchServer(invertedIndex &ind): _index(ind){};

    std::vector<std::vector<RelativeIndex>> search (const std::vector<std::string>& queries_input);

private:
    invertedIndex _index;

    std::vector<std::vector<std::string>> orderedWordsList(std::vector<std::vector<std::string>>& requestList);

    //std::pair<std::string, std::vector<Entry>> findRarestWord(std::vector<std::string>& _list);

    std::vector<RelativeIndex> getRank(std::vector<std::string> &_list);

    void sortRelativeVector(std::vector<RelativeIndex> &vec);

    std::vector<std::string> findRarestWord_2(std::vector<std::string>& _list);

};

//----
std::vector<std::vector<RelativeIndex>> SearchServer::search (const std::vector<std::string>& queries_input)
{
    std::vector<std::string> tmp ;
    tmp.clear();
    // разбили запросы по отдельным словам
    std::cout << "begin separation...\n";
    std::vector<std::vector<std::string>> fragmentedRequest;

    for (size_t i = 0; i < queries_input.size(); ++i){
        std::string  str = queries_input[i];
        _index.separationWord(str, tmp);
        fragmentedRequest.emplace_back(tmp);
    }
    std::cout << "fragmentedRequest - OK\n" << fragmentedRequest.size() << "\n";

    //формирует списки уникальных слов uniqueWordsList из fragmentedRequest
    std::vector<std::vector<std::string>> uniqueWordsList(fragmentedRequest.size());

    for(size_t i = 0; i < fragmentedRequest.size(); ++i)
    {
        //std::cout << "uniqueWordsList.size() = " << uniqueWordsList.size() << "\n";
        //uniqueWordsList[i].resize(fragmentedRequest[i].size());

        for(size_t j = 0; j < fragmentedRequest[i].size(); ++j) {
            auto it = std::find(uniqueWordsList[i].begin(), uniqueWordsList[i].end(),
                                fragmentedRequest[i][j]);

            if(it == uniqueWordsList[i].end()) {
                std::cout << "In cycle  " << i << "  " << j << "  " << fragmentedRequest[i][j] << std::endl;

                uniqueWordsList[i].emplace_back(fragmentedRequest[i][j]);
            }
        }
    }
    std::cout << "разбили запросы по отдельным словам, формирует списки уникальных слов uniqueWordsList из queries_input\n";
    std::cout << "uniqueWordsList.size() = " << uniqueWordsList.size() << "\n";
   // std::vector<std::pair<std::string, std::vector<Entry>>> rarestWordList;


    //упорядоченный список слов по частоте встречаемости в словаре
    std::vector<std::vector<std::string>> orderedWordList(uniqueWordsList.size());

    for(size_t i = 0; i < uniqueWordsList.size(); ++i)
        orderedWordList.emplace_back(findRarestWord_2(uniqueWordsList[i]));
    std::cout << "упорядоченный список слов по частоте встречаемости в словаре\n";

    std::vector<std::vector<RelativeIndex>> relativeIndex;
    for(size_t i; i < orderedWordList.size(); ++i)
        relativeIndex.emplace_back( getRank(orderedWordList[i]));
    std::cout << "возвращаем вектор RelativIndex\n";
    return relativeIndex;
}
//---------------------------------
//Упорядочивание списка слов
std::vector<std::vector<std::string>> SearchServer::orderedWordsList(std::vector<std::vector<std::string>>& requestList){
    size_t min = 0;
    size_t ind = 0;
    std::vector<bool> have_in_docs;
    std::vector<std::vector<std::string>> result;

    for (size_t i = 0; i < requestList.size(); ++i)
    {
        for(size_t j = 0; j < requestList[i].size(); ++j)
        {
            std::vector<Entry> vectorWord = _index.getDictionary().find(requestList[i][j])->second;
            std::map<size_t, size_t> orderWord;
            if(vectorWord.empty()) {
                have_in_docs[i] =  false;
                continue;
            } else {
                have_in_docs[i] = true;
                for(size_t k = 0; k < vectorWord.size(); ++k){
                    if(vectorWord[k].count && orderWord[k])
                        orderWord.emplace(std::make_pair(vectorWord[k].count, vectorWord[k].doc_id));// не годится
                }
            }

            if(!orderWord.empty()){
                for(auto it : orderWord){

                }
            }
        }
        if(!have_in_docs[i]){}
    }
    return result;
}

//------------------------------------------
//возвращает вектор слов отсортированный по частоте встречаемости в порядке возрастания в словаре
std::vector<std::string>  SearchServer::findRarestWord_2(std::vector<std::string>& _list){
    std::map <size_t, std::string> map;

    for(size_t i = 0; i < _list.size(); ++i)
    {
        auto it = _index.getDictionary().find(_list[i]);
        if(it != _index.getDictionary().end())
        {
            size_t sumFieldCount = 0;
            for (auto el : it->second) { sumFieldCount += el.count; } // сколько раз слово встречается во всех текстах
            //заполняем мапу
            if(map.find(sumFieldCount) == map.end()) map.emplace(std::make_pair(sumFieldCount, it->first)) ;
            else {
                while(map.find(sumFieldCount) != map.end())
                {
                    ++sumFieldCount;
                }
                map.emplace(std::make_pair(sumFieldCount, it->first));
            }
        }
    }
    std::vector<std::string> orderWord(map.size());
    for(const auto& iter : map){
        orderWord.emplace_back(iter.second) ;
    }
    return orderWord;
}


//--------------------------------------------!!!
//std::vector<RelativeIndex> SearchServer::getRank(std::vector<std::string>& _list, std::vector<size_t>& docList){
//    if(docList.empty()) return  std::vector<RelativeIndex>();
//    std::vector <std::vector<size_t>> rankTableAbs(_list.size());
//    for(size_t i = 0; i < docList.size(); ++i){
//        for(size_t j = 0; j < _list.size(); ++j){
//            auto it = _index.getDictionary().find(_list[i]);
//            for( size_t k = 0; k < docList.size(); ++k) {
//                if(docList[i] == it->second[k].doc_id) {
//                    rankTableAbs[i][k] = it->second[j].count;
//                }
//            }
//        }
//    }
//    std::vector <size_t> rankAbs;
//    size_t maxRank = 0;
//    for(size_t i = 0; i < rankTableAbs.size(); ++i){
//        for(size_t j = 0; j < rankTableAbs[i].size(); ++j){
//            rankAbs[i] += rankTableAbs[i][j];
//            if(maxRank < rankAbs[i]) maxRank = rankAbs[i];
//        }
//    }
//    std::vector<RelativeIndex> relativeIndex;
//    for(int i = 0; i < rankAbs.size(); ++i){
//        relativeIndex[i].rank = rankAbs[i]/maxRank;
//        relativeIndex[i].doc_id = docList[i];
//    }
//    sortRelativeVector(relativeIndex);
//    return relativeIndex;
//}

//-------------------------------------------
std::vector<RelativeIndex> SearchServer::getRank(std::vector<std::string> &_list){

    if(_list.empty()) {
        std::cout<< "getRank: список пуст!\n";
        return std::vector<RelativeIndex>();
    }

    std::vector<RelativeIndex> relativeIndex;

    std::vector<size_t> docList(_list.size());
    std::vector<size_t> rankTableAbs;

    auto it = _index.getDictionary().find(_list[0]);
    if(it != _index.getDictionary().end()) {
        for (size_t k = 0; k < it->second.size(); ++k) {
            docList.emplace_back(it->second[k].doc_id);
            rankTableAbs.emplace_back(it->second[k].count);
        }
    }

    std::vector <float> rank;
    size_t maxRank = 0;
    for(size_t i = 0; i < rankTableAbs.size(); ++i)
                if(maxRank < rankTableAbs[i]) maxRank = rankTableAbs[i];

    for(int i = 0; i < rank.size(); ++i)
    {
        relativeIndex[i].rank = rank[i]/(float)maxRank;
        relativeIndex[i].doc_id = docList[i];
    }
    sortRelativeVector(relativeIndex);
    if(relativeIndex.empty()) std::cout << "getRank: relativeIndex пуст\n";
    return relativeIndex;
}

//Сортировка по релевантности от большей к меньшей
void SearchServer::sortRelativeVector(std::vector<RelativeIndex> &vec){
    RelativeIndex rlvInd{0,0.0};
    for(int i= 0; i < vec.size(); ++i) {
        for (int j = i; j < vec.size(); ++j) {
            if (vec[j].rank > vec[i].rank) {
                rlvInd.rank = vec[j].rank;
                rlvInd.doc_id = vec[j].doc_id;
                vec[j].rank = vec[i].rank;
                vec[j].doc_id = vec[i].doc_id;
                vec[i].rank = rlvInd.rank;
                vec[i].doc_id = rlvInd.doc_id;
            }
        }
    }
}


