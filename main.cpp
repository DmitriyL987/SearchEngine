#include <thread>
#include <mutex>
#include <gtest/gtest.h>
//#include "converterJSON.h"
//#include "invertedIndex.hpp"
#include <chrono>

#include "searchEngine.hpp"

TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);
}


std::mutex mutex;

void checkConditionExit(bool&);

int main() {
    ConverterJSON Converter;
    invertedIndex inverter;
    std::vector<std::string> texts;
    texts = Converter.GetTextDocuments();


//    bool noExit = true;
//    auto *check = new std::thread(checkConditionExit, std::ref(noExit));
//    while (noExit){
//        inverter.UpdateDocumentBase(Converter.GetTextDocuments());
//
//        std::this_thread::sleep_for(std::chrono::seconds(5));
//    }
//    if(check->joinable())check->join();


//---------------- тест --------------------------------------------------------
    std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;

    inverter.UpdateDocumentBase(texts);

    std::vector<std::vector<std::string>> words;

    for(int i = 0; i<texts.size(); ++i) words.emplace_back(inverter.separationWord(texts[i]));

    for(auto it_vec : inverter.getDictionary()){
        std::cout << "WORD: " << it_vec.first << "\n\t\t{ ";
        for(int i = 0; i < it_vec.second.size(); ++i)
        {
            std::cout << " {"<< it_vec.second[i].doc_id << ", "
                      << it_vec.second[i].count << "} ";
        }
        std::cout << " };"<< std::endl;
    }

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
