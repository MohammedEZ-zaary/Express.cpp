#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <map>
#include <string>
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

// // Response Header Class
class Response {
private:
  SOCKET clientSocket;
  std::map<std::string, std::string>
      headers; // Store headers as key-value pairs
  // Private Method
  void setBody(const std::string &body);

public:
  Response(SOCKET socket);
  std::string statusCode = "200 OK"; // Default status code
  std::string body;                  // Response body

  //    // Constructor that accepts a reference to the server

  void setStatusCode(std::string status);
  void sendData(const std::string &data);
  std::string getResponseString();
  void sendFile(const std::string &filePath);
  void setClientSocket(const SOCKET &clientSocket);
  SOCKET getClientSocket();
  void setHeader(const std::string &key, const std::string &value);
  std::string getHeader(const std::string &key) const;
  void printHeaders() const;
};
#endif
