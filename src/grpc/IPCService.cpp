#include "IPCService.h"
#include "loggerManager.h"

Status IPCService::call(ServerContext* context, const IPCRequest* request, 
IPCReply* reply) {
    if(_service_call == nullptr) {
        Error(GLOBAL_LOG, "service call function is nullptr!");
        return Status::CANCELLED;
    }
    std::string msg = _service_call(request->body());
    reply->set_body(std::move(msg));
    return Status::OK;
}

void IPCService::init(ServiceFunc func) {
    _service_call = func;
}