#ifndef LINUX_PLATFORM_HPP
#define LINUX_PLATFORM_HPP

#include "platformInterface.hpp"

class LinuxPlatform : public PlatformInterface {
public:
  bool createSocket() override;
  bool bindSocket() override;
  bool listenForConnections() override;
  void acceptConnections() override;
  void processClientRequest(SOCKET clientSocket) override;
  void setPort(int port) override;
private :
int port ;
};
#endif
