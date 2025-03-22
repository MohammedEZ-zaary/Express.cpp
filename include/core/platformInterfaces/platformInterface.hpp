#ifndef PLATFORM_INTERFACE_HPP
#define PLATFORM_INTERFACE_HPP
#include "../poolManagment.hpp"
#include <vector>
#include "../route.hpp"


// windows
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif

// linux
#ifdef __linux__
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

class PlatformInterface {
public:
  virtual ~PlatformInterface() = default;
  virtual bool createSocket() = 0;
  virtual bool bindSocket() = 0;
  virtual bool listenForConnections() = 0;
  virtual void acceptConnections() = 0;
  virtual void processClientRequest(SOCKET clientSocket) = 0; // clientSocket , request

  virtual void  setPort(int port) = 0 ;
  // properties
  virtual  void setRoute(const Route& route) = 0;
  virtual Route getRoute(const std::string& routeName) = 0;



  PoolManagment pool;
 
  private:
    int port ;
    

};

#endif
