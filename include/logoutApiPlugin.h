#pragma once

#include "commonInterface.hpp"
#include "UserManager.h"

class LogoutApiPlugin : public IApiPlugin {
public:
    LogoutApiPlugin();
    ~LogoutApiPlugin();
    
public:
    auto method() -> ApiName;
    auto json_parser(const std::string &) -> std::optional<ApiRequest>;
    auto process(const ApiRequest &) -> ApiReply;
};

using LogoutApiPluginPtr = std::shared_ptr<LogoutApiPlugin>;