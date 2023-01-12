#include <iostream>
#include <grpcpp/grpcpp.h>
#include "IPC.grpc.pb.h"
#include <memory>
#include <optional>
#include "nlohmann/json.hpp"

using nlohmann::json;

using grpc::ClientContext;
using grpc::Status;
using grpc::Channel;
using IPC::IPCSrv;
using IPC::IPCRequest;
using IPC::IPCReply;

class IPCService {
public:
    IPCService(std::shared_ptr<Channel> channel)
        : _stub(std::make_unique<IPCSrv::Stub>(channel)) {}
    
    auto TestIPC(const std::string& str) -> std::string {
        IPCRequest request;
        IPCReply reply;
        ClientContext context;

        request.set_body(str);
        
        Status status = _stub->call(&context, request, &reply);
        
        if(status.ok()) {
            return reply.body();
        }else {
            std::cout << "IPC ERROR: " << status.error_code() << " " << status.error_message() << std::endl; 
            return "";
        }
    }

private:
    std::unique_ptr<IPCSrv::Stub> _stub;
};

int main() {

    auto ipcService = std::make_unique<IPCService>(
        grpc::CreateChannel("192.168.43.59:50052", grpc::InsecureChannelCredentials())
    );

    while(true) {
        std::string method, id;
        std::cin >> method >> id;
        std::string str = json({
            { "method", method },
            { (method == "genkey" ? "device_id" : "key"), id }
        }).dump();

        std::cout << "Reply: \n" << ipcService->TestIPC(str) << std::endl;
    }

    return 0;
}