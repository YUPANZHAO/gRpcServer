#include "genKeyApiPlugin.h"
#include "loggerManager.h"
#include "nlohmann/json.hpp"
#include "KeyManager.h"

using nlohmann::json;

GenKeyApiPlugin::GenKeyApiPlugin() {}

GenKeyApiPlugin::~GenKeyApiPlugin() {}

auto GenKeyApiPlugin::method() -> ApiName {
    return "genkey";
}

auto GenKeyApiPlugin::json_parser(const std::string & data) -> std::optional<ApiRequest> {
    GEN_KEY_INFO info = {};
    auto json_data = json::parse(data);
    try {
        info.device_id = json_data["device_id"];
    }catch(std::exception&) {
        Warn(GLOBAL_LOG, "method({}): json parse failed!", method());
        return std::nullopt;
    }
    ApiRequest req = info;
    return req;
}

auto GenKeyApiPlugin::process(const ApiRequest & req) -> ApiReply {
    auto info = std::get<GEN_KEY_INFO>(req);
    Info(GLOBAL_LOG, "method({}): device_id = {}", method(), info.device_id);

    std::string key = KeyManager::getInstance()->generator(info.device_id);
    std::string rtmp = KeyManager::getInstance()->genRtmpUrl(key);
    
    std::string msg(json({
        { "key", key },
        { "url", rtmp }
    }).dump());
    
    return { std::nullopt, msg };
}
