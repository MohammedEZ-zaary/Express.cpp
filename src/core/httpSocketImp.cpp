#include "../../include/core/httpSocket.hpp"


httpServer::httpServer(std::unique_ptr<PlatformInterface> platformImpl, std::unique_ptr<PoolManagment> pool , int port ) : platform(std::move(platformImpl)) ,poolManagment(std::move(pool)) {
    platform->setPort(port);
}

void httpServer::run() {
    if (platform->createSocket() && platform->bindSocket() && platform->listenForConnections()) {
        platform->acceptConnections();
    }
    // Add Ene Error Handler Here
}
