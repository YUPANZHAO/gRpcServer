#pragma once

#include "commonInterface.hpp"

class LoginApiPlugin : public IApiPlugin {
public:
    LoginApiPlugin();
    ~LoginApiPlugin();
    
public:
    auto method() -> ApiName;
    auto json_parser(const std::string &) -> std::optional<ApiRequest>;
    auto process(const ApiRequest &) -> ApiReply;
};

using LoginApiPluginPtr = std::shared_ptr<LoginApiPlugin>;