#pragma once

#include "commonInterface.hpp"
    
class ApiServer : public IApiServer {
public:
    ApiServer();
    ~ApiServer() override;

public:
    bool start() override;
    void wait() override;
    void shutdown() override;

private:
    struct ApiServerImpl;
    std::unique_ptr<ApiServerImpl> _impl;
};