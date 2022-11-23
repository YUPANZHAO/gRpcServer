#pragma once

#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "commonInterface.hpp"
#include "yamlConfigDec.h"

class SpdLogger : public ILogger {
    using ConsoleSinkType = spdlog::sinks::stdout_color_sink_mt;
    using FileSinkType = spdlog::sinks::daily_file_sink_mt;
    using LoggerType = spdlog::logger;

public:
    SpdLogger(const ConfigNode &);
    ~SpdLogger();

public:
    void destroy() override;

    void info     (LogMessage) override;
    void error    (LogMessage) override;
    void debug    (LogMessage) override;
    void warn     (LogMessage) override;
    void critical (LogMessage) override;

private:
    auto convertLevel(LOG_LEVEL) -> spdlog::level::level_enum;
    auto convertLevel(const std::string &) -> spdlog::level::level_enum;

private:
    std::shared_ptr<ConsoleSinkType> _console_sink;
    std::shared_ptr<FileSinkType> _file_sink;
    std::unique_ptr<LoggerType> _logger;
};