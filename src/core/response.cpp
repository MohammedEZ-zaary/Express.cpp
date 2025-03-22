#include "../../include/core/response.hpp"
#include "../../include/core/contentTypeMap.hpp"
#include "../../include/core/fileSystemManagment.hpp"
#include <sstream>

Response::Response(SOCKET socket) : clientSocket(socket) {
  headers["Content-Type"] = "text/html";
  headers["Content-Length"] = "";
  headers["Connection"] = "keep-alive";
  headers["Server"] = "HandsomeServer/1.0";
}

SOCKET Response::getClientSocket() { return clientSocket; };
void Response::setStatusCode(std::string status) { this->statusCode = status; }

void Response::setBody(const std::string &body) { this->body = body; }

void Response::sendData(const std::string &data) {
  setBody(data);
  setHeader("Content-Length", std::to_string(data.size()));
  std::string resAsString = getResponseString();

  send(clientSocket, resAsString.c_str(), resAsString.size(), 0);
}
// void setClientSocket(const SOCKET& clientSocket) {
//	clientSocket = clienSocket;
// }
void Response::sendFile(const std::string &filePath) {
  // add Error Handler
  ContentTypeMapper contentType;
  std::string CONTENT_TYPE_SELECTOR = contentType.getContentTypeHeader(
      filePath); // return text/html or application/json it deppend on file
                 // format
  std::string data = FileManager::readFileContent(filePath);
  setHeader("Content-Type", CONTENT_TYPE_SELECTOR);
  setHeader("Connection", "close");
  setHeader("Content-Length", std::to_string(data.size()));
  setStatusCode("200");
  // read content of file and put it on body Header
  setBody(data);
  std::string resAsString = getResponseString();
  send(clientSocket, resAsString.c_str(), resAsString.size(), 0);
}

std::string Response::getResponseString() {
  std::ostringstream responseStream;

  responseStream << "HTTP/1.1 " << this->statusCode << "\r\n";

  for (const auto &pair : headers) {
    responseStream << pair.first << ": " << pair.second << "\r\n";
  }

  responseStream << "\r\n"; // End of headers

  if (!this->body.empty()) {
    responseStream << this->body;
  }
  return responseStream.str();
}

void Response::setHeader(const std::string &key, const std::string &value) {
  headers[key] = value;
}

std::string Response::getHeader(const std::string &key) const {
  auto it = headers.find(key);
  if (it != headers.end()) {
    return it->second;
  }
  return ""; // Return empty string if the header is not found
}
void Response::setClientSocket(const SOCKET &cSkt) {
  // cSkt = client socket
  clientSocket = cSkt;
}
// void Response::printHeaders() const {
//   for (const auto &pair : headers) {
//     std::cout << pair.first << ": " << pair.second << std::endl;
//   }
// }
