//
// Created by Jakub Trubka on 27.11.19.
//

#include "Logger.h"

void Logger::initLogger(std::string logPath, bool verbose) {
    try
    {
        std::vector<spdlog::sink_ptr> sinks;

        //console sink
        if(verbose){
            sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
        }

        //rotating file sink for error and warn logs
        auto console_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logPath + "reader_error_log.txt", 1024*1024*5, 5, false);
        console_sink->set_level(spdlog::level::warn);
        sinks.push_back(console_sink);

        //rotating file sink for all logs
        sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logPath + "reader_info_log.txt", 1024*1024*5, 5, false));

        //create synchronous  loggers
        auto readerLogger = std::make_shared<spdlog::logger>("logger", begin(sinks), end(sinks));

        readerLogger->flush_on(spdlog::level::warn);

        // globally register the loggers so they can be accessed using spdlog::get(logger_name)
        spdlog::register_logger(readerLogger);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Logger::logInfo(std::string log) {
    std::shared_ptr<spdlog::logger> logger;
    logger = spdlog::get("logger");
    logger->info(log);
}

void Logger::logWarning(std::string log) {
    std::shared_ptr<spdlog::logger> logger;
    logger = spdlog::get("logger");
    logger->warn(log);
}

void Logger::logError(std::string log) {
    std::shared_ptr<spdlog::logger> logger;
    logger = spdlog::get("logger");
    logger->error(log);
}
