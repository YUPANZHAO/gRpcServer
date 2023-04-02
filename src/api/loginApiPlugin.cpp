#include "loginApiPlugin.h"
#include "loggerManager.h"
#include "nlohmann/json.hpp"

using nlohmann::json;

LoginApiPlugin::LoginApiPlugin() {}

LoginApiPlugin::~LoginApiPlugin() {}

auto LoginApiPlugin::method() -> ApiName {
    return "login";
}

auto LoginApiPlugin::json_parser(const std::string & data) -> std::optional<ApiRequest> {
    LOGIN_INFO info = {};
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

auto LoginApiPlugin::process(const ApiRequest & req) -> ApiReply {
    auto info = std::get<LOGIN_INFO>(req);
    Info(GLOBAL_LOG, "method({}): username = {}, password = {}", method(), info.username, info.password);
    auto token = UserManager::getInstance()->signIn(info.username, info.password);
    std::string msg;
    if(token == std::nullopt) {
        msg = json({
            { "msg", "failure" }
        }).dump();
    }else {
        auto key_set = UserManager::getInstance()->getAllDevice(*token);
        msg = json({
            { "token", (*token) },
            { "msg", "success" },
            { "device_list", json(key_set) },
        }).dump();
    }
    return { std::nullopt, msg };
}