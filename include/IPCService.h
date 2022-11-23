#pragma once

#include "commonInterface.hpp"
#include <grpcpp/grpcpp.h>
#include "IPC.grpc.pb.h"

using grpc::Server;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerBuilder;
using IPC::IPCSrv;
using IPC::IPCRequest;
using IPC::IPCReply;

class IPCService final : public IPCSrv::Service {
public:
    IPCService() : _service_call(nullptr) {}
    Status call(ServerContext*, const IPCRequest*, IPCReply*) override;
    void init(ServiceFunc);

private:
    ServiceFunc _service_call;
};