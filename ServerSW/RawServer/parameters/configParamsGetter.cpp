#include <getopt.h>
#include "configParamsGetter.h"

using namespace std;

Config parseArguments(int argc, char **argv){
    Config params;
    params.verbose = false;
    params.port = 9944;
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
    spdlog::drop("Raw_server");
    std::vector<spdlog::sink_ptr> sinks;

    // add stdout log
    if (verbose) {
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
    }

    //rotating tdmsFile sink for error and warn
    auto console_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/Error_log.txt", 1024*1024*5, 5, false);
    console_sink->set_level(spdlog::level::err);
    sinks.push_back(console_sink);

    //rotating tdmsFile sink for info
    sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/Info_log.txt", 1024*1024*5, 5, false));

    auto mdbrkLogger = std::make_shared<spdlog::logger>("Raw_server", begin(sinks), end(sinks));

    //todo change to error
    mdbrkLogger->flush_on(spdlog::level::info);

    spdlog::register_logger(mdbrkLogger);
}