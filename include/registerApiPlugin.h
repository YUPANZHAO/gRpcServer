#pragma once

#include "commonInterface.hpp"
#include "UserManager.h"

class RegisterApiPlugin : public IApiPlugin {
public:
    RegisterApiPlugin();
    ~RegisterApiPlugin();
    
public:
    auto method() -> ApiName;
    auto json_parser(const std::string &) -> std::optional<ApiRequest>;
    auto process(const ApiRequest &) -> ApiReply;
};

using RegisterApiPluginPtr = std::shared_ptr<RegisterApiPlugin>;