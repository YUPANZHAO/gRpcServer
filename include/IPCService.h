#pragma once

#include "commonInterface.hpp"
#include <grpcpp/grpcpp.h>
#include <memory>
#include "IPC.grpc.pb.h"

using grpc::Server;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerBuilder;
using grpc::ServerWriter;
using IPC::IPCSrv;
using IPC::IPCRequest;
using IPC::IPCReply;
using IPC::FileReply;

class IPCService final : public IPCSrv::Service {
public:
    IPCService() : _service_call(nullptr) {}
    Status call(ServerContext*, const IPCRequest*, IPCReply*) override;
    void init(ServiceFunc, ServiceBufferFunc);
    Status streamCall(ServerContext* context, const IPCRequest* request, ServerWriter<IPCReply>* writer) override;
    Status recordDownload(ServerContext* context, const IPCRequest* request, ServerWriter<FileReply>* writer) override;

private:
    ServiceFunc _service_call;
    ServiceBufferFunc _service_buffer_call;
};