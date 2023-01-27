#include "msgCBApiPlugin.h"
#include "loggerManager.h"
#include "nlohmann/json.hpp"
#include "DeviceManager.h"
#include "MessageQueue.h"

using nlohmann::json;

MsgCBApiPlugin::MsgCBApiPlugin(ServiceFunc func) : _cb(func) {}

MsgCBApiPlugin::~MsgCBApiPlugin() {}

auto MsgCBApiPlugin::method() -> ApiName {
    return "message_callback";
}

auto MsgCBApiPlugin::json_parser(const std::string & data) -> std::optional<ApiRequest> {
    MSG_CB_INFO info = {};
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

auto MsgCBApiPlugin::process(const ApiRequest & req) -> ApiReply {
    auto info = std::get<MSG_CB_INFO>(req);
    Info(GLOBAL_LOG, "method({}): id = {}", method(), info.id);
    
    auto data = _cb(info.id);

    return { std::nullopt, data };
}