#ifndef ROUTES_HPP
#define ROUTES_HPP
#include <functional>
#include <string>
#include "request.hpp"
 #include "response.hpp"

// Route Class
class Route {
    public:
      std::string routeName;
      std::function<void(const Request& , Response &)> executor;
    
      Route(const std::string &routeName,
            const std::function<void(const Request & , Response&)> &executor)
          : routeName(routeName), executor(executor) {}
    };

#endif