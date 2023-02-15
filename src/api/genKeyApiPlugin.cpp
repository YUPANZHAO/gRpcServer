#include "genKeyApiPlugin.h"
#include "loggerManager.h"
#include "nlohmann/json.hpp"
#include "DeviceManager.h"

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

    auto device = DeviceManager::getInstance()->addDevice(info.device_id);
    
    std::string msg(json({
        { "key", (*device)->key },
        { "url", (*device)->rtmp_url },
        { "encryption", (*device)->encryption }
    }).dump());
    
    return { std::nullopt, msg };
}
