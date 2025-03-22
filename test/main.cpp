#include "../include/core/multiPartFormDataHandler.hpp"
#include "../include/expressCpp.hpp"

#include <iostream>

int main() {
  ExpressCpp app;
  app.setStaticRouteFolder("test/templates/");

  app.route("/", [](const Request &req, Response &res) {
    //  std::cout << "wellcome to /  route" << std::endl ;
    res.setHeader("Content-Type", "text/html");
    res.sendFile("test/index.html");
  });

  app.route("/up", [&app](const Request &req, Response &res) {
    res.setHeader("Content-Type", "text/html");

    FormData upload(req, res.getClientSocket(), 2, "test/uploads/");

    res.sendData("<h1>File uploaded</h1>");
    std::cout << "finish " << std::endl;
  });

  app.run(8080);
  // Define a route for the root URL

  return 0;
}
