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

  void setRoute(const Route& route) override;
   Route getRoute(const std::string& routeName) override;



private:
  SOCKET serverSocket ;
  int port ;
  std::vector<Route> routes;
  Route Error404Page = Route("/404", [](const Request& , Response) {});
};
#endif
