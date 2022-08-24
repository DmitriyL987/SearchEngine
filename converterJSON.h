//
// Created by LDm on 21.08.2022.
//

#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ConverterJSON {
    json config;
    std::string name;
    std::string version;
    int responsesLimit;
    int indexUpdate;

    std::vector<std::string> listDocuments;
    std::vector<std::string> requests;
    std::vector<std::vector<std::pair<int, float>>> answers;
public:
    ConverterJSON() ;

    std::vector<std::string> GetTextDocuments();

    int GetResponsesLimit();

    std::vector<std::string> GetRequests();

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);

    bool error = false;
};


#endif //SEARCH_ENGINE_CONVERTERJSON_H
