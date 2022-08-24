//
// Created by LDm on 21.08.2022.
//

#include "converterJSON.h"

ConverterJSON::ConverterJSON() {
    std::cout << "Start..." << std::endl;
    std::ifstream configFile ("..\\config.json");
    if(!configFile.is_open()) {
        std::cerr << "config file is missing" << std::endl;
        error = true;
    } else {
        std::cout << "the configuration file has been opened successfully" << std::endl;
        configFile >> config;
        configFile.close();
        json::iterator cf = config.find("config");
        if (!config.contains("config")) {
            std::cerr << "config file is empty" << std::endl;
            error = true;
        }
    }
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> textList;
    json::iterator it = config.find("files");
    for(int i = 0; i < it->size();++i){
        textList.emplace_back((*it)[i]);
    }
    return textList;
}

int ConverterJSON::GetResponsesLimit() {
    json::iterator it = config.find("config");
    return (*it)["max_responses"];
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> requests;
    std::ifstream req("requests.json");
    if(req.is_open()) {
        json j;
        req >> j;
        req.close();
        json::iterator it = j.find("files");
        for (int i = 0; i < it->size(); ++i) requests.emplace_back((*it)[i]);
        return requests;
    } else return std::vector<std::string>();
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    std::ofstream fileAnswer("answers.json");
    if(fileAnswer.is_open()){
        json answerJSON;
        if(!answers.empty()) {
            int xxx = answers.size();
            answerJSON.value("answers", ""); //создание ключа "answers":
            for(int el = 0; el < answers.size(); ++el){
                //создание ключа "requests_XXX":
                std::string prefix;
                prefix.resize(3 - std::to_string(el + 1).size());
                std::fill(prefix.begin(), prefix.end(), '0');
                std::string keyRequests = "requests" + prefix + std::to_string(el + 1);
                answerJSON["answers"].value(keyRequests, "");
                if (!answers.empty()) answerJSON[keyRequests].value("result", "true");
                else answerJSON[keyRequests].value("result", "false");
                if(answers[el].size() > 1) {
                    answerJSON[keyRequests].value("relevance", "");
                    for(int i = 0; i < answers[el].size(); ++i) {
                        answerJSON[keyRequests]["relevance"].value("docid", answers[el][i].first);
                        answerJSON[keyRequests]["relevance"].value("rank", answers[el][i].second);
                    }
                } else {
                    answerJSON[keyRequests].value("docid", answers[el][0].first);
                    answerJSON[keyRequests].value("rank", answers[el][0].second);
                }
            }
        }
        fileAnswer << answerJSON;
        fileAnswer.close();
    }else std::cerr << "failed to open/create file \"answers.json\"" << std::endl;
}