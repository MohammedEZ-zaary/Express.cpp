#include "../include/core/httpParsing.hpp"
#include "../include/core/platformInterfaces/windowsPlatform.hpp"
#include "../include/core/response.hpp"
#include <iostream>

WindowsPlatform::WindowsPlatform() : serverSocket(INVALID_SOCKET) {};

void WindowsPlatform::setPort(int port) { this->port = port; };

bool WindowsPlatform::createSocket() {
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cerr << "WSAStartup failed" << std::endl;
    return false;
  }

  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == INVALID_SOCKET) {
    std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
    WSACleanup();
    return false;
  }
  return true;
};

bool WindowsPlatform::bindSocket() {
  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(port);

  if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) ==
      SOCKET_ERROR) {
    std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
    closesocket(serverSocket);
    WSACleanup();
    return false;
  } else {
    return true;
  }
};

bool WindowsPlatform::listenForConnections() {
  if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
    std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
    closesocket(serverSocket);
    WSACleanup();
    return false;
  }

  std::cout << "Server started on port " << port << std::endl;
  return true;
};

void WindowsPlatform::acceptConnections() {
  // Set the server socket to non-blocking mode
  u_long mode = 1; // 1 for non-blocking, 0 for blocking

  ioctlsocket(serverSocket, FIONBIO, &mode);
  while (true) {
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(serverSocket, &readSet);

    // Use select() to wait for incoming connections
    int selectResult = select(0, &readSet, nullptr, nullptr, nullptr);
    if (selectResult == SOCKET_ERROR) {
      std::cerr << "Select failed: " << WSAGetLastError() << std::endl;
      break;
    }

    if (FD_ISSET(serverSocket, &readSet)) {
      sockaddr_in clientAddr;
      int clientAddrSize = sizeof(clientAddr);

      // Accept the connection
      SOCKET clientSocket =
          accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrSize);
      if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
        continue;
      }

      // Set the client socket to non-blocking mode
      ioctlsocket(clientSocket, FIONBIO, &mode);

      // Process the client request asynchronously

      pool.addRequestExecution(
          [this, clientSocket]() { processClientRequest(clientSocket); });
    }
  }

  closesocket(serverSocket);
  WSACleanup();
};

void WindowsPlatform::processClientRequest(SOCKET clientSocket) {
  char buffer[4048] = {0};
  int bytesReceived;
  ParsingHttpHeader parseHeader;

  while (true) {
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
      if (WSAGetLastError() == WSAEWOULDBLOCK) {
        // No data available, wait for the next event
        Sleep(100);
        continue;
      }
      std::cerr << "Connection closed or error: " << WSAGetLastError()
                << std::endl;
      break;
    }
    // Start proccessing http header //
    // Step 1 : parse header
    std::string buffAsString(buffer);
    // std::cout << buffAsString << std::endl;
    parseHeader << buffAsString;

    parseHeader.parse();
    // After parsed  http header
    const ParsedHeader &parsed = parseHeader.getParsedHeader();
    // Step 2 : find the route
    Route route = getRoute(parsed.url);
    // Step 3 : pass the response and request header to route excute function
    Request request;
    // init response
    Response response(clientSocket);
    request.headerProprties = parsed;
    if (request.headerProprties.contentType.find("multipart/form-data") !=
        std::string::npos) {
      pool.addIo([route, clientSocket, parsed]() {
        // I/O thread
        Request request;
        request.headerProprties = parsed;
        // init response
        Response response(clientSocket);
        route.executor(request, response);
        closesocket(clientSocket);
      });
    } else {
      // this main thread
      route.executor(request, response);
      closesocket(clientSocket);
      break;
    }
    break;
    //
  }
}

void WindowsPlatform::setRoute(const Route &route) {
  // Function to add a route
  if (route.routeName == "/404") {
    Error404Page = route;
    return;
  }
  // first route
  if (routes.empty()) {
    routes.push_back(route);

    return;
  }
  routes.push_back(route);
}
Route WindowsPlatform::getRoute(const std::string &routeName) {
  // Function to get a route by name
  // search for Name of The client Route
  for (Route &route : routes) {
    if (route.routeName == routeName && routeName != "/404") {
      return route;
    }
  }

  return Error404Page;
};
