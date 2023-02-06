#pragma once

#include "commonInterface.hpp"

class RecordDownloadApiPlugin : public IApiPlugin {
public:
    RecordDownloadApiPlugin(BufferCallBack);
    ~RecordDownloadApiPlugin();

public:
    auto method() -> ApiName;
    auto json_parser(const std::string &) -> std::optional<ApiRequest>;
    auto process(const ApiRequest &) -> ApiReply;

private:
    BufferCallBack buffer_callback;
};

using RecordDownloadApiPluginPtr = std::shared_ptr<RecordDownloadApiPlugin>;