
//#include <gtest/gtest.h>
#include "converterJSON.h"
#include "invertedIndex.hpp"
#include <chrono>
#include <iostream>
#include "searchEngine.hpp"
#include <vector>

//TEST(sample_test_case, sample_test)
//{
//    EXPECT_EQ(1, 1);
//}


std::mutex mutex;

void checkConditionExit(bool&);

void test(std::vector<std::vector<std::pair<int, float>>> preAnswer);

int main() {
    ConverterJSON converter;
    invertedIndex inverter;
    std::vector<std::string> texts;
    texts = converter.GetTextDocuments();

//    bool noExit = true;
//    auto *check = new std::thread(checkConditionExit, std::ref(noExit));
//    while (noExit){
//        inverter.UpdateDocumentBase(Converter.GetTextDocuments());
//
//        std::this_thread::sleep_for(std::chrono::seconds(5));
//    }
//    if(check->joinable())check->join();


//---------------- тест --------------------------------------------------------
    //std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;

    inverter.UpdateDocumentBase(texts);
    std::vector<std::string> a = converter.GetRequests();
    //std::vector<std::vector<std::string>> words;

    std::vector<std::vector<RelativeIndex>> tmp;
    SearchServer server (inverter);
    std::cout << "creat object SearchServer\n";


    //std::this_thread::sleep_for(std::chrono::seconds(1));
    tmp = server.search(a);
    std::cout << "return RelativIndex\n";
    std::vector<std::vector<std::pair<int, float>>> preAnswer;
    for(int i = 0;i < tmp.size(); ++i){
        for( int j = 0; j < tmp[i].size(); ++j){
            preAnswer[i][j].first = tmp[i][j].doc_id;
            preAnswer[i][j].second = tmp[i][j].rank;
        }
    }
    test( preAnswer);
    converter.putAnswers(preAnswer);

    return 0;
}

//--------------------------------------
void checkConditionExit(bool& cond){
    std::string str;
    mutex.lock();
    std::cin >> str;
    mutex.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    if(str == "exit") {
        mutex.lock();
        cond = false;
        mutex.unlock();
    }
}

void  test(std::vector<std::vector<std::pair<int, float>>> preAnswer){
    for(int i = 0;i < preAnswer.size(); ++i){
        for( int j = 0; j < preAnswer[i].size(); ++j){
            std::cout << "doc_id = " << preAnswer[i][j].first << " : Rank = " << preAnswer[i][j].second << std::endl;
        }
    }
}
