#include "getDeviceInfoApiPlugin.h"
#include "nlohmann/json.hpp"
#include "loggerManager.h"
#include "DeviceManager.h"

using nlohmann::json;

GetDeviceInfoApiPlugin::GetDeviceInfoApiPlugin() {}

GetDeviceInfoApiPlugin::~GetDeviceInfoApiPlugin() {}

auto GetDeviceInfoApiPlugin::method() -> ApiName {
    return "get_device_info";
}

auto GetDeviceInfoApiPlugin::json_parser(const std::string & data) -> std::optional<ApiRequest> {
    GET_DEVICE_INFO info = {};
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

auto GetDeviceInfoApiPlugin::process(const ApiRequest & req) -> ApiReply {
    auto info = std::get<GET_DEVICE_INFO>(req);
    Info(GLOBAL_LOG, "method({}): key = {}", method(), info.key);
    
    auto device = DeviceManager::getInstance()->deviceInfo(info.key);
    if(device == nullopt) {
        return { fmt::format("does not exist device [{}]!", info.key), "" };        
    }

    std::string msg(json({
        { "name", (*device)->name },
        { "rtmp_url", (*device)->rtmp_url } 
    }).dump());
    
    return  { std::nullopt, msg };
}
