#pragma once

#include "commonInterface.hpp"

class HeartBeatApiPlugin : public IApiPlugin {
public:
    HeartBeatApiPlugin();
    ~HeartBeatApiPlugin();

public:
    auto method() -> ApiName;
    auto json_parser(const std::string &) -> std::optional<ApiRequest>;
    auto process(const ApiRequest &) -> ApiReply;
};

using HeartBeatApiPluginPtr = std::shared_ptr<HeartBeatApiPlugin>;