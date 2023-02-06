#pragma once

#include "commonInterface.hpp"

class GrpcServer : public IRpcServer {
public:
    GrpcServer();
    ~GrpcServer() override;

public:
    void init(ServiceFunc, ServiceBufferFunc, ConfigNode) override;
    bool start() override;
    void wait() override;
    void shutdown() override;

private:
    struct GrpcServerImpl;
    std::unique_ptr<GrpcServerImpl> _impl;
};