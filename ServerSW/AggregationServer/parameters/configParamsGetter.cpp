
#include <getopt.h>
#include "configParamsGetter.h"
#include "Structures.h"

Config parseArguments(int argc, char **argv){
    Config params;
    getParamsFromConfigFile<Config &>(params, params.configFileName);

    int c;
    while ((c = getopt(argc, argv, "vp:")) != -1)
        switch (c) {
            case 'v':
                params.verbose = true;
                break;
            case 'p':
                params.port = std::stoi(optarg);
                break;
            default:
                break;
        }

    if (params.port < 0){
        std::cout << "Fill port -p <port>" << std::endl;
    }

    createLogger(params.verbose);
    return params;
}

void createLogger(bool verbose) {
    std::vector<spdlog::sink_ptr> sinks;

    // add stdout log
    if (verbose) {
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
    }

    //rotating file sink for error and warn
    auto console_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/Error_log.txt", 1024*1024*5, 5, false);
    console_sink->set_level(spdlog::level::err);
    sinks.push_back(console_sink);

    //rotating file sink for info
    sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/Info_log.txt", 1024*1024*5, 5, false));

    auto mdbrkLogger = std::make_shared<spdlog::logger>("Aggregation_server", begin(sinks), end(sinks));

    //todo change to error
    mdbrkLogger->flush_on(spdlog::level::info);

    spdlog::register_logger(mdbrkLogger);
}

int setVariableValue(const std::string &key, const std::string &value, Config &params) {
    if (key == "DBDataAddress") {
        params.database.db_address = value;
        return 0;
    }
    if (key == "DBDataPassword") {
        params.database.db_password = value;
        return 0;
    }
    if (key == "DBDataDatabase") {
        params.database.db_database = value;
        return 0;
    }
    if (key == "DBDataUser") {
        params.database.db_user = value;
        return 0;
    }
    if (key == "serverPort") {
        return getNumber(value, params.port);
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
        std::shared_ptr<spdlog::logger> logger = spdlog::get("Aggregation_server");
        logger->error("Cannot parse number from config fle");
        return -1;
    }
}