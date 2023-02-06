#include "grpcServer.h"
#include "IPCService.h"
#include "loggerManager.h"

struct GrpcServer::GrpcServerImpl {
    ~GrpcServerImpl();
    
    void init(ServiceFunc, ServiceBufferFunc, ConfigNode);
    bool start();
    void wait();
    void shutdown();

    std::unique_ptr<IPCService> _service;
    std::unique_ptr<Server> _server;
    std::unique_ptr<ServerBuilder> _builder;
};

GrpcServer::GrpcServerImpl::~GrpcServerImpl() = default;
    
void GrpcServer::GrpcServerImpl::init(ServiceFunc func, ServiceBufferFunc buf_func, ConfigNode api) {
    _service = std::make_unique<IPCService>();
    _builder = std::make_unique<ServerBuilder>();

    _service->init(func, buf_func);

    std::string ip = api["ip"];
    std::string port = api["port"];
    std::string server_port(fmt::format("{}:{}", ip, port));

    _builder->AddListeningPort(server_port, grpc::InsecureServerCredentials());
    _builder->RegisterService(_service.get());

    Info(GLOBAL_LOG, "init grpc server... (version: {})", grpc_version_string());
    Info(GLOBAL_LOG, "grpc listing port: {}", server_port);
}

bool GrpcServer::GrpcServerImpl::start() {
    _server = _builder->BuildAndStart();
    if(_server) Info(GLOBAL_LOG, "grpc build and start successful!");
    else Error(GLOBAL_LOG, "grpc build and start failed!");
    return _server != nullptr;
}

void GrpcServer::GrpcServerImpl::wait() {
    Info(GLOBAL_LOG, "grpc server start wait!");
    _server->Wait();
}

void GrpcServer::GrpcServerImpl::shutdown() {
    Warn(GLOBAL_LOG, "grpc server shutdown!");
    _server->Shutdown();
}

GrpcServer::GrpcServer()
    : _impl(std::make_unique<GrpcServerImpl>()) {}

GrpcServer::~GrpcServer() = default;

void GrpcServer::init(ServiceFunc func, ServiceBufferFunc buf_func, ConfigNode api) { _impl->init(func, buf_func, api); }

bool GrpcServer::start() { return _impl->start(); }

void GrpcServer::wait() { _impl->wait(); }

void GrpcServer::shutdown() { _impl->shutdown(); }