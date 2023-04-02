#pragma once

#include <functional>
#include <memory>
#include <iostream>
#include <map>
#include <mutex>
#include <variant>
#include <optional>
#include <list>
#include <tuple>
#include "yamlConfigDec.h"

const int KEY_LEN = 6;
const int TOKEN_LEN = 16;
const int ENCRYPTION_LEN = 8;

using BufferCallBack = std::function<void(char*,int)>;

using ServiceFunc = std::function<std::string(const std::string&)>;
using ServiceBufferFunc = std::function<void(const std::string&,const std::string&,BufferCallBack)>;

class IApiServer {
public:
    virtual ~IApiServer() {}
    virtual bool start() = 0;
    virtual void wait() = 0;
    virtual void shutdown() = 0;
};

class IRpcServer {
public:
    virtual ~IRpcServer() {}
    virtual void init(ServiceFunc, ServiceBufferFunc, ConfigNode) = 0;
    virtual bool start() = 0;
    virtual void wait() = 0;
    virtual void shutdown() = 0;
};

using LogMessage = const std::string &;
class ILogger {
public:
    virtual ~ILogger() {}
    virtual void destroy() = 0;
    virtual void info(LogMessage) = 0;
    virtual void error(LogMessage) = 0;
    virtual void debug(LogMessage) = 0;
    virtual void warn(LogMessage) = 0;
    virtual void critical(LogMessage) = 0;
};

enum class LOGGER_MODE { CONSOLE, FILE, BOTH };
enum class LOG_LEVEL { INFO, ERROR, DEBUG, WARN, CRITICAL }; 

const std::string GLOBAL_LOG = "GLOBAL_LOG";
const std::string RECORDER_LOG = "RECORDER_LOG";

struct LOGIN_INFO {
    std::string username;
    std::string password;
};

struct REGISTER_INFO {
    std::string username;
    std::string password;
};

struct LOGOUT_INFO {
    std::string token;
};

struct GEN_KEY_INFO {
    std::string device_id;
    std::string device_name;
};

struct GET_DEVICE_INFO {
    std::string user;
    std::string key;
};

struct TALK_CTRL_INFO {
    std::string ctrl_type;
    std::string device_key;
};

struct MSG_CB_INFO {
    std::string id;
};

struct DEVICE_QUIT_INFO {
    std::string id;
};

struct HEART_BEAT_INFO {
    std::string id;
};

struct RECORD_DOWNLOAD_INFO {
    std::string key;
    std::string begin_time;
    std::string end_time;
};

using ApiRequest = std::variant<LOGIN_INFO, REGISTER_INFO, LOGOUT_INFO, GEN_KEY_INFO, GET_DEVICE_INFO, TALK_CTRL_INFO,
                                MSG_CB_INFO, DEVICE_QUIT_INFO, HEART_BEAT_INFO, RECORD_DOWNLOAD_INFO>;
using ApiReply = std::tuple<std::optional<std::string>, std::string>;
using ApiName = const std::string;
class IApiPlugin {
public:
    virtual ~IApiPlugin() {}
    virtual auto method() -> ApiName = 0;
    virtual auto json_parser(const std::string &) -> std::optional<ApiRequest> = 0;
    virtual auto process(const ApiRequest &) -> ApiReply = 0;
};
using IApiPluginPtr = std::shared_ptr<IApiPlugin>;

struct DeviceInfo {
    // 固定信息
    std::string name;
    std::string device_id;
    std::string key;
    std::string rtmp_url;
    std::string encryption;
    // 动态信息
    bool is_talking;
    bool is_active;
    time_t last_active_time;
};
using DeviceInfoPtr = std::shared_ptr<DeviceInfo>;

struct RecordMessage {
    char key[10];
    char rtmp_url[50];
};

struct UserInfo {
    std::string name;
    std::string password;
    std::string token;
};
using UserInfoPtr = std::shared_ptr<UserInfo>;