#pragma once

#include "commonInterface.hpp"

class MsgCBApiPlugin : public IApiPlugin {
public:
    MsgCBApiPlugin(ServiceFunc);
    ~MsgCBApiPlugin();

public:
    auto method() -> ApiName;
    auto json_parser(const std::string &) -> std::optional<ApiRequest>;
    auto process(const ApiRequest &) -> ApiReply;

private:
    ServiceFunc _cb;
};

using MsgCBApiPluginPtr = std::shared_ptr<MsgCBApiPlugin>;