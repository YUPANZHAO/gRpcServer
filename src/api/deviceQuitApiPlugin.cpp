#include "deviceQuitApiPlugin.h"
#include "loggerManager.h"
#include "nlohmann/json.hpp"
#include "DeviceManager.h"

using nlohmann::json;

DeviceQuitApiPlugin::DeviceQuitApiPlugin() {}

DeviceQuitApiPlugin::~DeviceQuitApiPlugin() {}

auto DeviceQuitApiPlugin::method() -> ApiName {
    return "device_quit";
}

auto DeviceQuitApiPlugin::json_parser(const std::string & data) -> std::optional<ApiRequest> {
    DEVICE_QUIT_INFO  info = {};
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

auto DeviceQuitApiPlugin::process(const ApiRequest & req) -> ApiReply {
    auto info = std::get<DEVICE_QUIT_INFO>(req);
    Info(GLOBAL_LOG, "method({}): id = {}", method(), info.id);
    
    DeviceManager::getInstance()->setDeviceQuit(info.id);

    return { std::nullopt, "" };
}
