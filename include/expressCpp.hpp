#ifndef EXPRESS_CPP_HPP
#define EXPRESS_CPP_HPP

#include "core/platformInterfaces/platformInterface.hpp"
#include "core/poolManagment.hpp"
#include "core/response.hpp"
#include "core/route.hpp"
#include <iostream>
#include <memory>

class ExpressCpp {

private:
  std::vector<Route> routes;
  int port = 8080; // default
  std::unique_ptr<PlatformInterface> platform;
  // default route for none exists . we can override it.
  Route Error404Page = Route("/404", [](const Request &, Response &) {});

  std::string staticRootFolderPath; // Path to the static files
  // Serve static files from a directory
  void serveStaticFile(const std::string &staticFolderName);

  void setRoute(const Route &route);
  Route getRoute(const std::string &routeName);

public:
  ExpressCpp();
  void
  route(const std::string &routeName,
        const std::function<void(const Request &, Response &)> &lambdaExcuter);
  // test
  std::unique_ptr<PoolManagment> pool;
  void run(int port);
  // set static folders like images,icons , videos to be access by the browser
  // like : /favicon.icon
  void setStaticRouteFolder(const std::string &folderPath);

  std::string getStaticRootFolderPath();

  std::string readFileContent(const std::string &filePath);
};

#endif
