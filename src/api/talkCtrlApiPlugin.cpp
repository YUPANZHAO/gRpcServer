#include "talkCtrlApiPlugin.h"
#include "loggerManager.h"
#include "nlohmann/json.hpp"
#include "DeviceManager.h"

using nlohmann::json;

TalkCtrlApiPlugin::TalkCtrlApiPlugin() {}

TalkCtrlApiPlugin::~TalkCtrlApiPlugin() {}

auto TalkCtrlApiPlugin::method() -> ApiName {
    return "talk_ctrl";
}

auto TalkCtrlApiPlugin::json_parser(const std::string & data) -> std::optional<ApiRequest> {
    TALK_CTRL_INFO info = {};
    auto json_data = json::parse(data);
    try {
        info.ctrl_type = json_data["ctrl_type"];
        info.device_key = json_data["device_key"];
    }catch(std::exception&) {
        Warn(GLOBAL_LOG, "method({}): json parse failed!", method());
        return std::nullopt;
    }
    ApiRequest req = info;
    return req;
}

auto TalkCtrlApiPlugin::process(const ApiRequest & req) -> ApiReply {
    auto info = std::get<TALK_CTRL_INFO>(req);
    Info(GLOBAL_LOG, "method({}): ctrl_type = {}, device_key = {}", method(), info.ctrl_type, info.device_key);
    
    bool ret = false;
    std::string msg;

    if(info.ctrl_type == "start") {
        auto [str, ret] = handle_start(info.device_key);
        msg = json({
            { "result", (ret ? "success" : "failed") },
            { (ret ? "rtmp_push_url" : "err_msg"), str }
        }).dump();
    }else if(info.ctrl_type == "stop") {
        handle_stop(info.device_key);
        msg = json({
            { "result", "success" }
        }).dump();
    }else {
        msg = json({
            { "result", "failed" },
            { "err_msg", "not exist this operation" }
        }).dump();
    }

    return { std::nullopt, msg };
}

auto TalkCtrlApiPlugin::handle_start(const std::string & device_key) -> std::tuple<std::string, bool> {
    auto manager = DeviceManager::getInstance();
    auto rtmp_push_url = manager->talkStart(device_key);
    if(rtmp_push_url == nullopt) return { "无法连接该设备", false };
    return { (*rtmp_push_url), true };
}

void TalkCtrlApiPlugin::handle_stop(const std::string & device_key) {
    auto manager = DeviceManager::getInstance();
    manager->talkStop(device_key);
}