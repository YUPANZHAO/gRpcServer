#include "IPCService.h"
#include "loggerManager.h"

Status IPCService::call(ServerContext* context, const IPCRequest* request, 
IPCReply* reply) {
    if(_service_call == nullptr) {
        Error(GLOBAL_LOG, "service call function is nullptr!");
        return Status::CANCELLED;
    }
    std::string msg = _service_call(request->body());
    reply->set_body(msg);
    return Status::OK;
}

void IPCService::init(ServiceFunc func, ServiceBufferFunc buf_func) {
    _service_call = func;
    _service_buffer_call = buf_func;
}

Status IPCService::streamCall(ServerContext* context, 
const IPCRequest* request, ServerWriter<IPCReply>* writer) {
    if(_service_call == nullptr) {
        Error(GLOBAL_LOG, "service call function is nullptr!");
        return Status::CANCELLED;
    }
    auto reply = std::make_unique<IPCReply>();
    Info(GLOBAL_LOG, "客户端回调开启 {}", request->body());
    while(true) {
        std::string data = _service_call(request->body());
        if(data == "") break;
        Info(GLOBAL_LOG, "获得一条回调数据 {}", data);
        reply->set_body(data);
        if(!writer->Write(*reply)) {
            break;
        }
        Info(GLOBAL_LOG, "回调数据发送成功");
    }
    Info(GLOBAL_LOG, "回调线程结束");
    return Status::OK;
}

Status IPCService::recordDownload(ServerContext* context,
const IPCRequest* request, ServerWriter<FileReply>* writer) {
    if(_service_call == nullptr) {
        Error(GLOBAL_LOG, "service call function is nullptr!");
        return Status::CANCELLED;
    }
    auto reply = std::make_unique<FileReply>();
    Info(GLOBAL_LOG, "开始传输录像 {}", request->body());

    _service_buffer_call("record_download", request->body(), [&](char* buf, int len) {
        reply->set_buffer(buf, len);
        if(!writer->Write(*reply)) return;
    });
        
    Info(GLOBAL_LOG, "录像传输结束");
    return Status::OK;
}