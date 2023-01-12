#pragma once

#include "commonInterface.hpp"

class GetDeviceInfoApiPlugin : public IApiPlugin {
public:
    GetDeviceInfoApiPlugin();
    ~GetDeviceInfoApiPlugin();

public:
    auto method() -> ApiName;
    auto json_parser(const std::string &) -> std::optional<ApiRequest>;
    auto process(const ApiRequest &) -> ApiReply;
};

using GetDeviceInfoApiPluginPtr = std::shared_ptr<GetDeviceInfoApiPlugin>;