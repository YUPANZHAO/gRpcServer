#include "spdLogger.h"

SpdLogger::SpdLogger(const ConfigNode & log) {
    int          mode             =  log["mode"];
    std::string  console_level    =  log["console"]["level"];
    std::string  file_level       =  log["file"]["level"];
    std::string  console_pattern  =  log["console"]["pattern"];
    std::string  file_pattern     =  log["file"]["pattern"];
    std::string  file_name        =  log["file"]["name"];
    int          rotation_hour    =  log["file"]["rotation_hour"];
    int          rotation_minute  =  log["file"]["rotation_minute"];
    bool         truncate         =  log["file"]["truncate"];
    std::string  flush_on         =  log["flush_on"];
    
    if(mode == 0 || mode == 2) {
        _console_sink = std::make_shared<ConsoleSinkType>();
        _console_sink->set_level(convertLevel(console_level));
        if(console_pattern.empty()) _console_sink->set_pattern(console_pattern);
    }
    if(mode == 1 || mode == 2) {
        _file_sink = std::make_shared<FileSinkType>(file_name, rotation_hour, rotation_minute, truncate);
        _file_sink->set_level(convertLevel(file_level));
        if(file_pattern.empty()) _file_sink->set_pattern(file_pattern);
    }
    if(mode == 0) {
        _logger = std::unique_ptr<LoggerType>(
            new LoggerType("spdlog", {_console_sink})
        );
    }else if(mode == 1) {
        _logger = std::unique_ptr<LoggerType>(
            new LoggerType("spdlog", {_file_sink})
        );
    }else {
        _logger = std::unique_ptr<LoggerType>(
            new LoggerType("spdlog", {_console_sink, _file_sink})
        );
    }
    _logger->flush_on(convertLevel(flush_on));
}

SpdLogger::~SpdLogger() {
    destroy();
}

void SpdLogger::destroy() {
    spdlog::shutdown();
}

void SpdLogger::info (LogMessage logMsg) {
    _logger->info(logMsg);
}

void SpdLogger::error (LogMessage logMsg) {
    _logger->error(logMsg);
}

void SpdLogger::debug (LogMessage logMsg) {
    _logger->debug(logMsg);
}

void SpdLogger::warn (LogMessage logMsg) {
    _logger->warn(logMsg);
}

void SpdLogger::critical (LogMessage logMsg) {
    _logger->critical(logMsg);
}

auto SpdLogger::convertLevel(const std::string & level) -> spdlog::level::level_enum {
    if(level == "info")     return spdlog::level::info;
    if(level == "error")    return spdlog::level::err;
    if(level == "debug")    return spdlog::level::debug;
    if(level == "warn")     return spdlog::level::warn;
    if(level == "critical") return spdlog::level::critical;
    return spdlog::level::n_levels;
}

auto SpdLogger::convertLevel(LOG_LEVEL level) -> spdlog::level::level_enum {
    switch (level)
    {
    case LOG_LEVEL::INFO:
        return spdlog::level::info;
    case LOG_LEVEL::ERROR:
        return spdlog::level::err;
    case LOG_LEVEL::DEBUG:
        return spdlog::level::debug;
    case LOG_LEVEL::WARN:
        return spdlog::level::warn;
    case LOG_LEVEL::CRITICAL:
        return spdlog::level::critical;
    default:
        break;
    }
    return spdlog::level::n_levels;
}