#include "logoutApiPlugin.h"
#include "loggerManager.h"
#include "nlohmann/json.hpp"

using nlohmann::json;

LogoutApiPlugin::LogoutApiPlugin() {}

LogoutApiPlugin::~LogoutApiPlugin() {}

auto LogoutApiPlugin::method() -> ApiName {
    return "logout";
}

auto LogoutApiPlugin::json_parser(const std::string & data) -> std::optional<ApiRequest> {
    LOGOUT_INFO info = {};
    auto json_data = json::parse(data);
    try {
        info.token = json_data["token"];
    }catch(std::exception&) {
        Warn(GLOBAL_LOG, "method({}): json parse failed!", method());
        return std::nullopt;
    }
    ApiRequest req = info;
    return req;
}

auto LogoutApiPlugin::process(const ApiRequest & req) -> ApiReply {
    auto info = std::get<LOGOUT_INFO>(req);
    Info(GLOBAL_LOG, "method({}): token = {}", method(), info.token);
    UserManager::getInstance()->logout(info.token);
    return { std::nullopt, "" };
}