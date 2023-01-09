#include "reqLiveStreamApiPlugin.h"
#include "nlohmann/json.hpp"
#include "loggerManager.h"
#include "KeyManager.h"

using nlohmann::json;

ReqLiveStreamApiPlugin::ReqLiveStreamApiPlugin() {}

ReqLiveStreamApiPlugin::~ReqLiveStreamApiPlugin() {}

auto ReqLiveStreamApiPlugin::method() -> ApiName {
    return "req_live_stream";
}

auto ReqLiveStreamApiPlugin::json_parser(const std::string & data) -> std::optional<ApiRequest> {
    REQ_LIVE_STREAM_INFO info = {};
    auto json_data = json::parse(data);
    try {
        info.key = json_data["key"];
    }catch(std::exception&) {
        Warn(GLOBAL_LOG, "method({}): json parse failed!", method());
        return std::nullopt;
    }
    ApiRequest req = info;
    return req;
}

auto ReqLiveStreamApiPlugin::process(const ApiRequest & req) -> ApiReply {
    auto info = std::get<REQ_LIVE_STREAM_INFO>(req);
    Info(GLOBAL_LOG, "method({}): key = {}", method(), info.key);
    
    auto rtmp = KeyManager::getInstance()->genRtmpUrl(info.key);
    if(rtmp.empty()) {
        return { fmt::format("does not exist key [{}]!", info.key), "" };        
    }

    std::string msg(json({
        { "rtmp_url", rtmp }
    }).dump());
    
    return  { std::nullopt, msg };
}
