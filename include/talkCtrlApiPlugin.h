#pragma once

#include "commonInterface.hpp"

class TalkCtrlApiPlugin : public IApiPlugin {
public:
    TalkCtrlApiPlugin();
    ~TalkCtrlApiPlugin();

public:
    auto method() -> ApiName;
    auto json_parser(const std::string &) -> std::optional<ApiRequest>;
    auto process(const ApiRequest &) -> ApiReply;

private:
    // 成功返回 rtmp_url和true, 失败返回 err_msg和false
    auto handle_start(const std::string & device_key) -> std::tuple<std::string, bool>;
    void handle_stop(const std::string & device_key); 
};

using TalkCtrlApiPluginPtr = std::shared_ptr<TalkCtrlApiPlugin>;