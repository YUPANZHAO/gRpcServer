#include "loggerManager.h"

LoggerManager::LoggerManager() {}

LoggerManager::~LoggerManager() {
    clearAllLogger();
}

auto LoggerManager::getInstance() -> LoggerManager* {
    static LoggerManager manager;
    return &manager;
}

bool LoggerManager::addLogger(LoggerName name, LoggerPtr ptr) {
    std::lock_guard<std::mutex> guard(_mutex);
    auto iter = _logger_map.find(name);
    if(iter != _logger_map.end()) return false;
    _logger_map[name] = ptr;
    return true;
}

auto LoggerManager::getLogger(LoggerName name) -> LoggerPtr {
    std::lock_guard<std::mutex> guard(_mutex);
    auto iter = _logger_map.find(name);
    if(iter == _logger_map.end()) return nullptr;
    return iter->second;
}

bool LoggerManager::removeLogger(LoggerName name) {
    std::lock_guard<std::mutex> guard(_mutex);
    auto iter = _logger_map.find(name);
    if(iter == _logger_map.end()) return false;
    iter->second->destroy();
    _logger_map.erase(iter);
    return true;
}

void LoggerManager::clearAllLogger() {
    std::lock_guard<std::mutex> guard(_mutex);
    auto iter = _logger_map.begin();
    for(; iter != _logger_map.end(); ) {
        iter->second->destroy();
        iter = _logger_map.erase(iter);
    }
}
