#include <gtest/gtest.h>
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

void TestInvertedIndexFunctionaliti(
        std::vector<std::string> &docs,
        const std::vector<std::string> & requests,
        const std::vector<std::vector<Entry>> & expected
        ){
    std::vector<std::vector<Entry>> result;
    invertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for(auto & request : requests){
        std::vector<Entry> wordCount = idx.GetWordCount(request);
        result.push_back(wordCount);
    }
    ASSERT_EQ(result, expected);
}
TEST(TestCaseInvertedIndex, TestBasic){
    std::vector<std::string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock" };
    const std::vector<std::string> requests = {"london", "the"};
    const std::vector<std::vector<Entry>> expected = {
            {{1,1}},
            {{1,0},{3,1}}  };
    TestInvertedIndexFunctionaliti(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2){
    std::vector<std::string> docs = {
            "milk, milk, milk, milk, water, water, water",
            "milk, water, water",
            "milk, milk, milk, milk, milk, water, water, water, water, water",
            "americano, cappuccino"
    };
    const std::vector<std::string> requests = {"milk", "water", "cappuccino"};
    const std::vector<std::vector<Entry>> expected = {
            {{4,0},{1,1},{5,2}},
            {{2,0},{2,1},{5,2}},
            {{3,1}}
    };
    TestInvertedIndexFunctionaliti(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord){
    std::vector<std::string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    const std::vector<std::string> requests = {"m", "statement"};
    const std::vector<std::vector<Entry>> expected = {
            {},
            {{1,1}}
        };
    TestInvertedIndexFunctionaliti(docs, requests, expected);
}

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
