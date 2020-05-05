
#include "ParamsGetter.h"

Arguments parseArguments(int argc, char **argv){
    Arguments config;
    getParamsFromConfigFile(config);

    if(argc != 2){
        Logger::logError("UID in argument needed");
        exit(EXIT_FAILURE);
    }

    config.uid = std::string(argv[1]);


    return config;
}


int setVariableValue(const std::string &key, const std::string &value, Arguments &config) {
    if (key == "db_address") {
        config.db_address = value;
        return 0;
    }
    if (key == "db_password") {
        config.db_password = value;
        return 0;
    }
    if (key == "db_database") {
        config.db_database = value;
        return 0;
    }
    if (key == "db_user") {
        config.db_user = value;
        return 0;
    }
    // ignore unknown variables => return 0 not error
    return 0;
}

int getNumber(const std::string &value, int &number) {
    std::string::size_type sz;

    try {
        number = stoi(value, &sz);
        if(sz != value.size()){
            throw std::logic_error("");
        }
        return 0;
    }catch (std::logic_error &e) {
        Logger::logError("Cannot parse number from arguments fle");
        return -1;
    }
}
int getParamsFromConfigFile(Arguments &params) {// open and map arguments file
    std::ifstream configFile("config");

    if (!configFile.is_open()) {
        Logger::logError("Arguments file is not opened");
    }

    // operations
    std::string line;
    int lineNumber = 0;
    std::string firstWord;
    std::string secondWord;
    std::string extraWord;
    std::map<std::string, std::string> configMap;

    while (configFile.good() && getline(configFile, line)) {
        lineNumber++;
        // miss comments
        if (line[0] == '#') { continue; }

        std::basic_stringstream<char> iss(line);

        // skip blank line
        if (!(iss >> std::ws >> firstWord)) {
            continue;
        }

        if (!(iss >> std::ws >> secondWord)) {
            Logger::logError("No second word on line {}"+ std::to_string(lineNumber));
            continue;
        }

        if (iss >> extraWord) {
            if(extraWord[0] == '#'){  }
            else {
                Logger::logError("Extra word on line " + std::to_string(lineNumber));
                continue;
            }
        }

        if (setVariableValue(firstWord, secondWord, params)) {
            Logger::logError("Variable in arguments is not assignable to any variable on line "+ std::to_string(lineNumber));
            return EXIT_FAILURE;
        }
    }

    configFile.close();
    return 0;
}

