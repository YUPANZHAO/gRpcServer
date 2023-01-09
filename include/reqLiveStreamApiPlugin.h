#pragma once

#include "commonInterface.hpp"

class ReqLiveStreamApiPlugin : public IApiPlugin {
public:
    ReqLiveStreamApiPlugin();
    ~ReqLiveStreamApiPlugin();

public:
    auto method() -> ApiName;
    auto json_parser(const std::string &) -> std::optional<ApiRequest>;
    auto process(const ApiRequest &) -> ApiReply;
};

using ReqLiveStreamApiPluginPtr = std::shared_ptr<ReqLiveStreamApiPlugin>;