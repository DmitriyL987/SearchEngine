//
// Created by LDm on 21.08.2022.
//

#include "converterJSON.h"

//json ConverterJSON::config;
//std::string ConverterJSON::name;
//std::string ConverterJSON::version;
//int ConverterJSON::responsesLimit;
//int ConverterJSON::indexUpdate;

//-------------------------------

void ConverterJSON::init() {
    std::ifstream configFile ("..\\config.json");
    if(!configFile.is_open()) {
        std::cerr << "config file is missing" << std::endl;

    } else {
        std::cout << "the configuration file has been opened successfully" << std::endl;
        json config;
        configFile >> config;

        if (!config.contains("config")) {
            std::cerr << "config file is empty" << std::endl;
        } else{
            std::cout<< "config is founded!" << std::endl;
            responsesLimit = config["config"]["max_responses"];
            indexUpdate = config["config"]["update"];
            name = config["config"]["name"];
            version = config["config"]["version"];
        }
        configFile.close();
    }
    std::cout<< "init - ok!" << std::endl;
}
//-------------------------------

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::ifstream configFile("..\\config.json");
    if(configFile.is_open()) {
        json config;
        configFile >> config;
        configFile.close();
        std::vector<std::string> textList;
        listDocuments.clear();
        json::iterator it = config.find("files");
        for (int i = 0; i < it->size(); ++i) {
            listDocuments.emplace_back((*it)[i]);
        }
        for(int i = 0; i < listDocuments.size(); ++i){
            std::ifstream file(listDocuments[i], std::ios::ate);
            std::cout << "file name: " << listDocuments[i] << "\n";
            if(file.is_open()){
                auto szFile = file.tellg();
                std::string str(szFile, '\0');
                file.seekg(0);
                file.read(&str[0], szFile);
                textList.emplace_back(str);
                file.close();
            } else {
                std::cerr << "Ошибка открытия файла" << std::endl;
            }
        }
        std::cout << "ok!" << "\n";
        std::cout << textList[1] << std::endl;
        return textList;
    }else return std::vector<std::string>();
}
//------------------------------

int ConverterJSON::GetResponsesLimit() {
    std::ifstream configFile("..\\config.json");
    if(configFile.is_open()) {
        json config;
        configFile >> config;
        configFile.close();
        json::iterator it = config.find("config");
        return (*it)["max_responses"];
    } else return -1;
}
//-------------------------------

std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> requests;
    std::ifstream req("requests.json");
    if(req.is_open()) {
        json j;
        req >> j;
        req.close();
        requests = j.get<std::vector<std::string>>();
        return requests;

    } else return requests;
}
//-------------------------------

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    std::ofstream fileAnswer("..\\answers.json");
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
                std::string keyRequests = "requests" + prefix + std::to_string(el+1);
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
    }else std::cerr << "failed to open/create file \"..\\answers.json\"" << std::endl;
}