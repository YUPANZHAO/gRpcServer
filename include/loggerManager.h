#pragma once

#include "commonInterface.hpp"
#include "spdLogger.h"

using FormatType = const std::string &;
using LoggerName = const std::string;
using LoggerPtr  = std::shared_ptr<ILogger>;

class LoggerManager {
public:
    ~LoggerManager();
    static auto getInstance() -> LoggerManager*;

public:
    bool addLogger(LoggerName, LoggerPtr);
    LoggerPtr getLogger(LoggerName);
    bool removeLogger(LoggerName);
    void clearAllLogger();

    template<typename... Args>
    inline void info(LoggerPtr logger, FormatType fmt, Args... args) {
        logger->info(fmt::format(fmt, args...));
    }
    template<typename... Args>
    inline void error(LoggerPtr logger, FormatType fmt, Args... args) {
        logger->error(fmt::format(fmt, args...));
    }
    template<typename... Args>
    inline void debug(LoggerPtr logger, FormatType fmt, Args... args) {
        logger->debug(fmt::format(fmt, args...));
    }
    template<typename... Args>
    inline void warn(LoggerPtr logger, FormatType fmt, Args... args) {
        logger->warn(fmt::format(fmt, args...));
    }
    template<typename... Args>
    inline void critical(LoggerPtr logger, FormatType fmt, Args... args) {
        logger->critical(fmt::format(fmt, args...));
    }
    
private:
    LoggerManager();
    LoggerManager(const LoggerManager &) = delete;
    LoggerManager& operator = (const LoggerManager &) = delete;

    std::map<LoggerName, LoggerPtr> _logger_map;
    std::mutex _mutex;
};

template<typename... Args>
inline void Info(LoggerName name, FormatType fmt, Args... args) {
    LoggerManager* manager = LoggerManager::getInstance();
    LoggerPtr logger = manager->getLogger(name);
    if(logger != nullptr) manager->info(logger, fmt, args...);
}
template<typename... Args>
inline void Error(LoggerName name, FormatType fmt, Args... args) {
    LoggerManager* manager = LoggerManager::getInstance();
    LoggerPtr logger = manager->getLogger(name);
    if(logger != nullptr) manager->error(logger, fmt, args...);
}
template<typename... Args>
inline void Debug(LoggerName name, FormatType fmt, Args... args) {
    LoggerManager* manager = LoggerManager::getInstance();
    LoggerPtr logger = manager->getLogger(name);
    if(logger != nullptr) manager->debug(logger, fmt, args...);
}
template<typename... Args>
inline void Warn(LoggerName name, FormatType fmt, Args... args) {
    LoggerManager* manager = LoggerManager::getInstance();
    LoggerPtr logger = manager->getLogger(name);
    if(logger != nullptr) manager->warn(logger, fmt, args...);
}
template<typename... Args>
inline void Critical(LoggerName name, FormatType fmt, Args... args) {
    LoggerManager* manager = LoggerManager::getInstance();
    LoggerPtr logger = manager->getLogger(name);
    if(logger != nullptr) manager->critical(logger, fmt, args...);
}
