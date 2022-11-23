#include "apiServer.h"

int main() {
    
    auto apiServer = std::make_unique<ApiServer>();
    apiServer->start();
    apiServer->wait();

    return 0;
}