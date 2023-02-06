#include "recordDownloadApiPlugin.h"
#include "loggerManager.h"
#include "nlohmann/json.hpp"
#include "DeviceManager.h"

using nlohmann::json;

RecordDownloadApiPlugin::RecordDownloadApiPlugin(BufferCallBack buf_cb)
: buffer_callback(buf_cb) {}

RecordDownloadApiPlugin::~RecordDownloadApiPlugin() {}

auto RecordDownloadApiPlugin::method() -> ApiName {
    return "record_download";
}

auto RecordDownloadApiPlugin::json_parser(const std::string & data) -> std::optional<ApiRequest> {
    RECORD_DOWNLOAD_INFO info = {};
    auto json_data = json::parse(data);
    try {
        info.key = json_data["key"];
        info.begin_time = json_data["begin_time"];
        info.end_time = json_data["end_time"];
    }catch(std::exception&) {
        Warn(GLOBAL_LOG, "method({}): json parse failed!", method());
        return std::nullopt;
    }
    ApiRequest req = info;
    return req;
}

auto RecordDownloadApiPlugin::process(const ApiRequest & req) -> ApiReply {
    auto info = std::get<RECORD_DOWNLOAD_INFO>(req);
    Info(GLOBAL_LOG, "method({}): key = {}, begin_time = {}, end_time = {}", method(), info.key, info.begin_time, info.end_time);

    DeviceManager::getInstance()->recordDownload(info.key, info.begin_time, info.end_time, buffer_callback);
    
    return { std::nullopt, "" };
}
