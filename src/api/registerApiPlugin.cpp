#include "registerApiPlugin.h"
#include "loggerManager.h"
#include "nlohmann/json.hpp"

using nlohmann::json;

RegisterApiPlugin::RegisterApiPlugin() {}

RegisterApiPlugin::~RegisterApiPlugin() {}

auto RegisterApiPlugin::method() -> ApiName {
    return "register";
}

auto RegisterApiPlugin::json_parser(const std::string & data) -> std::optional<ApiRequest> {
    REGISTER_INFO info = {};
    auto json_data = json::parse(data);
    try {
        info.username = json_data["username"];
        info.password = json_data["password"];
    }catch(std::exception&) {
        Warn(GLOBAL_LOG, "method({}): json parse failed!", method());
        return std::nullopt;
    }
    ApiRequest req = info;
    return req;
}

auto RegisterApiPlugin::process(const ApiRequest & req) -> ApiReply {
    auto info = std::get<REGISTER_INFO>(req);
    Info(GLOBAL_LOG, "method({}): username = {}, password = {}", method(), info.username, info.password);
    auto ret = UserManager::getInstance()->signUp(info.username, info.password);
    std::string msg;
    if(ret == std::nullopt) {
        msg = json({
            { "msg", "success" }
        }).dump();
    }else {
        msg = json({
            { "msg", "failure" },
            { "reason", (*ret) }
        }).dump();
    }
    return { std::nullopt, msg };
}