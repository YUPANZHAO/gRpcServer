#include "heartBeatApiPlugin.h"
#include "loggerManager.h"
#include "nlohmann/json.hpp"
#include "DeviceManager.h"

using nlohmann::json;

HeartBeatApiPlugin::HeartBeatApiPlugin() {}

HeartBeatApiPlugin::~HeartBeatApiPlugin() {}

auto HeartBeatApiPlugin::method() -> ApiName {
    return "heart_beat";
}

auto HeartBeatApiPlugin::json_parser(const std::string & data) -> std::optional<ApiRequest> {
    HEART_BEAT_INFO info = {};
    auto json_data = json::parse(data);
    try {
        info.id = json_data["id"];
    }catch(std::exception&) {
        Warn(GLOBAL_LOG, "method({}): json parse failed!", method());
        return std::nullopt;
    }
    ApiRequest req = info;
    return req;
}

auto HeartBeatApiPlugin::process(const ApiRequest & req) -> ApiReply {
    auto info = std::get<HEART_BEAT_INFO>(req);
    Info(GLOBAL_LOG, "method({}): id = {}", method(), info.id);

    DeviceManager::getInstance()->heartBeat(info.id);
    
    return { std::nullopt, "" };
}
