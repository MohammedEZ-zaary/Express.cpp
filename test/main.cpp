#include "../include/core/httpSocket.hpp"
#include "../include/core/platformInterfaces/platformFactory.hpp"
#include "../include/core/poolManagment.hpp"

#include <iostream>

int main() {
  PoolManagment pool ;
  
  httpServer app(createPlatform(), std::make_unique<PoolManagment>() ,  8080) ;
  
  app.run();
  // Define a route for the root URL

  return 0;
}
