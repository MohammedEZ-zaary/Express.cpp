#ifndef HTTP_SOCKET 
#define HTTP_SOCKET
#include <memory>
#include "../platformInterfaces/platformInterface.hpp"
#include "poolManagment.hpp"

class httpServer {
private:
    std::unique_ptr<PlatformInterface> platform ;
    std::unique_ptr<PoolManagment> poolManagment;
    

    // Other members...

public:
    httpServer(std::unique_ptr<PlatformInterface> platformImpl ,  std::unique_ptr<PoolManagment> pool, int port);
    void run();
    
};

#endif