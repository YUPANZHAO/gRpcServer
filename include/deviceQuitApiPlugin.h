#pragma once

#include "commonInterface.hpp"

class DeviceQuitApiPlugin : public IApiPlugin {
public:
    DeviceQuitApiPlugin();
    ~DeviceQuitApiPlugin();

public:
    auto method() -> ApiName;
    auto json_parser(const std::string &) -> std::optional<ApiRequest>;
    auto process(const ApiRequest &) -> ApiReply;
};

using DeviceQuitApiPluginPtr = std::shared_ptr<DeviceQuitApiPlugin>;