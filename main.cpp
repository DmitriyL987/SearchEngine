//#include <gtest/gtest.h>
#include "converterJSON.h"
#include "invertedIndex.hpp"
#include <chrono>
#include <iostream>
#include "searchEngine.hpp"
//std::mutex mutex;
//void test(std::vector<std::vector<std::pair<int, float>>> preAnswer);


//TEST(sample_test_case, sample_test)
//{
//    EXPECT_EQ(1, 1);
//}




int main() {
    ConverterJSON converter;
    invertedIndex inverter;
    std::vector<std::string> texts;
    texts = converter.GetTextDocuments();
    inverter.UpdateDocumentBase(texts);
    std::vector<std::string> requests = converter.GetRequests();
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    SearchServer server (inverter);
    std::vector<std::vector<RelativeIndex>> tmp(server.search(requests));
    std::vector<std::vector<std::pair<int, float>>> preAnswer(tmp.size());

    for(int i = 0;i < tmp.size(); ++i){
        for( int j = 0; j < tmp[i].size(); ++j)
                preAnswer[i].emplace_back(std::make_pair(tmp[i][j].doc_id,tmp[i][j].rank));
    }
    //test( preAnswer);
    converter.putAnswers(preAnswer);
    return 0;
}

//--------------------------------------
//void checkConditionExit(bool& cond){
//    std::string str;
//    mutex.lock();
//    std::cin >> str;
//    mutex.unlock();
//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    if(str == "exit") {
//        mutex.lock();
//        cond = false;
//        mutex.unlock();
//    }
//}
//--------------------------------------
//void  test(std::vector<std::vector<std::pair<int, float>>> preAnswer){
//    std::cout << "\t\tTEST " << "\n";
//    for(int i = 0;i < preAnswer.size(); ++i){
//        std::cout << "Request " << i << "\n";
//        for( int j = 0; j < preAnswer[i].size(); ++j){
//            std::cout << "doc_id = " << preAnswer[i][j].first << " : Rank = " << preAnswer[i][j].second << std::endl;
//        }
//        std::cout << std::endl;
//    }
//}
