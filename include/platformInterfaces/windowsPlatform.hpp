#ifndef WINDOWS_PLATFORM_HPP
#define WINDOWS_PLATFORM_HPP
#include "platformInterface.hpp"


class WindowsPlatform : public PlatformInterface  {
public:
  WindowsPlatform() ;

  bool createSocket() override;
  bool bindSocket() override;
  bool listenForConnections() override;
  void acceptConnections() override;
  void processClientRequest(SOCKET clientSocket) override ;
  void setPort(int port) override ;

  PoolManagment poolManagment ;
  
private:
  SOCKET serverSocket ;
  int port ;
};
#endif
