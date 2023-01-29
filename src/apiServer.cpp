#include "apiServer.h"
#include "grpcServer.h"
#include <signal.h>
#include "loggerManager.h"
#include "yamlConfigDec.h"
#include "loginApiPlugin.h"
#include "genKeyApiPlugin.h"
#include "DeviceManager.h"
#include "nlohmann/json.hpp"
#include "getDeviceInfoApiPlugin.h"
#include "MessageQueue.h"
#include "msgCBApiPlugin.h"
#include "talkCtrlApiPlugin.h"
#include "deviceQuitApiPlugin.h"
#include "heartBeatApiPlugin.h"

using nlohmann::json;

struct ApiServer::ApiServerImpl {
    ~ApiServerImpl();

    bool start();
    void wait();
    void shutdown();

    static auto getInstance() -> ApiServerImpl*;
    static void handle_shutdown(int);

    auto handle_service_request(const std::string&) -> std::string;
    auto handle_message_callback(const std::string&) -> std::string;

    bool loadConfigFile(const std::string &);
    bool initLogger();
    void registerPlugins();
    bool startRpcServer();

    LoggerManager* _loggerManager_ptr;
    std::unique_ptr<IRpcServer> _grpcServer_ptr;
    std::unique_ptr<YamlConfigDec> _configDec_ptr;
    ConfigNode _config;
    std::map<ApiName, IApiPluginPtr> _plugin_map;
    MessageQueue<std::string> _msgQue;
};

ApiServer::ApiServerImpl::~ApiServerImpl() = default;

bool ApiServer::ApiServerImpl::start() {
    if(loadConfigFile("config.yaml") == false) return false;
    if(initLogger() == false) return false;
    registerPlugins();
    if(startRpcServer() == false) return false;
    DeviceManager::getInstance()->init(_config["rtmp"]);
    DeviceManager::getInstance()->initMsgQuePtr(
        shared_ptr<MessageQueue<std::string>>(&_msgQue)
    );
    DeviceManager::getInstance()->startHeartBeatHandleTimer();
    return true;
}

void ApiServer::ApiServerImpl::wait() {
    signal(SIGINT, handle_shutdown);

    _grpcServer_ptr->wait();
}

void ApiServer::ApiServerImpl::shutdown() {
    _grpcServer_ptr->shutdown();
    _loggerManager_ptr->clearAllLogger();
}

auto ApiServer::ApiServerImpl::getInstance() -> ApiServerImpl* {
    static auto instance = std::make_unique<ApiServerImpl>();
    return instance.get();
}

void ApiServer::ApiServerImpl::handle_shutdown(int signal) {
    auto *apiServerImpl = getInstance();
    apiServerImpl->shutdown();
}

auto ApiServer::ApiServerImpl::handle_service_request(const std::string& json_data) -> std::string {
    Debug(GLOBAL_LOG, "service request data:\r\n{}", json_data);

    std::string method;
    try { 
        json data = json::parse(json_data);
        method = data["method"]; 
    } catch(std::exception&) { 
        Warn(GLOBAL_LOG, "service request data error, can't find method!");
        return json({
            { "code", 400 },
            { "msg", "json format failed or no method!" }
        }).dump();
    }

    if(_plugin_map.find(method) == _plugin_map.end()) {
        Warn(GLOBAL_LOG, "service request, no match method plugin!");
        return json({
            { "code", 400 },
            { "msg", "no match method plugin!" }
        }).dump();
    }

    auto plugin = _plugin_map[method];
    
    auto request = plugin->json_parser(json_data);
    if(request == std::nullopt) {
        Warn(GLOBAL_LOG, "json parser failed! method: {}", method);
        return json({
            { "code", 400 },
            { "msg", "json parser failed!" }
        }).dump();
    }
    
    auto [ err, reply ] = plugin->process(*request);
    if(err != std::nullopt) {
        Warn(GLOBAL_LOG, "running error! method({}) errMsg: {}", method, *err);
        return json({
            { "code", 500 },
            { "msg", "running error!" },
            { "errMsg", *err }
        }).dump();
    }
    
    if(method == "message_callback" && reply == "") return "";

    Debug(GLOBAL_LOG, "service reply data:\r\n{}", reply);
    return json({
        { "code", 200 },
        { "msg", "OK" },
        { "reply", reply == "" ? "" : json::parse(reply) }
    }).dump();
}

auto ApiServer::ApiServerImpl::handle_message_callback(const std::string & id) -> std::string {
    auto ret = _msgQue.get(id);
    json data = json::parse(*ret);
    if(data["operation"] == "close msg cb") ret = std::nullopt;
    if(ret == std::nullopt) return "";
    return (*ret);
}

bool ApiServer::ApiServerImpl::loadConfigFile(const std::string & filename) {
    _configDec_ptr = std::make_unique<YamlConfigDec>();
    auto [ config, err ] = _configDec_ptr->decode(filename);
    _config = config["config"];
    return !err;
}

bool ApiServer::ApiServerImpl::initLogger() {
    auto log = _config["log"];
    _loggerManager_ptr = LoggerManager::getInstance();
    return _loggerManager_ptr->addLogger(
        GLOBAL_LOG,
        [&]() {
            std::string lib = log["lib"];
            if(lib == "spdlog") return std::make_shared<SpdLogger>(log);
            return std::make_shared<SpdLogger>(log);
        }()
    );
}

void ApiServer::ApiServerImpl::registerPlugins() {
    std::vector<IApiPluginPtr> plugins {
        std::make_shared<LoginApiPlugin>(),
        std::make_shared<GenKeyApiPlugin>(),
        std::make_shared<GetDeviceInfoApiPlugin>(),
        std::make_shared<TalkCtrlApiPlugin>(),
        std::make_shared<MsgCBApiPlugin>([this](const std::string & params) -> std::string {
            return this->handle_message_callback(params);
        }),
        std::make_shared<DeviceQuitApiPlugin>(),
        std::make_shared<HeartBeatApiPlugin>(),
        // Add New Api Plugin
    };
    for (IApiPluginPtr plugin : plugins) {
        if(_plugin_map.find(plugin->method()) != _plugin_map.end()) {
            Warn(GLOBAL_LOG, "api plugin conflict!!! method name: \'{}\'", plugin->method());
            continue;
        }
        _plugin_map[plugin->method()] = plugin;
    }
}

bool ApiServer::ApiServerImpl::startRpcServer() {
    auto api = _config["api"];
    _grpcServer_ptr = [&]() {
        std::string lib = api["lib"];
        if(lib == "grpc") return std::make_unique<GrpcServer>();
        return std::make_unique<GrpcServer>();
    }();
    _grpcServer_ptr->init([&](const std::string& data) -> std::string {
        return handle_service_request(data);
    }, api);
    return _grpcServer_ptr->start();
}

ApiServer::ApiServer()
    : _impl(ApiServerImpl::getInstance()) {}

ApiServer::~ApiServer() = default;

bool ApiServer::start() { return _impl->start(); }

void ApiServer::wait() { _impl->wait(); }

void ApiServer::shutdown() { _impl->shutdown(); }