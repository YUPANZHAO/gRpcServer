#pragma once

#include "commonInterface.hpp"

class GenKeyApiPlugin : public IApiPlugin {
public:
    GenKeyApiPlugin();
    ~GenKeyApiPlugin();

public:
    auto method() -> ApiName;
    auto json_parser(const std::string &) -> std::optional<ApiRequest>;
    auto process(const ApiRequest &) -> ApiReply;
};

using GenKeyApiPluginPtr = std::shared_ptr<GenKeyApiPlugin>;